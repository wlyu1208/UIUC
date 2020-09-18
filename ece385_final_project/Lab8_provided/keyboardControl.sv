//-------------------------------------------------------------------------
//    Ball.sv                                                            --
//    Viral Mehta                                                        --
//    Spring 2005                                                        --
//                                                                       --
//    Modified by Stephen Kempf 03-01-2006                               --
//                              03-12-2007                               --
//    Translated by Joe Meng    07-07-2013                               --
//    Modified by Po-Han Huang  12-08-2017                               --
//    Spring 2018 Distribution                                           --
//                                                                       --
//    For use with ECE 385 Lab 8                                         --
//    UIUC ECE Department                                                --
//-------------------------------------------------------------------------


module  keyboardControl ( input 	Clk,                // 50 MHz clock
											Reset,              // Active-high reset signal
               input [9:0]   		DrawX, DrawY, gridYGhost,      // Current pixel coordinates
					input [7:0]   		keycode,					 // Whether current pixel belongs to ball or background
					input 				canMoveRight, canMoveLeft, canMoveDown, newTetramino, canRotateCounterClockwise, canRotateClockwise, shiftUp1,
					input 				shiftLeft1, shiftLeft2, shiftRight1, shiftRight2, shiftUp2, canGhostMoveDown, signalStart,
					input [2:0]			randomNextShape,
					input [14:0]		totalLinesCleared, 
               output [9:0]  		tetraminoX, tetraminoY, ghostPosX, ghostPosY,
					output [1:0]  		state_out,
					output [2:0]		shape_out,
					output 				delaySigReset, hardDrop
              );
				  
	// 640x480 display resolution
    
    parameter [9:0] X_Step = 10'd16;      // Step size on the X axis
    parameter [9:0] Y_Step = 10'd16;      // Step size on the Y axis
    
	 
	 logic [9:0] ghostX = 10'd320;
	 logic [9:0] ghostX_in = 10'd0;
	 logic [9:0] ghostY = 10'd80;
	 logic [9:0] ghostY_in = 10'd0;
    logic [9:0] Ball_X_Pos = 10'd320;
	 logic [9:0] Ball_Y_Pos = 10'd64;
	 logic [9:0] Ball_X_Motion = 10'd0; 
	 logic [9:0] Ball_Y_Motion = 10'd0;  
    logic [9:0] Ball_X_Pos_in = 10'd0;  
	 logic [9:0] Ball_X_Motion_in = 10'd0;  
	 logic [9:0] Ball_Y_Pos_in = 10'd0;  
	 logic [9:0] Ball_Y_Motion_in = 10'd0;  
	 logic keyDownM = 1'd0;
	 logic keyDownM_in = 1'd0;
	 logic keyDownR = 1'd0;
	 logic keyDownR_in = 1'd0;
	 logic [1:0] state_in = 2'd0;
	 logic [1:0] state = 2'd0;
	 logic [2:0] shape_in = 3'd2;
	 logic [2:0] shape = 3'd2;
	 logic softDropDelay, ghostDelay;
	 logic dropClk;
	 logic move_down=1'b1;
	 logic isHardDrop = 0;
	 logic dropRateOffset = 30'd50000000;
	 
	 assign shape_out = shape;
	 assign state_out = state;
	 assign dropRateOffset = 30'd50000000 - totalLinesCleared*10000;

	 
	 softDrop softDrop0(.clk(Clk), .offset(30'd10000000), .delaySig(softDropDelay), .keyDownM);
	 softDrop dropRate(.clk(Clk), .offset(30'd50000000), .delaySig(dropClk), .keyDownM(1'b1));
	 softDrop ghostspeedreduction(.clk(Clk), .offset(30'd1000), .delaySig(ghostDelay), .keyDownM(1'b1));
	 
	 
	 
	 
    // Update registers
    always_ff @ (posedge Clk)
    begin


        if (Reset)
        begin
				ghostX <= 10'd320;
		      ghostY <= 10'd80;
            Ball_X_Pos <= 10'd320;
            Ball_Y_Pos <= 10'd64;
            Ball_X_Motion <= 10'd0;
            Ball_Y_Motion <= 10'd0;
				keyDownM <= 1'd0;
				keyDownR <= 1'd0;
				state <= 2'd0;
				shape <= randomNextShape;
        end
		else if(newTetramino || isHardDrop)
		begin
				ghostX <= 10'd320;
		      ghostY <= 10'd80;
				Ball_X_Pos <= 10'd320;
            Ball_Y_Pos <= 10'd64;
				Ball_X_Motion <= 10'd0;
				Ball_Y_Motion <= 10'd0;
				keyDownM <= 1'd0;
				keyDownR <= 1'd0;
				state <= 2'd0;
				shape <= randomNextShape;
		end
        else
        begin
				ghostX <= Ball_X_Pos;
		      ghostY <= ghostY_in;
            Ball_X_Pos <= Ball_X_Pos_in;
            Ball_Y_Pos <= Ball_Y_Pos_in;
				Ball_X_Motion <= Ball_X_Motion_in;
				Ball_Y_Motion <= Ball_Y_Motion_in;
				keyDownM <= keyDownM_in;
				keyDownR <= keyDownR_in;
				state <= state_in;
				shape <= shape_in;
        end
    end
    //////// Do not modify the always_ff blocks. ////////
	

	 
    // You need to modify always_comb block.
    always_comb
    begin
	 
			// By default, keep motion and position unchanged
			Ball_X_Pos_in = Ball_X_Pos;
			Ball_Y_Pos_in = Ball_Y_Pos;
			Ball_X_Motion_in = 10'd0;
			Ball_Y_Motion_in = 10'd0;
			
			state_in = state;
			shape_in = shape;
			keyDownM_in = keyDownM;
			keyDownR_in = keyDownR;
			delaySigReset = 0;
			move_down=1'b1;
			isHardDrop = 1'd0;
			
			if(gridYGhost == 10'hFF)
				ghostY_in = 10'd900;
			else if(canGhostMoveDown && ghostDelay) 
				ghostY_in = ghostY + Y_Step;
			else
				ghostY_in = ghostY;

			if(dropClk && canMoveDown && signalStart) 
			begin
			Ball_X_Motion_in=10'd0;
			Ball_Y_Motion_in=Y_Step;
			ghostY_in = Ball_Y_Pos;
			end
	 
        // Update position 
        if (keycode != 8'h0000 && signalStart)
        begin
				// TODO: Add other boundary detections and handle keypress here.
				delaySigReset = 1;
				

				if(keyDownM == 0)
				begin
				case(keycode)
//				8'h52: // up arrow
//					begin
//					Ball_X_Motion_in = 10'd0;
//					Ball_Y_Motion_in = (~(Y_Step) + 10'd1);
//					keyDownM_in = 1'd1;
//					end
				8'h50: // left arrow
					begin
					if(canMoveLeft)
					begin
						Ball_Y_Motion_in=10'd0;
						Ball_X_Motion_in=(~(X_Step) + 10'd1);
						keyDownM_in = 1'd1;
						if(Ball_Y_Pos < 10'd80)
							ghostY_in = 10'd80;
						else
							ghostY_in = Ball_Y_Pos;
					end
					end
				8'h51: // down arrow
					begin
					if(canMoveDown)
					begin
						Ball_X_Motion_in=10'd0;
						Ball_Y_Motion_in=Y_Step;
						keyDownM_in = 1'd1;
						if(Ball_Y_Pos < 10'd80)
							ghostY_in = 10'd80;
						else
							ghostY_in = Ball_Y_Pos;
					end
					end
				8'h4f: // right arrow
					begin
					if(canMoveRight)
					begin
						Ball_Y_Motion_in=10'd0;
						Ball_X_Motion_in=X_Step;
						keyDownM_in = 1'd1;
						if(Ball_Y_Pos < 10'd80)
							ghostY_in = 10'd80;
						else
							ghostY_in = Ball_Y_Pos;
					end
					end
				8'h2c: // space bar
					begin
						Ball_Y_Motion_in=10'd0;
						Ball_X_Motion_in=10'd0;
						keyDownM_in = 1'd1;
						isHardDrop = 1'd1;
					end
				endcase
				end
				
				if(keyDownR == 0)
				begin
				case(keycode)
				8'h52: // D key for clockwise rotation (D key = 8'd07)
					begin
					if(canRotateClockwise)
					begin
					
					if(shiftUp1)
						Ball_Y_Motion_in = (~(Y_Step) + 10'd1);
					else if(shiftUp2)
						Ball_Y_Motion_in = (~(Y_Step*2) + 10'd1);
					else
						Ball_Y_Motion_in=10'd0;

					if(shiftLeft1)
						Ball_X_Motion_in = (~(X_Step) + 10'd1);
					else if(shiftLeft2)
						Ball_X_Motion_in = (~(X_Step*2) + 10'd1);
					else if(shiftRight1)
						Ball_X_Motion_in = X_Step;
					else if(shiftRight2)
						Ball_X_Motion_in = X_Step*2;
					else
						Ball_X_Motion_in=10'd0;
						
					
					if(Ball_Y_Pos < 10'd80)
							ghostY_in = 10'd80;
						else
							ghostY_in = Ball_Y_Pos;
					
					keyDownR_in = 1'd1;
					state_in = state + 2'd1;
					end
					end
				8'h04: // A key for counter-clockwise rotation
					begin
					if(canRotateCounterClockwise)
					begin
					
					if(shiftUp1)
						Ball_Y_Motion_in = (~(Y_Step) + 10'd1);
					else if(shiftUp2)
						Ball_Y_Motion_in = (~(Y_Step*2) + 10'd1);
					else
						Ball_Y_Motion_in=10'd0;
					
					if(shiftLeft1)
						Ball_X_Motion_in = (~(X_Step) + 10'd1);
					else if(shiftLeft2)
						Ball_X_Motion_in = (~(X_Step*2) + 10'd1);
					else if(shiftRight1)
						Ball_X_Motion_in = X_Step;
					else if(shiftRight2)
						Ball_X_Motion_in = X_Step*2;
					else
						Ball_X_Motion_in=10'd0;
						
					if(Ball_Y_Pos < 10'd80)
							ghostY_in = 10'd80;
						else
							ghostY_in = Ball_Y_Pos;
						
					keyDownR_in = 1'd1;
					state_in = state - 2'd1;
					end
					end
				endcase
				end					
        end
		  else
		  begin
			delaySigReset = 0;
		  end
		  
		  if(keycode == 8'h0000 && keyDownM == 1'd1 || softDropDelay && keyDownM == 1'd1)
		  begin
				keyDownM_in = 1'd0;
		  end
		  if(keycode == 8'h0000 && keyDownR == 1'd1)
		  begin
				keyDownR_in = 1'd0;
		  end
		  
		  
		  // Update the ball's position with its motion
        Ball_X_Pos_in = Ball_X_Pos + Ball_X_Motion;
		  if(isHardDrop == 0)
				Ball_Y_Pos_in = Ball_Y_Pos + Ball_Y_Motion;
		  else
				Ball_Y_Pos_in = ghostY;
        

    end
    
    // Compute whether the pixel corresponds to ball or background
    /* Since the multiplicants are required to be signed, we have to first cast them
       from logic to int (signed by default) before they are multiplied. */

    assign tetraminoX = Ball_X_Pos;
    assign tetraminoY = Ball_Y_Pos;
	 assign ghostPosX = ghostX;
	 assign ghostPosY = ghostY;
   
    
endmodule

module softDrop(input clk, input [29:0] offset, input keyDownM, output delaySig);

logic [29:0] count = 0;

always_ff @ (posedge (clk & keyDownM))
begin
	if(count == offset || keyDownM == 0)
		count <= 0;
	else
		count <= count+1;
end

always_comb
begin
	if(count == offset/2)
		delaySig = 1;
	else
		delaySig = 0;
end


endmodule
