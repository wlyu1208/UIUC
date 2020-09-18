/*
 * ECE385-HelperTools/PNG-To-Txt
 * Author: Rishi Thakkar
 *
 */

module BlockBackground
(
		input [9:0] read_address,
		input Clk,
		output logic [23:0] data_Out
);

// mem has width of 3 bits and a total of 400 addresses
logic [23:0] rgbBackground [0:255];

initial
begin
	 $readmemh("tetris_background_gameSpace16x16.txt", rgbBackground);
end


always_ff @ (posedge Clk) 
begin
	data_Out <= rgbBackground[read_address];
end


endmodule
