module sext11(input logic[10:0] A,
				  output logic[15:0] out);
	assign out={{5{A[10]}},A};
				  
endmodule