module nzp_logic(input logic[15:0] bus,
					  output logic n, z, p);

always_comb
begin
	if(bus==16'h0000)
		begin
		n=1'b0;
		z=1'b1;
		p=1'b0;
		end
	else if(bus[15])
		begin
		n=1'b1;
		z=1'b0;
		p=1'b0;
		end
	else
		begin
		n=1'b0;
		z=1'b0;
		p=1'b1;
		end
end
endmodule
