module two_mux(input logic selec,
					input logic[15:0] s0, s1,
					output logic[15:0] out);
always_comb
begin
	if(selec==1'b0)
		out=s0;
	else
		out=s1;
end
endmodule
