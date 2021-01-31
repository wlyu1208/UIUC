module flip_flop(input logic Clk, LD, Reset, 
			  input logic [15:0] D,
			  output logic [15:0] Q);
			  
always_ff @ (posedge Clk or posedge Reset)
begin
	if(Reset)
		Q<=16'h0000;
	else 
		if(LD)
			Q<=D;
end
endmodule

module ff_3bit(input logic Clk, LD, Reset, 
			  input logic n, z, p,
			  output logic n_f, z_f, p_f);
			  
always_ff @ (posedge Clk or posedge Reset)
begin
	if(Reset)
		begin
		n_f<=1'b0;
		z_f<=1'b0;
		p_f<=1'b0;
		end
	else 
		if(LD)
			begin
			n_f<=n;
			z_f<=z;
			p_f<=p;
			end
end
endmodule

module LED_12(input logic Clk, LD, Reset, 
			  input logic [11:0] D,
			  output logic [11:0] Q);
			  
always_ff @ (posedge Clk or posedge Reset)
begin
	if(Reset)
		Q<=12'h000;
	else 
		if(LD)
			Q<=D;
end
endmodule

module ben_ff(input logic Clk, Reset, N, Z, P, LD, 
				  input logic[2:0] ir,
				  output logic ben);
				  
always_ff @ (posedge Clk or posedge Reset)
begin
	if(Reset)
		ben<=1'b0;
	else 
		if(LD)
			ben<=(ir[2]&N)+(ir[1]&Z)+(ir[0]&P);
end
				  
endmodule
