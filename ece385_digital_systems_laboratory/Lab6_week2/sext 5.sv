module sext5(input logic[4:0] A,
				  output logic[15:0] out);
	assign out={{11{A[4]}},A};
				  
endmodule
