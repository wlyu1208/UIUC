module sext8(input logic[8:0] A,
				  output logic[15:0] out);
	assign out={{7{A[8]}},A};
				  
endmodule
