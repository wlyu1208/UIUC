module ALU(input logic [15:0] A, B,
			  input logic [1:0] Sel,
			  output logic [15:0] out);
	always_comb
	begin
		if(Sel==2'b00)
			out=A+B;
		else if(Sel==2'b01)
			out=A&B;
		else if(Sel==2'b10)
			out=~A;
		else
			out=A;
	end
			  
endmodule
