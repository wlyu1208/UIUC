module register_unit (input  logic Clk, Reset, A_In, B_In, Ld_A, Ld_B, 
                            Shift_En,
                      input  logic [7:0]  D_in, 
                      output logic A_out, B_out, 
                      output logic [7:0]  A, B);


    reg_8  reg_A (.*, .Shift_In(A_In), .Load(Ld_A), .Shift_Out(A_out), .Dout(A));
    reg_8  reg_B (.*, .Shift_In(B_In), .Load(Ld_B), .Shift_Out(B_out), .Dout(B));

endmodule

module reg_8(input logic Clk, Reset, Load, Shift_In, Shift_En,
				 input logic[7:0] D_in,
				 output logic[7:0] Dout,
				 output logic Shift_Out);
	always_ff @ (posedge Clk)
	begin
		if(Reset)
			Dout<=8'h0;
		else if(Load)
			Dout<=D_in;
		else if(Shift_En)
			Dout<= {Shift_In, Dout[7:1] };
	end
	
	assign Shift_Out=Dout[0];

endmodule
