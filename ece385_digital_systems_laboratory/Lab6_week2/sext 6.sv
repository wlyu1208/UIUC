module sext6(input logic[5:0] A,
				  output logic[15:0] out);
	assign out={{10{A[5]}},A};
				  
endmodule
