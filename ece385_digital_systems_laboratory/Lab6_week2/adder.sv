module sixteen_adder(input logic[15:0] A, B,
							output logic [15:0] S);
logic c0, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14;

adder a0  (.x(A[0]),  .y(B[0]),  .z(0 ),  .cout(c0),  .s(S[0]));
adder a1  (.x(A[1]),  .y(B[1]),  .z(c0),  .cout(c1),  .s(S[1]));
adder a2  (.x(A[2]),  .y(B[2]),  .z(c1),  .cout(c2),  .s(S[2]));
adder a3  (.x(A[3]),  .y(B[3]),  .z(c2),  .cout(c3),  .s(S[3]));
adder a4  (.x(A[4]),  .y(B[4]),  .z(c3),  .cout(c4),  .s(S[4]));
adder a5  (.x(A[5]),  .y(B[5]),  .z(c4),  .cout(c5),  .s(S[5]));
adder a6  (.x(A[6]),  .y(B[6]),  .z(c5),  .cout(c6),  .s(S[6]));
adder a7  (.x(A[7]),  .y(B[7]),  .z(c6),  .cout(c7),  .s(S[7]));
adder a8  (.x(A[8]),  .y(B[8]),  .z(c7),  .cout(c8),  .s(S[8]));
adder a9  (.x(A[9]),  .y(B[9]),  .z(c8),  .cout(c9),  .s(S[9]));
adder a10 (.x(A[10]), .y(B[10]), .z(c9),  .cout(c10), .s(S[10]));
adder a11 (.x(A[11]), .y(B[11]), .z(c10), .cout(c11), .s(S[11]));
adder a12 (.x(A[12]), .y(B[12]), .z(c11), .cout(c12), .s(S[12]));
adder a13 (.x(A[13]), .y(B[13]), .z(c12), .cout(c13), .s(S[13]));
adder a14 (.x(A[14]), .y(B[14]), .z(c13), .cout(c14), .s(S[14]));
adder a15 (.x(A[15]), .y(B[15]), .z(c14), .cout(), .s(S[15]));

//assign S=A+B;
							
endmodule

module adder(input logic x, y, z,
				 output logic s, cout);
assign s=x^y^z;
assign cout=(x&y)|(x&z)|(y&z);
				 
endmodule
