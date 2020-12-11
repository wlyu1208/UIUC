/*  
Using Lfsr algorithm to make a random number between 1~7
citation:
http://www.asic-world.com/examples/systemverilog/lfsr.html
*/
module randNum( 	input clk,
						input Reset, signalStart,
						output[2:0] randomNextShape);
assign randomNextShape = randNum;

logic [2:0] randNum = 0;
logic linear_feedback = 0;

always_ff @ (posedge clk & signalStart)
begin
	if(Reset)
		randNum <= {randNum[1:0], linear_feedback};
	else if(randNum==3'b000)
		randNum<=3'b001;
	else
	randNum <= {randNum[1:0], linear_feedback};
end

assign linear_feedback=randNum[2]^randNum[1];
assign randomNextShape=randNum;

endmodule





//
//module randNum( 	input clk,
//						input Reset, signalStart,
//						output[2:0] randomNextShape);
//
//logic [2:0] randNum;
//
//always_ff @ (posedge clk & signalStart)
//begin
//	if(Reset)
//		randNum<=3'b111;
//	else
//	randNum <= randNum % 3'd7 + 1;
//end
//
//assign randomNextShape = randNum;
//
//
//endmodule