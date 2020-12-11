module four_mux(input logic[15:0] s0, s1, s2, s3,
					 input logic[1:0] selec,
					 output logic[15:0] out
);
always_comb
begin
	if(selec==2'b00)
		out=s0;
	else if(selec==2'b01)
		out=s1;
	else if(selec==2'b10)
		out=s2;
	else
		out=s3;
end
endmodule
