
module lab1_qsys (
	clk_clk,
	pioleds_external_connection_export,
	piopushbutton_external_connection_export,
	pioswitch_external_connection_export,
	reset_reset_n);	

	input		clk_clk;
	output	[9:0]	pioleds_external_connection_export;
	input		piopushbutton_external_connection_export;
	input	[3:0]	pioswitch_external_connection_export;
	input		reset_reset_n;
endmodule
