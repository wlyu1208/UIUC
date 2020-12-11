module flipflop(input logic Clk, Load, Reset, D1, D2,
					 output logic Q);

always_ff @ (posedge Reset or posedge Clk)
begin
	if(Reset)
		Q<=1'b0;
	else
		if(~Load)
			Q<=D1;
		else
			Q<=D2;
end
endmodule
