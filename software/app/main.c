#include <stdint.h>
#include "../bsp/system.h"
#include "../bsp/drivers/inc/altera_avalon_pio_regs.h"
#include "../bsp/HAL/inc/sys/alt_irq.h"
#include "../bsp/HAL/inc/sys/alt_sys_init.h"
#include <io.h>
#include <alt_types.h>

static void pushButton_IRQHandler (void * context, alt_u32 id)	{
	static alt_u8 index = 0;
	alt_16 leds = 0;
	leds = 1<<index;
	index++;
	IOWR_ALTERA_AVALON_PIO_SET_BITS(PIOLEDS_BASE, leds);

	// clear edge capture interrupt flag
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIOPUSHBUTTON_BASE, 1);
}

static void switch_IRQHandler (void * context, alt_u32 id)	{
	
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
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIOSWITCH_BASE, 1);
	/* --- */

	while(1);

	return 0;
}