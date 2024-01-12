	component lab1_qsys is
		port (
			clk_clk                                  : in  std_logic                    := 'X';             -- clk
			pioleds_external_connection_export       : out std_logic_vector(9 downto 0);                    -- export
			piopushbutton_external_connection_export : in  std_logic                    := 'X';             -- export
			pioswitch_external_connection_export     : in  std_logic_vector(3 downto 0) := (others => 'X'); -- export
			reset_reset_n                            : in  std_logic                    := 'X'              -- reset_n
		);
	end component lab1_qsys;

	u0 : component lab1_qsys
		port map (
			clk_clk                                  => CONNECTED_TO_clk_clk,                                  --                               clk.clk
			pioleds_external_connection_export       => CONNECTED_TO_pioleds_external_connection_export,       --       pioleds_external_connection.export
			piopushbutton_external_connection_export => CONNECTED_TO_piopushbutton_external_connection_export, -- piopushbutton_external_connection.export
			pioswitch_external_connection_export     => CONNECTED_TO_pioswitch_external_connection_export,     --     pioswitch_external_connection.export
			reset_reset_n                            => CONNECTED_TO_reset_reset_n                             --                             reset.reset_n
		);

