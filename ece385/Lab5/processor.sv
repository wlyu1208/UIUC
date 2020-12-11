//toplevel of 8bit

module processor(input logic clk, Reset, ClrA_LdB, Run,
					  input logic[7:0] Sw,
					  output logic[6:0] AhexU, AhexL, BhexU, BhexL,
					  output logic X,
					  output logic[7:0] Aval, Bval);
	
	logic shift_t, add_t, sub_t, CL_t, X_t, A_L, B_L, fini;
	logic[7:0] A, B;
	logic[8:0] s_adder;
	logic reset_out, clrA_ldB_out, run_out;
	
	control		 CTRL	       (.Clk(clk), .Reset(reset_out), .ClearA_LoadB(clrA_ldB_out), .Run(run_out),  
								     .m(B_L), .Shift(shift_t), .Clr_Ld(CL_t), .Add(add_t), .Sub(sub_t), .fin(fini));	
	
	reg_8 		 REGA  		 (.Clk(clk), .Reset(reset_out|CL_t), .Load(add_t|sub_t), .Shift_In(X_t), 
								     .Shift_En(shift_t), .D_in(s_adder[7:0]), .Shift_Out(A_L), .Dout(A[7:0]));
									  
	reg_8 		 REGB		    (.Clk(clk), .Reset(reset_out), .Load(CL_t), .Shift_In(A_L), 
								     .Shift_En(shift_t), .D_in(Sw[7:0]), .Shift_Out(B_L), .Dout(B[7:0]));
	
	ninebitadder S0		 	 (.A(A[7:0]), .S(Sw[7:0]), .W(sub_t), .XA(s_adder[8:0]));
	
	flipflop	 	 FF0			 (.Clk(clk), .Load(add_t|sub_t), .Reset(reset_out|clrA_ldB_out), 
								     .D1(X_t), .D2(s_adder[8]), .Q(X_t));
	 
	HexDriver        HexAL ( .In0(A[3:0]), .Out0(AhexL[6:0]) );
	HexDriver        HexBL ( .In0(B[3:0]), .Out0(BhexL[6:0]) );	
	HexDriver        HexAU ( .In0(A[7:4]), .Out0(AhexU[6:0]) );				
	HexDriver        HexBU ( .In0(B[7:4]), .Out0(BhexU[6:0]) );
	
	always_comb
	begin
	X=X_t;
	Aval=A;
	Bval=B;
	end
	
	sync button_sync[2:0] (clk, {~Reset, ~ClrA_LdB, ~Run}, {reset_out, clrA_ldB_out, run_out});
	sync Sw_sync[7:0] (clk, Sw);
	
endmodule
