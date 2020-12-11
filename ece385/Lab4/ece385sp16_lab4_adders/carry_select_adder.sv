//module carry_select_adder
//(
//    input   logic[15:0]     A,
//    input   logic[15:0]     B,
//    output  logic[15:0]     Sum,
//    output  logic           CO
//);
//
//    /* TODO
//     *
//     * Insert code here to implement a carry select.
//     * Your code should be completly combinational (don't use always_ff or always_latch).
//     * Feel free to create sub-modules or other files. */
//     
//	  logic C1, C2, C3;
//	  
//	  four_bit FSA0(.x(A[3:0]), .y(B[3:0]), .cin(1'b0), .s(Sum[3:0]), .COUT(C1));
//	  csa_add_four CSA0(.X(A[7:4]), .Y(B[7:4]), .Cin(C1), .S(Sum[7:4]), .Cout(C2));
//	  csa_add_four CSA1(.X(A[11:8]), .Y(B[11:8]), .Cin(C2), .S(Sum[11:8]), .Cout(C3));
//	  csa_add_four CSA2(.X(A[15:12]), .Y(B[15:12]), .Cin(C3), .S(Sum[15:12]), .Cout(CO));
//	  
//endmodule
//
//module csa_add_four(
//	input[3:0] X, Y,
//	input Cin,
//	output logic[3:0] S,
//	output logic Cout
//);
//
//	logic[3:0] temp0, temp1;
//	logic Cout0, Cout1;
//	
//	four_bit FRA0(.x(X[3:0]), .y(Y[3:0]), .cin(1'b0), .s(temp0[3:0]), .COUT(Cout0));
//	four_bit FRA1(.x(X[3:0]), .y(Y[3:0]), .cin(1'b1), .s(temp1[3:0]), .COUT(Cout1));
//	
//	
//	assign Cout=Cout0|(Cout1&Cin);
//	
//	always_comb begin
//	if(Cin==0)
//		S=temp0;
//	else
//		S=temp1;
//	end
//		
//endmodule
//
//module four_bit(
//	input [3:0] x, 
//	input [3:0] y,
//	input cin,
//	output logic [3:0] s,
//	output logic COUT
//	);
//	
//	logic c0, c1, c2;
//	
//	full_adder fa0(.x(x[0]), .y(y[0]), .cin(cin), .s(s[0]), .cout(c0));
//	full_adder fa1(.x(x[1]), .y(y[1]), .cin(c0 ), .s(s[1]), .cout(c1));
//	full_adder fa2(.x(x[2]), .y(y[2]), .cin(c1 ), .s(s[2]), .cout(c2));
//	full_adder fa3(.x(x[3]), .y(y[3]), .cin(c2 ), .s(s[3]), .cout(COUT));
//	
//
//endmodule
//
//module full_adder(
//	input x, y, cin,
//	output logic s, cout
//	);
//	
//	assign s= x^y^cin;
//	assign cout=(x&y)||(y&cin)|(cin&x);
//	
//endmodule
