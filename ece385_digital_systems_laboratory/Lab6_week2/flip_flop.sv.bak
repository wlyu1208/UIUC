module flip_flop(input logic Clk, LD, Reset, 
			  input logic [15:0] D,
			  output logic [15:0] Q);

logic[15:0] data;	  
			  
always_ff @ (posedge Clk or posedge Reset)
begin
	if(Reset)
		Q<=16'h0000;
	else 
		if(LD)
			Q<=D;
end
endmodule
