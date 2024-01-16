#include <stdint.h>
#include "../bsp/system.h"
#include "../bsp/drivers/inc/altera_avalon_pio_regs.h"
#include "../bsp/drivers/inc/altera_avalon_timer_regs.h"
#include "../bsp/HAL/inc/sys/alt_stdio.h"
#include "../bsp/HAL/inc/sys/alt_irq.h"	
#include "../bsp/HAL/inc/sys/alt_sys_init.h"
#include <io.h>
#include <alt_types.h>

#define NBR_USED_LEDS 9 // nombre de leds utilisés
#define CHASER_LENGTH 4 // taille du chenillard


static void pushButton_IRQHandler (void * context, alt_u32 id)	{
	static alt_8 start = 0;
	start = !start;

	// on démarre ou on stoppe le compteur
	if(start) IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, (IORD_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE)|ALTERA_AVALON_TIMER_CONTROL_START_MSK)&(~ALTERA_AVALON_TIMER_CONTROL_STOP_MSK));
	else  IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, (IORD_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE)|ALTERA_AVALON_TIMER_CONTROL_STOP_MSK)&(~ALTERA_AVALON_TIMER_CONTROL_START_MSK));

	// clear edge capture interrupt flag
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIOPUSHBUTTON_BASE, 1);
}

static void switch_IRQHandler (void * context, alt_u32 id)	{
	// changement vitesse (période du timer)
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, IORD_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE)&
		(~(ALTERA_AVALON_TIMER_CONTROL_START_MSK|ALTERA_AVALON_TIMER_CONTROL_STOP_MSK))); // permet de ne pas arrêter le compteur lors du changement de la période
	alt_u32 timerCnt = 500000 + (alt_u32)IORD_ALTERA_AVALON_PIO_DATA(PIOSWITCH_BASE)*(50000000 - 500000)/15;

	IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_BASE, (timerCnt)&0xFFFF);
	IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_BASE, ((timerCnt)&(0xFFFF<<16))>>16);

	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIOSWITCH_BASE, 0xF);
}

static void timer_IRQHandler (void * context, alt_u32 id)	{
	static alt_u8 nbrNewLeds = 0; // nombre de nouvelles leds allumées 
	static alt_16 leds = 0;

	if(((leds>>(NBR_USED_LEDS-1)) & 1 == 1) && (nbrNewLeds == CHASER_LENGTH))	{
		nbrNewLeds = 0;
	}
	leds = leds<<1;
	if(nbrNewLeds < CHASER_LENGTH)	{
		nbrNewLeds++;
		leds = leds|0b1;
	}
	IOWR_ALTERA_AVALON_PIO_SET_BITS(PIOLEDS_BASE, leds);
	IOWR_ALTERA_AVALON_PIO_CLEAR_BITS(PIOLEDS_BASE, ~leds);

	// clear timeout status register
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE, ALTERA_AVALON_TIMER_STATUS_TO_MSK);
}

int main()	{
	alt_irq_init(NULL);
	
	/* --- pushButton interrupt init --- */
	alt_ic_isr_register(PIOPUSHBUTTON_IRQ_INTERRUPT_CONTROLLER_ID,
                        PIOPUSHBUTTON_IRQ,
                        pushButton_IRQHandler,
                        NULL,
                        NULL);
	alt_ic_irq_enable (PIOPUSHBUTTON_IRQ_INTERRUPT_CONTROLLER_ID, PIOPUSHBUTTON_IRQ);
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIOPUSHBUTTON_BASE, 1);
	/* --- */

	/* --- switch interrupt init --- */
	alt_ic_isr_register(PIOSWITCH_IRQ_INTERRUPT_CONTROLLER_ID,
                        PIOSWITCH_IRQ,
                        switch_IRQHandler,
                        NULL,
                        NULL);
	alt_ic_irq_enable (PIOSWITCH_IRQ_INTERRUPT_CONTROLLER_ID, PIOSWITCH_IRQ);
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIOSWITCH_BASE, 0xF);
	/* --- */

	/* --- timer interrupt init --- */
	alt_ic_isr_register(TIMER_IRQ_INTERRUPT_CONTROLLER_ID,
                        TIMER_IRQ,
                        timer_IRQHandler,
                        NULL,
                        NULL);
	alt_ic_irq_enable (TIMER_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_IRQ);
	/* --- */

	/* --- timer init --- */
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, ALTERA_AVALON_TIMER_CONTROL_ITO_MSK|ALTERA_AVALON_TIMER_CONTROL_CONT_MSK|ALTERA_AVALON_TIMER_CONTROL_STOP_MSK);
	// set initial period (1s)
	IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_BASE, (500000)&0xFFFF);
	IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_BASE, ((500000)&(0xFFFF<<16))>>16);
	/* --- */

	while(1);

	return 0;
}