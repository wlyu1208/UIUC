module control (input logic Clk, Reset, Run, ClearA_LoadB, m,
					 output logic fin, Clr_Ld, Shift, Add, Sub);
					 //inputs and outputs
					 
		enum logic [4:0] {A, AB, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R} curr_state, next_state;
		//A=start
		
		//updates flip_flop
		always_ff @ (posedge Reset or posedge Clk)
		begin
			if (Reset)
					curr_state <= A;
			else
					curr_state <= next_state;
					
		end
		
		//Assign outputs based on state
		
		always_comb
		begin
		
			next_state=curr_state;
			unique case(curr_state)
			
				A : if(Run)
					 next_state=B;
				B : next_state=C;
				C : next_state=D;
				D : next_state=E;
				E : next_state=F;
				F : next_state=G;
				G : next_state=H;
				H : next_state=I;
				I : next_state=J;
				J : next_state=K;
				K : next_state=L;
				L : next_state=M;
				M : next_state=N;
				N : next_state=O;
				O : next_state=P;
				P : next_state=Q;
				Q : next_state=R;
				R : if(~Run)
					 next_state=A;
			endcase
		
		case(curr_state)
			A: //start state
				begin 
				Clr_Ld=ClearA_LoadB;
				Shift=1'b0;
				Add=1'b0;
				Sub=1'b0;
				fin=1'b0;
				end
			B: //1st add
				begin
				Clr_Ld=1'b0;
				Shift=1'b0;
				Add=m;
				Sub=1'b0;
				fin=1'b0;
				end
			
			C: //1st shift
				begin
				Clr_Ld=1'b0;
				Shift=1'b1;
				Add=1'b0;
				Sub=1'b0;
				fin=1'b0;
				end
			
			D://2nd add
				begin
				Clr_Ld=1'b0;
				Shift=1'b0;
				Add=m;
				Sub=1'b0;
				fin=1'b0;
				end
			
			E: //2nd shift
				begin
				Clr_Ld=1'b0;
				Shift=1'b1;
				Add=1'b0;
				Sub=1'b0;
				fin=1'b0;
				end
			
			F: //3rd add
				begin
				Clr_Ld=1'b0;
				Shift= 1'b0;
				Add=m;
				Sub=1'b0;
				fin=1'b0;
				end
			
			G: //3rd shift
				begin
				Clr_Ld=1'b0;
				Shift=1'b1;
				Add=1'b0;
				Sub=1'b0;
				fin=1'b0;
				end
				
			H: //4th add
				begin
				Clr_Ld=1'b0;
				Shift= 1'b0;
				Add=m;
				Sub=1'b0;
				fin=1'b0;
				end
				
			I: //4th shift
				begin 
				Clr_Ld=1'b0;
				Shift=1'b1;
				Add=1'b0;
				Sub=1'b0;
				fin=1'b0;
				end
				
			J: //5th add
				begin
				Clr_Ld=1'b0;
				Shift= 1'b0;
				Add=m;
				Sub=1'b0;
				fin=1'b0;
				end
			
			K: //5th shift
				begin 
				Clr_Ld=1'b0;
				Shift=1'b1;
				Add=1'b0;
				Sub=1'b0;
				fin=1'b0;
				end
			
			L: //6th add
				begin
				Clr_Ld=1'b0;
				Shift= 1'b0;
				Add=m;
				Sub=1'b0;
				fin=1'b0;
				end
			
			M: //6th shift
				begin 
				Clr_Ld=1'b0;
				Shift=1'b1;
				Add=1'b0;
				Sub=1'b0;
				fin=1'b0;
				end
			
			N: //7th add
				begin
				Clr_Ld=1'b0;
				Shift= 1'b0;
				Add=m;
				Sub=1'b0;
				fin=1'b0;
				end
				
			O: //7th shift
				begin 
				Clr_Ld=1'b0;
				Shift=1'b1;
				Add=1'b0;
				Sub=1'b0;
				fin=1'b0;
				end
			
			P: //8th sub
				begin
				Clr_Ld=1'b0;
				Shift=1'b0;
				Add=1'b0;
				Sub=m;
				fin=1'b0;
				end
				
			Q: //8th shift
				begin
				Clr_Ld=1'b0;
				Shift=1'b1;
				Add=1'b0;
				Sub=1'b0;
				fin=1'b0;
				end
				
			R: //done
				begin
				Clr_Ld=1'b0;
				Shift=1'b0;
				Add=1'b0;
				Sub=1'b0;
				fin=1'b0;
				end
		endcase
	end
	
endmodule
