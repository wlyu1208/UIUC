module Reg_f(input logic Clk, Reset, LD_REG,
				 input logic[15:0] in,
				 output logic[15:0] sr2_out, sr1_out,
				 input logic[2:0] sr2, dr_m, sr1_m);

		logic [7:0][15:0] data_temp;
		
always_ff @ (posedge Clk or posedge Reset)
begin
	if(Reset)
		begin
			data_temp[0]=16'h0000;
			data_temp[1]=16'h0000;
			data_temp[2]=16'h0000;
			data_temp[3]=16'h0000;
			data_temp[4]=16'h0000;
			data_temp[5]=16'h0000;
			data_temp[6]=16'h0000;
			data_temp[7]=16'h0000;
		end
	else if(LD_REG)
		begin
			data_temp[dr_m]=in;
		end
end		
	assign sr2_out=data_temp[sr2];
	assign sr1_out=data_temp[sr1_m];
endmodule
