//module carry_lookahead_adder
//(
//    input   logic[15:0]     A,
//    input   logic[15:0]     B,
//    output  logic[15:0]     Sum,
//    output  logic           CO
//);
//
//    /* TODO
//     *
//     * Insert code here to implement a CLA adder.
//     * Your code should be completly combinational (don't use always_ff or always_latch).
//     * Feel free to create sub-modules or other files. */
//     logic C0, C1, C2;
//	  logic PG0, PG1, PG2, PG3, GG0, GG1, GG2, GG3;
//	  
//	  four_bit_cla Cla0( .x(A[3:0]), .y(B[3:0]), .cin(1'b0), .s(Sum[3:0]), .PG(PG0), .GG(GG0), .COUT(C0));
//	  four_bit_cla Cla1( .x(A[7:4]), .y(B[7:4]), .cin(C0), .s(Sum[7:4]), .PG(PG1), .GG(GG1), .COUT(C1));
//	  four_bit_cla Cla2( .x(A[11:8]), .y(B[11:8]), .cin(C1), .s(Sum[11:8]), .PG(PG2), .GG(GG2), .COUT(C2));
//	  four_bit_cla Cla3( .x(A[15:12]), .y(B[15:12]), .cin(C2), .s(Sum[15:12]), .PG(PG3), .GG(GG3), .COUT(CO));
//endmodule
//
//module four_bit_cla(
//	input [3:0] x, 
//	input [3:0] y,
//	input cin,
//	output logic [3:0] s,
//	output logic COUT, PG, GG
//
//);
//logic c0, c1, c2;
//logic p0, p1, p2, p3;
//logic g0, g1, g2, g3;
//
//CLA_adder cla0( .x(x[0]), .y(y[0]), .cin(cin), .s(s[0]), .g(g0), .p(p0), .cout(c0));
//CLA_adder cla1( .x(x[1]), .y(y[1]), .cin(c0), .s(s[1]), .g(g1), .p(p1), .cout(c1));
//CLA_adder cla2( .x(x[2]), .y(y[2]), .cin(c1), .s(s[2]), .g(g2), .p(p2), .cout(c2));
//CLA_adder cla3( .x(x[3]), .y(y[3]), .cin(c2), .s(s[3]), .g(g3), .p(p3), .cout(COUT));
//
//assign PG=p0+p1+p2+p3;
//assign GG=g3|(g2&p3)|(g1&p3&p2)|(g0&p3&p2&p1);
//endmodule
//
//module CLA_adder(
//	input x, y, z, cin,
//	output logic s, cout, p, g
//	);
//	
//	assign s= x^y^cin;
//	assign p=x^y;
//	assign g=x&y;
//	assign cout=(g|(p&cin));
//	
//endmodule
