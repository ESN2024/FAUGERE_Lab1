library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

ENTITY lab1 IS
PORT(
	clk : IN std_logic;
	rst : IN std_logic;
	
	leds : OUT std_logic_vector(9 downto 0);
	pushButton : IN std_logic;
	switch : IN std_logic_vector(3 downto 0)
);
END lab1;

ARCHITECTURE archi OF lab1 IS

component lab1_qsys is
  port (
		clk_clk                                  : in  std_logic                    := 'X';             -- clk
		pioleds_external_connection_export       : out std_logic_vector(9 downto 0);                    -- export
		piopushbutton_external_connection_export : in  std_logic                    := 'X';             -- export
		pioswitch_external_connection_export     : in  std_logic_vector(3 downto 0) := (others => 'X'); -- export
		reset_reset_n                            : in  std_logic                    := 'X'              -- reset_n
  );
end component lab1_qsys;
 
BEGIN

u0 : component lab1_qsys
port map (
	clk_clk                                  => clk,                                  --                               clk.clk
	pioleds_external_connection_export       => leds,       --       pioleds_external_connection.export
	piopushbutton_external_connection_export => pushButton, -- piopushbutton_external_connection.export
	pioswitch_external_connection_export     => switch,     --     pioswitch_external_connection.export
	reset_reset_n                            => rst                             --                             reset.reset_n
);


END archi;