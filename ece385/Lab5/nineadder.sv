module ninebitadder(input logic [7:0] A, S,
						  input logic W,
						  output logic [8:0] XA);

	logic c0, c1, c2, c3, c4, c5, c6, c7;
	// x,y=input z=cin s=sum c=cout
	logic[7:0] S2;
	logic temp1, temp2;
	
	always_comb
	begin
	temp1=A[7];
	
	S2=(S^{8{W}}); 
	temp2=S2[7];
	end
	
	fulladder fa0 (.x(A[0]), .y(S2[0]), .c_in( W), .s(XA[0]), .c_out(c0));
	fulladder fa1 (.x(A[1]), .y(S2[1]), .c_in(c0), .s(XA[1]), .c_out(c1));
	fulladder fa2 (.x(A[2]), .y(S2[2]), .c_in(c1), .s(XA[2]), .c_out(c2));
	fulladder fa3 (.x(A[3]), .y(S2[3]), .c_in(c2), .s(XA[3]), .c_out(c3));
	fulladder fa4 (.x(A[4]), .y(S2[4]), .c_in(c3), .s(XA[4]), .c_out(c4));
	fulladder fa5 (.x(A[5]), .y(S2[5]), .c_in(c4), .s(XA[5]), .c_out(c5));
	fulladder fa6 (.x(A[6]), .y(S2[6]), .c_in(c5), .s(XA[6]), .c_out(c6));
	fulladder fa7 (.x(A[7]), .y(S2[7]), .c_in(c6), .s(XA[7]), .c_out(c7));
	fulladder fa8 (.x(temp1), .y(temp2), .c_in(c7), .s(XA[8]), .c_out());
	
endmodule


module fulladder(input logic x, y, c_in,
					  output logic s, c_out);
	assign s=x^y^c_in;
	assign c_out=(x&y)|(x&c_in)|(y&c_in);
endmodule
