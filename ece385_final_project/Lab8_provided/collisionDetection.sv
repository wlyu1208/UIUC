

module collisionDetection( input clk, signalStart,
									input [9:0]		blockSize,
									input [9:0] minBlockPixX,
									input [9:0] maxBlockPixX,
									input [9:0] maxBlockPixY,
									input [9:0] currX, 
									input [9:0] currY, 
									input [2:0] controlledShape,
									input [1:0] controlledState,
									
									input [9:0][21:0][2:0] grid ,
									input [9:0] gridX,
									input [9:0] gridY,
									
									output canMoveRight,
									output canMoveLeft,
									output canMoveDown,
									output canRotateClockwise,
									output canRotateCounterClockwise,
									output shiftUp1, shiftUp2,
									output shiftLeft1,
									output shiftLeft2,
									output shiftRight1,
									output shiftRight2);

									
									
									
logic canMoveRight_in = 1'd1;
logic canMoveLeft_in = 1'd1;
logic canMoveDown_in = 1'd1;
logic canRotateClockwise_in = 1'd1;
logic canRotateCounterClockwise_in = 1'd1;
logic shiftUp1_in = 1'd0;
logic shiftUp2_in = 1'd0;
logic shiftLeft1_in = 1'd0;
logic shiftLeft2_in = 1'd0;
logic shiftRight1_in = 1'd0;
logic shiftRight2_in = 1'd0;


		
always_ff @ (posedge clk)
begin
	canMoveRight <= canMoveRight_in;
   canMoveLeft <= canMoveLeft_in;
	canMoveDown <= canMoveDown_in;
	canRotateCounterClockwise <= canRotateCounterClockwise_in;
	canRotateClockwise <= canRotateClockwise_in;
	shiftUp1 <= shiftUp1_in;
	shiftUp2 <= shiftUp2_in;
	shiftLeft1 <= shiftLeft1_in;
	shiftLeft2 <= shiftLeft2_in;
	shiftRight1 <= shiftRight1_in;
	shiftRight2 <= shiftRight2_in;
end

	
always_comb
begin
if(gridY == 10'hFF)
begin

	if(currX > minBlockPixX)
		canMoveLeft_in = 1'd1;
	else
		canMoveLeft_in = 1'd0;
		
	if(currX < maxBlockPixX)
		canMoveRight_in = 1'd1;
	else
		canMoveRight_in = 1'd0;
	
	if(currY < maxBlockPixY && signalStart)
		canMoveDown_in = 1'd1;
	else
		canMoveDown_in = 1'd0;
	
	
	canRotateCounterClockwise_in = 1'd1;
	canRotateClockwise_in = 1'd1;
	shiftUp1_in = 1'd0;
	shiftUp2_in = 1'd0;
	shiftLeft1_in = 1'd0;
	shiftLeft2_in = 1'd0;
	shiftRight1_in = 1'd0;
	shiftRight2_in = 1'd0;



	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	





end
else
begin	

	case(controlledShape)    // pieces spawn at (320, 112)
		3'd1: // I piece
			case(controlledState)
				
				2'd0:
					begin
						if(currX + blockSize*2 < maxBlockPixX && grid[gridX+2][gridY] == 3'd0) 
							canMoveRight_in = 1'd1;
						else
							canMoveRight_in = 1'd0;
							
						if(currX - blockSize*2 > minBlockPixX && grid[gridX-3][gridY] == 3'd0)
							canMoveLeft_in = 1'd1;
						else
							canMoveLeft_in = 1'd0;
							
						if(currY + blockSize < maxBlockPixY && grid[gridX][gridY+1] == 3'd0 && grid[gridX+1][gridY+1] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0 && grid[gridX-2][gridY+1] == 3'd0)
							canMoveDown_in = 1'd1;
						else
							canMoveDown_in = 1'd0;
							

														
						// rotation checks	
						shiftLeft1_in = 1'd0;
						shiftLeft2_in = 1'd0;
						shiftRight1_in = 1'd0;
						shiftRight2_in = 1'd0;
						shiftUp2_in = 1'd0;
						
					
						
						case(gridY)
							
							10'd19:
							begin
								shiftUp1_in = 1'd1;
								
								if(grid[gridX-1][gridY-1] == 3'd0 && grid[gridX-1][gridY-2] == 3'd0 && grid[gridX-1][gridY-3] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									
									if(grid[gridX][gridY-1] == 3'd0 && grid[gridX][gridY-2] == 3'd0 && grid[gridX][gridY-3] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
									
									if(grid[gridX][gridY-1] == 3'd0 && grid[gridX][gridY-2] == 3'd0 && grid[gridX][gridY-3] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
							end
							
							default:
							begin
								shiftUp1_in = 1'd0;
								
								if(grid[gridX-1][gridY+1] == 3'd0 && grid[gridX-1][gridY-1] == 3'd0 && grid[gridX-1][gridY-2] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									
									if(grid[gridX][gridY-1] == 3'd0 && grid[gridX][gridY-2] == 3'd0 && grid[gridX][gridY+1] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
									
									if(grid[gridX][gridY-1] == 3'd0 && grid[gridX][gridY-2] == 3'd0 && grid[gridX][gridY+1] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
							
							end
						
						endcase			
	
					end
				
				2'd1:
					begin

					
						if(currX < maxBlockPixX && grid[gridX][gridY] == 3'd0 && grid[gridX][gridY+1] == 3'd0 && grid[gridX][gridY-1] == 3'd0 && grid[gridX][gridY-2] == 3'd0)
							canMoveRight_in = 1'd1;
						else
							canMoveRight_in = 1'd0;
							
						if(currX - blockSize > minBlockPixX && grid[gridX-2][gridY] == 3'd0 && grid[gridX-2][gridY+1] == 3'd0 && grid[gridX-2][gridY-1] == 3'd0 && grid[gridX-2][gridY-2] == 3'd0)
							canMoveLeft_in = 1'd1;
						else
							canMoveLeft_in = 1'd0;
							
						if(currY + blockSize*2 < maxBlockPixY && grid[gridX-1][gridY+2] == 3'd0)
							canMoveDown_in = 1'd1;
						else
							canMoveDown_in = 1'd0;
							
						

						// rotation checks
						shiftUp1_in = 1'd0;
						shiftUp2_in = 1'd0;
					
						case(gridX)
						
							10'd10: //right edge
							begin
							
								if(grid[gridX-4][gridY-1] == 3'd0 && grid[gridX-3][gridY-1] == 3'd0 && grid[gridX-2][gridY-1] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									shiftLeft1_in = 1'd0;
									shiftLeft2_in = 1'd1;
									shiftRight1_in = 1'd0;
									shiftRight2_in = 1'd0;
									
									if(grid[gridX-4][gridY] == 3'd0 && grid[gridX-3][gridY] == 3'd0 && grid[gridX-2][gridY] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
								
									if(grid[gridX-4][gridY] == 3'd0 && grid[gridX-3][gridY] == 3'd0 && grid[gridX-2][gridY] == 3'd0)
									begin
										canRotateCounterClockwise_in = 1'd1;
										shiftLeft1_in = 1'd0;
										shiftLeft2_in = 1'd1;
										shiftRight1_in = 1'd0;
										shiftRight2_in = 1'd0;
									end
									else
									begin
										canRotateClockwise_in = 1'd0;
										canRotateCounterClockwise_in = 1'd0;
										shiftLeft1_in = 1'd0;
										shiftLeft2_in = 1'd0;
										shiftRight1_in = 1'd0;
										shiftRight2_in = 1'd0;
									end
								end
							end
							10'd9:
							begin
								if(grid[gridX-3][gridY-1] == 3'd0 && grid[gridX-2][gridY-1] == 3'd0 && grid[gridX][gridY-1] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									shiftLeft1_in = 1'd1;
									shiftLeft2_in = 1'd0;
									shiftRight1_in = 1'd0;
									shiftRight2_in = 1'd0;
									
									if(grid[gridX-3][gridY] == 3'd0 && grid[gridX-2][gridY] == 3'd0 && grid[gridX][gridY] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
								
									if(grid[gridX-3][gridY] == 3'd0 && grid[gridX-2][gridY] == 3'd0 && grid[gridX][gridY] == 3'd0)
									begin
										canRotateCounterClockwise_in = 1'd1;
										shiftLeft1_in = 1'd1;
										shiftLeft2_in = 1'd0;
										shiftRight1_in = 1'd0;
										shiftRight2_in = 1'd0;
									end
									else
									begin
										canRotateClockwise_in = 1'd0;
										canRotateCounterClockwise_in = 1'd0;
										shiftLeft1_in = 1'd0;
										shiftLeft2_in = 1'd0;
										shiftRight1_in = 1'd0;
										shiftRight2_in = 1'd0;
									end
								end
							end
							10'd1:
							begin
								if(grid[gridX][gridY-1] == 3'd0 && grid[gridX+1][gridY-1] == 3'd0 && grid[gridX+2][gridY-1] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									shiftLeft1_in = 1'd0;
									shiftLeft2_in = 1'd0;
									shiftRight1_in = 1'd1;
									shiftRight2_in = 1'd0;
									
									if(grid[gridX][gridY] == 3'd0 && grid[gridX+1][gridY] == 3'd0 && grid[gridX+2][gridY] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
								
									if(grid[gridX][gridY] == 3'd0 && grid[gridX+1][gridY] == 3'd0 && grid[gridX+2][gridY] == 3'd0)
									begin
										canRotateCounterClockwise_in = 1'd1;
										shiftLeft1_in = 1'd0;
										shiftLeft2_in = 1'd0;
										shiftRight1_in = 1'd1;
										shiftRight2_in = 1'd0;
									end
									else
									begin
										canRotateClockwise_in = 1'd0;
										canRotateCounterClockwise_in = 1'd0;
										shiftLeft1_in = 1'd0;
										shiftLeft2_in = 1'd0;
										shiftRight1_in = 1'd0;
										shiftRight2_in = 1'd0;
									end
								end
							end
							default:
							begin
								shiftLeft1_in = 1'd0;
								shiftLeft2_in = 1'd0;
								shiftRight1_in = 1'd0;
								shiftRight2_in = 1'd0;
								
								
								
								if(grid[gridX-2][gridY-1] == 3'd0 && grid[gridX+1][gridY-1] == 3'd0 && grid[gridX][gridY-1] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									
									if(grid[gridX-2][gridY] == 3'd0 && grid[gridX+1][gridY] == 3'd0 && grid[gridX][gridY] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
									
									if(grid[gridX-2][gridY] == 3'd0 && grid[gridX+1][gridY] == 3'd0 && grid[gridX][gridY] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
								
								
								
							end
						endcase
	
							
					end
				
				2'd2:
					begin
					
					
						if(currX + blockSize*2 < maxBlockPixX && grid[gridX+2][gridY-1] == 3'd0)
							canMoveRight_in = 1'd1;
						else
							canMoveRight_in = 1'd0;
							
						if(currX - blockSize*2 > minBlockPixX && grid[gridX-3][gridY-1] == 3'd0)
							canMoveLeft_in = 1'd1;
						else
							canMoveLeft_in = 1'd0;
							
						if(currY < maxBlockPixY && grid[gridX][gridY] == 3'd0 && grid[gridX+1][gridY] == 3'd0
								&& grid[gridX-1][gridY] == 3'd0 && grid[gridX-2][gridY] == 3'd0)
							canMoveDown_in = 1'd1;
						else
							canMoveDown_in = 1'd0;
							
	
						// rotation checks
						shiftLeft1_in = 1'd0;
						shiftLeft2_in = 1'd0;
						shiftRight1_in = 1'd0;
						shiftRight2_in = 1'd0;
						
							
						case(gridY)

						
							10'd20:
							begin
								shiftUp2_in = 1'd1;
								shiftUp1_in = 1'd0;
								
								if(grid[gridX][gridY-2] == 3'd0 && grid[gridX][gridY-3] == 3'd0 && grid[gridX][gridY-4] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									
									if(grid[gridX-1][gridY-2] == 3'd0 && grid[gridX-1][gridY-3] == 3'd0 && grid[gridX-1][gridY-4] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
									
									if(grid[gridX-1][gridY-2] == 3'd0 && grid[gridX-1][gridY-3] == 3'd0 && grid[gridX-1][gridY-4] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
							end
							
							10'd19:
							begin
								shiftUp2_in = 1'd0;
								shiftUp1_in = 1'd1;
								
								if(grid[gridX][gridY-2] == 3'd0 && grid[gridX][gridY-3] == 3'd0 && grid[gridX][gridY] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									
									if(grid[gridX-1][gridY-2] == 3'd0 && grid[gridX-1][gridY-3] == 3'd0 && grid[gridX-1][gridY] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
									
									if(grid[gridX-1][gridY-2] == 3'd0 && grid[gridX-1][gridY-3] == 3'd0 && grid[gridX-1][gridY] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
							end
							
							default:
							begin
								shiftUp2_in = 1'd0;
								shiftUp1_in = 1'd0;
								
								
								if(grid[gridX][gridY] == 3'd0 && grid[gridX][gridY+1] == 3'd0 && grid[gridX][gridY-2] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									
									if(grid[gridX-1][gridY-2] == 3'd0 && grid[gridX-1][gridY] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
									
									if(grid[gridX-1][gridY-2] == 3'd0 && grid[gridX-1][gridY] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
								
							end
						
						endcase
							
					end
				2'd3:
					begin
					
						if(currX + blockSize < maxBlockPixX && grid[gridX+1][gridY] == 3'd0 && grid[gridX+1][gridY+1] == 3'd0 && grid[gridX+1][gridY-1] == 3'd0 && grid[gridX+1][gridY-2] == 3'd0)
							canMoveRight_in = 1'd1;
						else
							canMoveRight_in = 1'd0;
						if(currX > minBlockPixX && grid[gridX-1][gridY] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0 && grid[gridX-2][gridY-1] == 3'd0 && grid[gridX-2][gridY-2] == 3'd0)
							canMoveLeft_in = 1'd1;
						else
							canMoveLeft_in = 1'd0;
						if(currY + blockSize*2 < maxBlockPixY && grid[gridX][gridY+2] == 3'd0)
							canMoveDown_in = 1'd1;
						else
							canMoveDown_in = 1'd0;
							

					
						shiftUp2_in = 1'd0;
						shiftUp1_in = 1'd0;
						shiftLeft2_in = 1'd0;
						
						
						
						case(gridX)
						
							10'd0:
								begin
									shiftRight2_in = 1'd1;
									shiftLeft1_in = 1'd0;
									shiftRight1_in = 1'd0;
								
									if(grid[gridX+1][gridY] == 3'd0 && grid[gridX+2][gridY] == 3'd0 && grid[gridX+3][gridY] == 3'd0)
									begin
										canRotateClockwise_in = 1'd1;
										
										if(grid[gridX+1][gridY-1] == 3'd0 && grid[gridX+2][gridY-1] == 3'd0 && grid[gridX+3][gridY-1] == 3'd0)
											canRotateCounterClockwise_in = 1'd1;
										else
											canRotateCounterClockwise_in = 1'd0;
									end
									else
									begin
										canRotateClockwise_in = 1'd0;
									
										if(grid[gridX+1][gridY-1] == 3'd0 && grid[gridX+2][gridY-1] == 3'd0 && grid[gridX+3][gridY-1] == 3'd0)
											canRotateCounterClockwise_in = 1'd1;
										else
											canRotateCounterClockwise_in = 1'd0;
									end
								end
								
							10'd1:
								begin
									shiftRight1_in = 1'd1;
									shiftLeft1_in = 1'd0;
									shiftRight2_in = 1'd0;
								
									if(grid[gridX+1][gridY] == 3'd0 && grid[gridX+2][gridY] == 3'd0 && grid[gridX+3][gridY] == 3'd0)
									begin
										canRotateClockwise_in = 1'd1;
										
										if(grid[gridX+1][gridY-1] == 3'd0 && grid[gridX+2][gridY-1] == 3'd0 && grid[gridX+3][gridY-1] == 3'd0)
											canRotateCounterClockwise_in = 1'd1;
										else
											canRotateCounterClockwise_in = 1'd0;
									end
									else
									begin
										canRotateClockwise_in = 1'd0;
									
										if(grid[gridX+1][gridY-1] == 3'd0 && grid[gridX+2][gridY-1] == 3'd0 && grid[gridX+3][gridY-1] == 3'd0)
											canRotateCounterClockwise_in = 1'd1;
										else
											canRotateCounterClockwise_in = 1'd0;
									end
								end
								
							10'd9:
								begin
									shiftRight1_in = 1'd0;
									shiftLeft1_in = 1'd1;
									shiftRight2_in = 1'd0;
								
									if(grid[gridX-1][gridY] == 3'd0 && grid[gridX-2][gridY] == 3'd0 && grid[gridX-3][gridY] == 3'd0)
									begin
										canRotateClockwise_in = 1'd1;
										
										if(grid[gridX-1][gridY-1] == 3'd0 && grid[gridX-2][gridY-1] == 3'd0 && grid[gridX-3][gridY-1] == 3'd0)
											canRotateCounterClockwise_in = 1'd1;
										else
											canRotateCounterClockwise_in = 1'd0;
									end
									else
									begin
										canRotateClockwise_in = 1'd0;
									
										if(grid[gridX-1][gridY-1] == 3'd0 && grid[gridX-2][gridY-1] == 3'd0 && grid[gridX-3][gridY-1] == 3'd0)
											canRotateCounterClockwise_in = 1'd1;
										else
											canRotateCounterClockwise_in = 1'd0;
									end
								end
								
							default:
								begin
									shiftLeft1_in = 1'd0;
									shiftRight1_in = 1'd0;
									shiftRight2_in = 1'd0;
									

									if(grid[gridX+1][gridY] == 3'd0 && grid[gridX-1][gridY] == 3'd0 && grid[gridX-2][gridY] == 3'd0)
									begin
										canRotateClockwise_in = 1'd1;
										
										if(grid[gridX+1][gridY-1] == 3'd0 && grid[gridX-1][gridY-1] == 3'd0 && grid[gridX-2][gridY-1] == 3'd0)
											canRotateCounterClockwise_in = 1'd1;
										else
											canRotateCounterClockwise_in = 1'd0;
									end
									else
									begin
										canRotateClockwise_in = 1'd0;
									
										if(grid[gridX+1][gridY-1] == 3'd0 && grid[gridX-1][gridY-1] == 3'd0 && grid[gridX-2][gridY-1] == 3'd0)
											canRotateCounterClockwise_in = 1'd1;
										else
											canRotateCounterClockwise_in = 1'd0;
									end	
									
								end
						
						endcase	
						
					end
			endcase
		3'd2: // L piece
			case(controlledState)
				2'd0:
					begin

					
						if(currX + blockSize*2 < maxBlockPixX && grid[gridX+2][gridY] == 3'd0 && grid[gridX+2][gridY-1] == 3'd0)
							canMoveRight_in = 1'd1;
						else
							canMoveRight_in = 1'd0;
							
						if(currX - blockSize > minBlockPixX && grid[gridX-2][gridY] == 3'd0 && grid[gridX][gridY-1] == 3'd0)
							canMoveLeft_in = 1'd1;
						else
							canMoveLeft_in = 1'd0;
							
						if(currY + blockSize < maxBlockPixY && grid[gridX][gridY+1] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0 && grid[gridX+1][gridY+1] == 3'd0)
							canMoveDown_in = 1'd1;
						else
							canMoveDown_in = 1'd0;
							
							
						// rotation checks
						shiftUp2_in = 1'd0;
						shiftLeft1_in = 1'd0;
						shiftLeft2_in = 1'd0;
						shiftRight1_in = 1'd0;
						shiftRight2_in = 1'd0;
						
						
						case(gridY)
						
						
							10'd19:
							begin
								shiftUp1_in = 1'd1;
								
								if(grid[gridX][gridY] == 3'd0 && grid[gridX][gridY-1] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									
									if(grid[gridX][gridY] == 3'd0 && grid[gridX][gridY-1] == 3'd0 && grid[gridX-1][gridY-1] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
									
									if(grid[gridX][gridY] == 3'd0 && grid[gridX][gridY-1] == 3'd0 && grid[gridX-1][gridY-1] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
							end

							default:
							begin
								shiftUp1_in = 1'd0;
								
								if(grid[gridX][gridY+1] == 3'd0 && grid[gridX+1][gridY+1] == 3'd0 && grid[gridX][gridY-1] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									
									if(grid[gridX][gridY+1] == 3'd0 && grid[gridX][gridY-1] == 3'd0 && grid[gridX-1][gridY-1] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
									
									if(grid[gridX][gridY+1] == 3'd0 && grid[gridX][gridY-1] == 3'd0 && grid[gridX-1][gridY-1] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
									
							end
						
						endcase
							
							
							
							
							
					end
				2'd1:
					begin

					
						if(currX + blockSize*2 < maxBlockPixX && grid[gridX+1][gridY] == 3'd0 && grid[gridX+2][gridY+1] == 3'd0 && grid[gridX+1][gridY-1] == 3'd0)
							canMoveRight_in = 1'd1;
						else
							canMoveRight_in = 1'd0;
							
						if(currX > minBlockPixX && grid[gridX-1][gridY] == 3'd0 && grid[gridX-1][gridY-1] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0)
							canMoveLeft_in = 1'd1;
						else
							canMoveLeft_in = 1'd0;
							
						if(currY + blockSize*2 < maxBlockPixY && grid[gridX][gridY+2] == 3'd0 && grid[gridX+1][gridY+2] == 3'd0)
							canMoveDown_in = 1'd1;
						else
							canMoveDown_in = 1'd0;
							
							
						shiftUp2_in = 1'd0;
						shiftUp1_in = 1'd0;
						shiftLeft1_in = 1'd0;
						shiftLeft2_in = 1'd0;
						shiftRight2_in = 1'd0;

						
						case(gridX)
						
							10'd0:
							begin
								shiftRight1_in = 1'd1;
								
								if(grid[gridX+1][gridY] == 3'd0 && grid[gridX+2][gridY] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									
									if(grid[gridX+1][gridY] == 3'd0 && grid[gridX+2][gridY] == 3'd0 && grid[gridX+2][gridY-1] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
									
									if(grid[gridX+1][gridY] == 3'd0 && grid[gridX+2][gridY] == 3'd0 && grid[gridX+2][gridY-1] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
							end
							
							default:
							begin
								shiftRight1_in = 1'd0;
								
								if(grid[gridX+1][gridY] == 3'd0 && grid[gridX-1][gridY] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									
									if(grid[gridX+1][gridY] == 3'd0 && grid[gridX+1][gridY-1] == 3'd0 && grid[gridX-1][gridY] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
									
									if(grid[gridX+1][gridY] == 3'd0 && grid[gridX+1][gridY-1] == 3'd0 && grid[gridX-1][gridY] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
							end
						
						
						endcase
							
							
							
							
					end
				
				2'd2:
					begin
					
					
						if(currX + blockSize*2 < maxBlockPixX && grid[gridX+2][gridY] == 3'd0 && grid[gridX][gridY+1] == 3'd0)
							canMoveRight_in = 1'd1;
						else
							canMoveRight_in = 1'd0;
							
						if(currX - blockSize > minBlockPixX && grid[gridX-2][gridY] == 3'd0 && grid[gridX-2][gridY+1] == 3'd0)
							canMoveLeft_in = 1'd1;
						else
							canMoveLeft_in = 1'd0;
							
						if(currY + blockSize*2 < maxBlockPixY && grid[gridX-1][gridY+2] == 3'd0 && grid[gridX][gridY+1] == 3'd0 && grid[gridX+1][gridY+1] == 3'd0)
							canMoveDown_in = 1'd1;
						else
							canMoveDown_in = 1'd0;
							
							
							
						shiftUp2_in = 1'd0;
						shiftUp1_in = 1'd0;
						shiftLeft1_in = 1'd0;
						shiftLeft2_in = 1'd0;
						shiftRight1_in = 1'd0;
						shiftRight2_in = 1'd0;

						
						if(grid[gridX][gridY-1] == 3'd0 && grid[gridX][gridY+1] == 3'd0 && grid[gridX-1][gridY-1] == 3'd0)
						begin
							canRotateClockwise_in = 1'd1;
							
							if(grid[gridX][gridY+1] == 3'd0 && grid[gridX][gridY-1] == 3'd0 && grid[gridX+1][gridY+1] == 3'd0)
								canRotateCounterClockwise_in = 1'd1;
							else
								canRotateCounterClockwise_in = 1'd0;
						end
						else
						begin
							canRotateClockwise_in = 1'd0;
							
							if(grid[gridX][gridY+1] == 3'd0 && grid[gridX][gridY-1] == 3'd0 && grid[gridX+1][gridY+1] == 3'd0)
								canRotateCounterClockwise_in = 1'd1;
							else
								canRotateCounterClockwise_in = 1'd0;
						end

							
					end
				
				2'd3:
					begin
					
					
						if(currX + blockSize < maxBlockPixX && grid[gridX+1][gridY] == 3'd0 && grid[gridX+1][gridY-1] == 3'd0 && grid[gridX+1][gridY+1] == 3'd0)
							canMoveRight_in = 1'd1;
						else
							canMoveRight_in = 1'd0;
							
						if(currX - blockSize > minBlockPixX && grid[gridX-1][gridY] == 3'd0 && grid[gridX-2][gridY-1] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0)
							canMoveLeft_in = 1'd1;
						else
							canMoveLeft_in = 1'd0;
						
						if(currY + blockSize*2 < maxBlockPixY && grid[gridX][gridY+2] == 3'd0 && grid[gridX-1][gridY] == 3'd0)
							canMoveDown_in = 1'd1;
						else
							canMoveDown_in = 1'd0;
							
							
						
						shiftUp2_in = 1'd0;
						shiftUp1_in = 1'd0;
						shiftLeft2_in = 1'd0;
						shiftRight1_in = 1'd0;
						shiftRight2_in = 1'd0;

						
						case(gridX)
						
							10'd9:
								begin
									shiftLeft1_in = 1'd1;
									
									if(grid[gridX-1][gridY] == 3'd0 && grid[gridX-2][gridY] == 3'd0)
									begin
										canRotateClockwise_in = 1'd1;
										if(grid[gridX-1][gridY] == 3'd0 && grid[gridX-2][gridY] == 3'd0 && grid[gridX-2][gridY+1] == 3'd0)
											canRotateCounterClockwise_in = 1'd1;
										else
											canRotateCounterClockwise_in = 1'd0;
									end
									else
									begin
										canRotateClockwise_in = 1'd0;
										
										if(grid[gridX-1][gridY] == 3'd0 && grid[gridX-2][gridY] == 3'd0 && grid[gridX-2][gridY+1] == 3'd0)
											canRotateCounterClockwise_in = 1'd1;
										else
											canRotateCounterClockwise_in = 1'd0;
									end
								end
						
							default:
								begin
									shiftLeft1_in = 1'd0;
									
									if(grid[gridX-1][gridY] == 3'd0 && grid[gridX+1][gridY] == 3'd0 && grid[gridX+1][gridY-1] == 3'd0)
									begin
										canRotateClockwise_in = 1'd1;
										if(grid[gridX-1][gridY] == 3'd0 && grid[gridX+1][gridY] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0)
											canRotateCounterClockwise_in = 1'd1;
										else
											canRotateCounterClockwise_in = 1'd0;
									end
									else
									begin
										canRotateClockwise_in = 1'd0;
										
										if(grid[gridX-1][gridY] == 3'd0 && grid[gridX+1][gridY] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0)
											canRotateCounterClockwise_in = 1'd1;
										else
											canRotateCounterClockwise_in = 1'd0;
									end
								end
						endcase
					end
			endcase		
		3'd3: // J piece
			case(controlledState)
				2'd0:
					begin
					
					
						if(currX + blockSize*2 < maxBlockPixX && grid[gridX+2][gridY] == 3'd0 && grid[gridX][gridY-1] == 3'd0)
							canMoveRight_in = 1'd1;
						else
							canMoveRight_in = 1'd0;
							
						if(currX - blockSize > minBlockPixX && grid[gridX-2][gridY] == 3'd0 && grid[gridX-2][gridY-1] == 3'd0)
							canMoveLeft_in = 1'd1;
						else
							canMoveLeft_in = 1'd0;
							
						if(currY + blockSize < maxBlockPixY && grid[gridX-1][gridY+1] == 3'd0 && grid[gridX][gridY+1] == 3'd0 && grid[gridX+1][gridY+1] == 3'd0)
							canMoveDown_in = 1'd1;
						else
							canMoveDown_in = 1'd0;
							
							
							
						shiftUp2_in = 1'd0;
						shiftLeft1_in = 1'd0;
						shiftLeft2_in = 1'd0;
						shiftRight1_in = 1'd0;
						shiftRight2_in = 1'd0;

						case(gridY)
						
							10'd19:
							begin
								begin
									shiftUp1_in = 1'd1;
									
									if(grid[gridX][gridY-1] == 3'd0 && grid[gridX][gridY-2] == 3'd0 && grid[gridX+1][gridY-2] == 3'd0)
									begin
										canRotateClockwise_in = 1'd1;
										
										if(grid[gridX][gridY-1] == 3'd0 && grid[gridX][gridY-2] == 3'd0)
											canRotateCounterClockwise_in = 1'd1;
										else
											canRotateCounterClockwise_in = 1'd0;
									end
									else
									begin
										canRotateClockwise_in = 1'd0;
										
										if(grid[gridX][gridY-1] == 3'd0 && grid[gridX][gridY-2] == 3'd0)
											canRotateCounterClockwise_in = 1'd1;
										else
											canRotateCounterClockwise_in = 1'd0;
									end
								end
							end
							
							
							default:
								begin
									shiftUp1_in = 1'd0;
									
									if(grid[gridX][gridY-1] == 3'd0 && grid[gridX][gridY+1] == 3'd0 && grid[gridX+1][gridY-1] == 3'd0)
									begin
										canRotateClockwise_in = 1'd1;
										
										if(grid[gridX][gridY-1] == 3'd0 && grid[gridX][gridY+1] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0)
											canRotateCounterClockwise_in = 1'd1;
										else
											canRotateCounterClockwise_in = 1'd0;
									end
									else
									begin
										canRotateClockwise_in = 1'd0;
										
										if(grid[gridX][gridY-1] == 3'd0 && grid[gridX][gridY+1] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0)
											canRotateCounterClockwise_in = 1'd1;
										else
											canRotateCounterClockwise_in = 1'd0;
									end
								end
						
						endcase
		
					end
				
				2'd1:
					begin
						
						if(currX + blockSize*2 < maxBlockPixX && grid[gridX+2][gridY-1] == 3'd0 && grid[gridX+1][gridY] == 3'd0 && grid[gridX+1][gridY-1] == 3'd0)
							canMoveRight_in = 1'd1;
						else
							canMoveRight_in = 1'd0;
							
						if(currX > minBlockPixX && grid[gridX-1][gridY] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0 && grid[gridX-1][gridY-1] == 3'd0)
							canMoveLeft_in = 1'd1;
						else
							canMoveLeft_in = 1'd0;
							
						if(currY + blockSize*2 < maxBlockPixY && grid[gridX][gridY+2] == 3'd0 && grid[gridX+1][gridY] == 3'd0)
							canMoveDown_in = 1'd1;
						else
							canMoveDown_in = 1'd0;

						shiftUp2_in = 1'd0;
						shiftUp1_in = 1'd0;
						shiftLeft1_in = 1'd0;
						shiftLeft2_in = 1'd0;
						shiftRight2_in = 1'd0;
						
						case(gridX)
							10'd0:
							begin
								shiftRight1_in = 1'd1;
								
								if(grid[gridX+1][gridY] == 3'd0 && grid[gridX+2][gridY] == 3'd0 && grid[gridX+2][gridY+1] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									
									if(grid[gridX+1][gridY] == 3'd0 && grid[gridX+2][gridY] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd1;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
									
									if(grid[gridX+1][gridY] == 3'd0 && grid[gridX+2][gridY] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd1;
								end
							end

							default:
							begin
								shiftRight1_in = 1'd0;
								
								if(grid[gridX+1][gridY] == 3'd0 && grid[gridX+1][gridY+1] == 3'd0 && grid[gridX-1][gridY] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									
									if(grid[gridX+1][gridY] == 3'd0 && grid[gridX-1][gridY] == 3'd0 && grid[gridX-1][gridY-1] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd1;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
									
									if(grid[gridX+1][gridY] == 3'd0 && grid[gridX-1][gridY] == 3'd0 && grid[gridX-1][gridY-1] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd1;
								end
							end
						endcase
					end
				
				2'd2:
					begin
						
					
						if(currX + blockSize*2 < maxBlockPixX && grid[gridX+2][gridY] == 3'd0 && grid[gridX+2][gridY+1] == 3'd0)
							canMoveRight_in = 1'd1;
						else
							canMoveRight_in = 1'd0;
							
						if(currX - blockSize > minBlockPixX && grid[gridX-2][gridY] == 3'd0 && grid[gridX][gridY+1] == 3'd0)
							canMoveLeft_in = 1'd1;
						else
							canMoveLeft_in = 1'd0;
							
						if(currY + blockSize*2 < maxBlockPixY && grid[gridX-1][gridY+1] == 3'd0 && grid[gridX][gridY+1] == 3'd0 && grid[gridX+1][gridY+2] == 3'd0)
							canMoveDown_in = 1'd1;
						else
							canMoveDown_in = 1'd0;
							
	
						shiftUp2_in = 1'd0;
						shiftUp1_in = 1'd0;
						shiftLeft1_in = 1'd0;
						shiftLeft2_in = 1'd0;
						shiftRight1_in = 1'd0;
						shiftRight2_in = 1'd0;

						
						if(grid[gridX][gridY-1] == 3'd0 && grid[gridX][gridY+1] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0)
						begin
							canRotateClockwise_in = 1'd1;
							
							if(grid[gridX][gridY-1] == 3'd0 && grid[gridX][gridY+1] == 3'd0 && grid[gridX+1][gridY-1] == 3'd0)
								canRotateCounterClockwise_in = 1'd1;
							else
								canRotateCounterClockwise_in = 1'd1;
						end
						else
						begin
							canRotateClockwise_in = 1'd0;
							
							if(grid[gridX][gridY-1] == 3'd0 && grid[gridX][gridY+1] == 3'd0 && grid[gridX+1][gridY-1] == 3'd0)
								canRotateCounterClockwise_in = 1'd1;
							else
								canRotateCounterClockwise_in = 1'd1;
						end
	
					end
				2'd3:
					begin

						if(currX + blockSize < maxBlockPixX && grid[gridX+1][gridY] == 3'd0 && grid[gridX+1][gridY+1] == 3'd0 && grid[gridX+1][gridY-1] == 3'd0)
							canMoveRight_in = 1'd1;
						else
							canMoveRight_in = 1'd0;
							
						if(currX - blockSize > minBlockPixX && grid[gridX-1][gridY] == 3'd0 && grid[gridX-2][gridY+1] == 3'd0 && grid[gridX-1][gridY-1] == 3'd0)
							canMoveLeft_in = 1'd1;
						else
							canMoveLeft_in = 1'd0;
							
						if(currY + blockSize*2 < maxBlockPixY && grid[gridX-1][gridY+2] == 3'd0 && grid[gridX][gridY+2] == 3'd0)
							canMoveDown_in = 1'd1;
						else
							canMoveDown_in = 1'd0;
							
							
					
						shiftUp2_in = 1'd0;
						shiftUp1_in = 1'd0;
						shiftLeft2_in = 1'd0;
						shiftRight1_in = 1'd0;
						shiftRight2_in = 1'd0;

						case(gridX)
						
							
							10'd9:
							begin
								shiftLeft1_in = 1'd1;
								
								if(grid[gridX-1][gridY] == 3'd0 && grid[gridX-2][gridY] == 3'd0 && grid[gridX-2][gridY-1] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									
									if(grid[gridX-1][gridY] == 3'd0 && grid[gridX-2][gridY] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
									
									if(grid[gridX-1][gridY] == 3'd0 && grid[gridX-2][gridY] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
							end
						
							
							default:
							begin
								shiftLeft1_in = 1'd0;
								
								if(grid[gridX+1][gridY] == 3'd0 && grid[gridX-1][gridY] == 3'd0 && grid[gridX-1][gridY-1] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									
									if(grid[gridX+1][gridY] == 3'd0 && grid[gridX-1][gridY] == 3'd0 && grid[gridX+1][gridY+1] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
									
									if(grid[gridX+1][gridY] == 3'd0 && grid[gridX-1][gridY] == 3'd0 && grid[gridX+1][gridY+1] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
							end
							
						endcase	
					end
			endcase
		3'd4: // T piece
			case(controlledState)
				2'd0:
					begin
					
						if(currX + blockSize*2 < maxBlockPixX && grid[gridX+2][gridY] == 3'd0 && grid[gridX+1][gridY-1] == 3'd0)
							canMoveRight_in = 1'd1;
						else
							canMoveRight_in = 1'd0;
							
						if(currX - blockSize > minBlockPixX && grid[gridX-2][gridY] == 3'd0 && grid[gridX-1][gridY-1] == 3'd0)
							canMoveLeft_in = 1'd1;
						else
							canMoveLeft_in = 1'd0;
							
						if(currY + blockSize < maxBlockPixY && grid[gridX][gridY+1] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0 && grid[gridX+1][gridY+1] == 3'd0)
							canMoveDown_in = 1'd1;
						else
							canMoveDown_in = 1'd0;
							
							
							
						shiftUp2_in = 1'd0;
						shiftLeft1_in = 1'd0;
						shiftLeft2_in = 1'd0;
						shiftRight1_in = 1'd0;
						shiftRight2_in = 1'd0;
						
						
						case(gridY)
						
							10'd19:
							begin
								shiftUp1_in = 1'd1;
							
								if(grid[gridX+1][gridY-1] == 3'd0 && grid[gridX][gridY-2] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									
									if(grid[gridX-1][gridY-1] == 3'd0 && grid[gridX][gridY-2] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
									
									if(grid[gridX-1][gridY-1] == 3'd0 && grid[gridX][gridY-2] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
							end
						
							default:
							begin
								shiftUp1_in = 1'd0;
							
								if(grid[gridX][gridY-1] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									canRotateCounterClockwise_in = 1'd1;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
									canRotateCounterClockwise_in = 1'd0;
								end
							end
						endcase
							
							
					end
				
				2'd1:
					begin
					
						if(currX + blockSize*2 < maxBlockPixX && grid[gridX+2][gridY] == 3'd0 && grid[gridX+1][gridY+1] == 3'd0 && grid[gridX+1][gridY-1] == 3'd0)
							canMoveRight_in = 1'd1;
						else
							canMoveRight_in = 1'd0;
							
						if(currX > minBlockPixX && grid[gridX-1][gridY] == 3'd0 && grid[gridX-1][gridY-1] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0)
							canMoveLeft_in = 1'd1;
						else
							canMoveLeft_in = 1'd0;
							
						if(currY + blockSize*2 < maxBlockPixY && grid[gridX][gridY+2] == 3'd0 && grid[gridX+1][gridY+1] == 3'd0)
							canMoveDown_in = 1'd1;
						else
							canMoveDown_in = 1'd0;
						
						
						shiftUp2_in = 1'd0;
						shiftUp1_in = 1'd0;
						shiftLeft1_in = 1'd0;
						shiftLeft2_in = 1'd0;
						shiftRight2_in = 1'd0;
				
						case(gridX)
							
							10'd0:
							begin
								shiftRight1_in = 1'd1;
							
								if(grid[gridX+1][gridY+1] == 0 && grid[gridX+2][gridY] == 0)
								begin
									canRotateClockwise_in = 1'd1;
									
									if(grid[gridX+1][gridY-1] == 0 && grid[gridX+2][gridY] == 0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
								
									if(grid[gridX+1][gridY-1] == 0 && grid[gridX+2][gridY] == 0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
							end
							
							
							default:
							begin
								shiftRight1_in = 1'd0;
							
								if(grid[gridX-1][gridY] == 0)
								begin
									canRotateCounterClockwise_in = 1'd1;
									canRotateClockwise_in = 1'd1;
								end
								else
								begin
									canRotateCounterClockwise_in = 1'd0;
									canRotateClockwise_in = 1'd0;
								end
							end
						
						
						
						endcase
						
						
						
						
					end
				2'd2:
					begin

					
						if(currX + blockSize*2 < maxBlockPixX && grid[gridX+2][gridY] == 3'd0 && grid[gridX+1][gridY+1] == 3'd0)
							canMoveRight_in = 1'd1;
						else
							canMoveRight_in = 1'd0;
							
						if(currX - blockSize > minBlockPixX && grid[gridX-2][gridY] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0)
							canMoveLeft_in = 1'd1;
						else
							canMoveLeft_in = 1'd0;
							
						if(currY + blockSize*2 < maxBlockPixY && grid[gridX][gridY+2] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0 && grid[gridX+1][gridY+1] == 3'd0)
							canMoveDown_in = 1'd1;
						else
							canMoveDown_in = 1'd0;
							
						
					
						shiftUp2_in = 1'd0;
						shiftUp1_in = 1'd0;
						shiftLeft1_in = 1'd0;
						shiftLeft2_in = 1'd0;
						shiftRight1_in = 1'd0;
						shiftRight2_in = 1'd0;

						if(grid[gridX][gridY-1] == 0)
						begin
							canRotateCounterClockwise_in = 1'd1;
							canRotateClockwise_in = 1'd1;
						end
						else
						begin
							canRotateCounterClockwise_in = 1'd0;
							canRotateClockwise_in = 1'd0;
						end
					end
				
				2'd3:
					begin

						if(currX + blockSize < maxBlockPixX && grid[gridX+1][gridY] == 3'd0 && grid[gridX+1][gridY+1] == 3'd0 && grid[gridX+1][gridY-1] == 3'd0)
							canMoveRight_in = 1'd1;
						else
							canMoveRight_in = 1'd0;
							
						if(currX - blockSize > minBlockPixX && grid[gridX-2][gridY] == 3'd0 && grid[gridX-1][gridY-1] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0)
							canMoveLeft_in = 1'd1;
						else
							canMoveLeft_in = 1'd0;
							
						if(currY + blockSize*2 < maxBlockPixY && grid[gridX][gridY+2] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0)
							canMoveDown_in = 1'd1;
						else
							canMoveDown_in = 1'd0;

						shiftUp2_in = 1'd0;
						shiftUp1_in = 1'd0;
						shiftLeft2_in = 1'd0;
						shiftRight1_in = 1'd0;
						shiftRight2_in = 1'd0;
						
						case(gridX)
							
							10'd9:
							begin
								shiftLeft1_in = 1'd1;
								
								if(grid[gridX][gridY-1] == 3'd0 && grid[gridX-1][gridY] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									
									if(grid[gridX-2][gridY] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
									
									if(grid[gridX-2][gridY] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
							end

							default:
							begin
								shiftLeft1_in = 1'd0;
							
								if(grid[gridX][gridY-1] == 3'd0)
								begin
									canRotateCounterClockwise_in = 1'd1;
									canRotateClockwise_in = 1'd1;
								end
								else
								begin
									canRotateCounterClockwise_in = 1'd0;
									canRotateClockwise_in = 1'd0;
								end
							end
						
						endcase
					end
			endcase
		3'd5: // S piece
			case(controlledState)
				2'd0:
					begin
						
						if(currX + blockSize*2 < maxBlockPixX && grid[gridX+2][gridY-1] == 3'd0 && grid[gridX+1][gridY] == 3'd0)
							canMoveRight_in = 1'd1;
						else
							canMoveRight_in = 1'd0;
							
						if(currX - blockSize > minBlockPixX && grid[gridX-2][gridY] == 3'd0 && grid[gridX-1][gridY-1] == 3'd0)
							canMoveLeft_in = 1'd1;
						else
							canMoveLeft_in = 1'd0;
							
						if(currY + blockSize < maxBlockPixY && grid[gridX][gridY+1] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0 && grid[gridX+1][gridY] == 3'd0)
							canMoveDown_in = 1'd1;
						else
							canMoveDown_in = 1'd0;
							
							

						shiftUp2_in = 1'd0;
						shiftLeft1_in = 1'd0;
						shiftLeft2_in = 1'd0;
						shiftRight1_in = 1'd0;
						shiftRight2_in = 1'd0;
					
						case(gridY)
						
						
							10'd19:
							begin
								shiftUp1_in = 1'd1;
							
								if(grid[gridX][gridY-2] == 0 && grid[gridX+1][gridY] == 0)
								begin
									canRotateClockwise_in = 1'd1;
									
									if(grid[gridX-1][gridY] == 0 && grid[gridX-1][gridY-2] == 0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd1;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
									
									if(grid[gridX-1][gridY] == 0 && grid[gridX-1][gridY-2] == 0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd1;
								end
							end

							default:
							begin
								shiftUp1_in = 1'd0;
							
								if(grid[gridX+1][gridY] == 0 && grid[gridX+1][gridY+1] == 0)
								begin
									canRotateClockwise_in = 1'd1;
									
									if(grid[gridX-1][gridY-1] == 0 && grid[gridX][gridY+1] == 0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd1;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
									
									if(grid[gridX-1][gridY-1] == 0 && grid[gridX][gridY+1] == 0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd1;
								end
							end

						endcase
	
					end
				
				2'd1:
					begin

						if(currX + blockSize*2 < maxBlockPixX && grid[gridX+2][gridY] == 3'd0 && grid[gridX+2][gridY+1] == 3'd0 && grid[gridX+1][gridY-1] == 3'd0)
							canMoveRight_in = 1'd1;
						else
							canMoveRight_in = 1'd0;
							
						if(currX > minBlockPixX && grid[gridX-1][gridY] == 3'd0 && grid[gridX-1][gridY-1] == 3'd0 && grid[gridX][gridY+1] == 3'd0)
							canMoveLeft_in = 1'd1;
						else
							canMoveLeft_in = 1'd0;
							
						if(currY + blockSize*2 < maxBlockPixY && grid[gridX][gridY+1] == 3'd0 && grid[gridX+1][gridY+2] == 3'd0)
							canMoveDown_in = 1'd1;
						else
							canMoveDown_in = 1'd0;

						shiftUp2_in = 1'd0;
						shiftUp1_in = 1'd0;
						shiftLeft1_in = 1'd0;
						shiftLeft2_in = 1'd0;
						shiftRight2_in = 1'd0;
			
						case(gridX)
							10'd0:
							begin
								shiftRight1_in = 1'd1;
								
								if(grid[gridX+2][gridY] == 3'd0 && grid[gridX][gridY+1] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									
									if(grid[gridX+1][gridY-1] == 3'd0 && grid[gridX+2][gridY-1] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
									
									if(grid[gridX+1][gridY-1] == 3'd0 && grid[gridX+2][gridY-1] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
							end
							default:
							begin
								shiftRight1_in = 1'd0;
								
								if(grid[gridX][gridY+1] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									
									if(grid[gridX+1][gridY-1] == 3'd0 && grid[gridX-1][gridY] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
									
									if(grid[gridX+1][gridY-1] == 3'd0 && grid[gridX-1][gridY] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
							end
						endcase	
					end
				
				2'd2:
					begin

						if(currX + blockSize*2 < maxBlockPixX && grid[gridX+2][gridY] == 3'd0 && grid[gridX+1][gridY+1] == 3'd0)
							canMoveRight_in = 1'd1;
						else
							canMoveRight_in = 1'd0;
							
						if(currX - blockSize > minBlockPixX && grid[gridX-1][gridY] == 3'd0 && grid[gridX-2][gridY+1] == 3'd0)
							canMoveLeft_in = 1'd1;
						else
							canMoveLeft_in = 1'd0;
							
						if(currY + blockSize*2 < maxBlockPixY && grid[gridX][gridY+2] == 3'd0 && grid[gridX-1][gridY+2] == 3'd0 && grid[gridX+1][gridY+1] == 3'd0)
							canMoveDown_in = 1'd1;
						else
							canMoveDown_in = 1'd0;
							
					
						shiftUp2_in = 1'd0;
						shiftUp1_in = 1'd0;
						shiftLeft1_in = 1'd0;
						shiftLeft2_in = 1'd0;
						shiftRight1_in = 1'd0;
						shiftRight2_in = 1'd0;
					
						if(grid[gridX-1][gridY] == 3'd0 && grid[gridX-1][gridY-1] == 3'd0)
						begin
							canRotateClockwise_in = 1'd1;
							
							if(grid[gridX][gridY-1] == 3'd0 && grid[gridX+1][gridY+1] == 3'd0)
								canRotateCounterClockwise_in = 1'd1;
							else
								canRotateCounterClockwise_in = 1'd0;
						end
						else
						begin
							canRotateClockwise_in = 1'd0;
							
							if(grid[gridX][gridY-1] == 3'd0 && grid[gridX+1][gridY+1] == 3'd0)
								canRotateCounterClockwise_in = 1'd1;
							else
								canRotateCounterClockwise_in = 1'd0;
						end
					end
				
				2'd3:
					begin

						if(currX + blockSize < maxBlockPixX && grid[gridX+1][gridY] == 3'd0 && grid[gridX+1][gridY+1] == 3'd0 && grid[gridX][gridY-1] == 3'd0)
							canMoveRight_in = 1'd1;
						else
							canMoveRight_in = 1'd0;
							
						if(currX - blockSize > minBlockPixX && grid[gridX-2][gridY] == 3'd0 && grid[gridX-2][gridY-1] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0)
							canMoveLeft_in = 1'd1;
						else
							canMoveLeft_in = 1'd0;
							
						if(currY + blockSize*2 < maxBlockPixY && grid[gridX][gridY+2] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0)
							canMoveDown_in = 1'd1;
						else
							canMoveDown_in = 1'd0;

						shiftUp2_in = 1'd0;
						shiftUp1_in = 1'd0;
						shiftLeft2_in = 1'd0;
						shiftRight1_in = 1'd0;
						shiftRight2_in = 1'd0;
							
						case(gridX)
							10'd9:
							begin
								shiftLeft1_in = 1'd1;
							
								if(grid[gridX][gridY-1] == 3'd0 && grid[gridX-2][gridY] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									
									if(grid[gridX-2][gridY+1] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
									
									if(grid[gridX-2][gridY+1] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
							end
						
						
							default:
							begin
								shiftLeft1_in = 1'd0;
							
								if(grid[gridX][gridY-1] == 3'd0 && grid[gridX+1][gridY-1] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									
									if(grid[gridX+1][gridY] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
									
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
									
									if(grid[gridX+1][gridY] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
							end
						endcase
					end
			endcase
		3'd6: // Z peice
			case(controlledState)
				2'd0:
					begin

						if(currX + blockSize*2 < maxBlockPixX && grid[gridX+2][gridY] == 3'd0 && grid[gridX+1][gridY-1] == 3'd0)
							canMoveRight_in = 1'd1;
						else
							canMoveRight_in = 1'd0;
							
						if(currX - blockSize > minBlockPixX && grid[gridX-2][gridY-1] == 3'd0 && grid[gridX-1][gridY] == 3'd0)
							canMoveLeft_in = 1'd1;
						else
							canMoveLeft_in = 1'd0;
							
						if(currY + blockSize < maxBlockPixY && grid[gridX][gridY+1] == 3'd0 && grid[gridX-1][gridY] == 3'd0 && grid[gridX+1][gridY+1] == 3'd0)
							canMoveDown_in = 1'd1;
						else
							canMoveDown_in = 1'd0;
							
					
						shiftUp2_in = 1'd0;
						shiftLeft1_in = 1'd0;
						shiftLeft2_in = 1'd0;
						shiftRight1_in = 1'd0;
						shiftRight2_in = 1'd0;
						
						case(gridY)
							10'd19:
							begin
								shiftUp1_in = 1'd1;
								
								if(grid[gridX+1][gridY-1] == 3'd0 && grid[gridX+1][gridY-2] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									
									if(grid[gridX-1][gridY] == 3'd0 && grid[gridX][gridY-2] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
									
									if(grid[gridX-1][gridY] == 3'd0 && grid[gridX][gridY-2] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
							end
							
							default:
							begin
								shiftUp1_in = 1'd0;
								
								if(grid[gridX+1][gridY-1] == 3'd0 && grid[gridX][gridY+1] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									
									if(grid[gridX-1][gridY] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
									
									if(grid[gridX-1][gridY] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
							end
						endcase
					end
				
				2'd1:
					begin

						if(currX + blockSize*2 < maxBlockPixX && grid[gridX+2][gridY] == 3'd0 && grid[gridX+2][gridY-1] == 3'd0 && grid[gridX+1][gridY+1] == 3'd0)
							canMoveRight_in = 1'd1;
						else
							canMoveRight_in = 1'd0;
							
						if(currX > minBlockPixX && grid[gridX-1][gridY] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0 && grid[gridX][gridY-1] == 3'd0)
							canMoveLeft_in = 1'd1;
						else
							canMoveLeft_in = 1'd0;
							
						if(currY + blockSize*2 < maxBlockPixY && grid[gridX][gridY+2] == 3'd0 && grid[gridX+1][gridY+1] == 3'd0)
							canMoveDown_in = 1'd1;
						else
							canMoveDown_in = 1'd0;
							
							
						shiftUp2_in = 1'd0;
						shiftUp1_in = 1'd0;
						shiftLeft1_in = 1'd0;
						shiftLeft2_in = 1'd0;
						shiftRight2_in = 1'd0;
						
						case(gridX)
						
							10'd0:
							begin
								shiftRight1_in = 1'd1;
								
								if(grid[gridX+1][gridY+1] == 3'd0 && grid[gridX+2][gridY+1] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									
									if(grid[gridX][gridY-1] == 3'd0 && grid[gridX+2][gridY] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd1;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
									
									if(grid[gridX][gridY-1] == 3'd0 && grid[gridX+2][gridY] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd1;
								end
							end
						
							default:
							begin
								shiftRight1_in = 1'd0;
								
								if(grid[gridX+1][gridY+1] == 3'd0 && grid[gridX-1][gridY] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									
									if(grid[gridX][gridY-1] == 3'd0 && grid[gridX-1][gridY-1] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd1;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
									
									if(grid[gridX][gridY-1] == 3'd0 && grid[gridX-1][gridY-1] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd1;
								end
							end
						endcase		
					end
				
				2'd2:
					begin

						if(currX + blockSize*2 < maxBlockPixX && grid[gridX+1][gridY] == 3'd0 && grid[gridX+2][gridY+1] == 3'd0)
							canMoveRight_in = 1'd1;
						else
							canMoveRight_in = 1'd0;
							
						if(currX - blockSize > minBlockPixX && grid[gridX-2][gridY] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0)
							canMoveLeft_in = 1'd1;
						else
							canMoveLeft_in = 1'd0;
							
						if(currY + blockSize*2 < maxBlockPixY && grid[gridX-1][gridY+1] == 3'd0 && grid[gridX][gridY+2] == 3'd0 && grid[gridX+1][gridY+2] == 3'd0)
							canMoveDown_in = 1'd1;
						else
							canMoveDown_in = 1'd0;

						shiftUp2_in = 1'd0;
						shiftUp1_in = 1'd0;
						shiftLeft1_in = 1'd0;
						shiftLeft2_in = 1'd0;
						shiftRight1_in = 1'd0;
						shiftRight2_in = 1'd0;
						
						if(grid[gridX][gridY-1] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0)
						begin
							canRotateClockwise_in = 1'd1;
							
							if(grid[gridX+1][gridY] == 3'd0 && grid[gridX+1][gridY-1] == 3'd0)
								canRotateCounterClockwise_in = 1'd1;
							else
								canRotateCounterClockwise_in = 1'd0;
						end
						else
						begin
							canRotateClockwise_in = 1'd0;
							
							if(grid[gridX+1][gridY] == 3'd0 && grid[gridX+1][gridY-1] == 3'd0)
								canRotateCounterClockwise_in = 1'd1;
							else
								canRotateCounterClockwise_in = 1'd0;
						end
					end
				
				2'd3:
					begin
					
						if(currX + blockSize < maxBlockPixX && grid[gridX+1][gridY] == 3'd0 && grid[gridX+1][gridY-1] == 3'd0 && grid[gridX][gridY+1] == 3'd0)
							canMoveRight_in = 1'd1;
						else
							canMoveRight_in = 1'd0;
							
						if(currX - blockSize > minBlockPixX && grid[gridX-2][gridY] == 3'd0 && grid[gridX-2][gridY+1] == 3'd0 && grid[gridX-1][gridY-1] == 3'd0)
							canMoveLeft_in = 1'd1;
						else
							canMoveLeft_in = 1'd0;
							
						if(currY + blockSize*2 < maxBlockPixY && grid[gridX-1][gridY+2] == 3'd0 && grid[gridX][gridY+1] == 3'd0)
							canMoveDown_in = 1'd1;
						else
							canMoveDown_in = 1'd0;
					
						shiftUp2_in = 1'd0;
						shiftUp1_in = 1'd0;
						shiftLeft2_in = 1'd0;
						shiftRight1_in = 1'd0;
						shiftRight2_in = 1'd0;
						
						case(gridX)
							10'd9:
							begin
								shiftLeft1_in = 1'd1;
							
								if(grid[gridX-1][gridY-1] == 3'd0 && grid[gridX-2][gridY-1] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									
									if(grid[gridX][gridY+1] == 3'd0 && grid[gridX-2][gridY] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
									
									if(grid[gridX][gridY+1] == 3'd0 && grid[gridX-2][gridY] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
							end

							default:
							begin
								shiftLeft1_in = 1'd0;
							
								if(grid[gridX+1][gridY] == 3'd0 && grid[gridX-1][gridY-1] == 3'd0)
								begin
									canRotateClockwise_in = 1'd1;
									
									if(grid[gridX][gridY+1] == 3'd0 && grid[gridX+1][gridY+1] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
								else
								begin
									canRotateClockwise_in = 1'd0;
									
									if(grid[gridX][gridY+1] == 3'd0 && grid[gridX+1][gridY+1] == 3'd0)
										canRotateCounterClockwise_in = 1'd1;
									else
										canRotateCounterClockwise_in = 1'd0;
								end
							end
						endcase	
					end
			endcase
		3'd7: // O piece
			begin
					
				// rotation
				shiftUp2_in = 1'd0;
				shiftUp1_in = 1'd0;
				canRotateCounterClockwise_in = 1'd1;
				canRotateClockwise_in = 1'd1;
				shiftLeft1_in = 1'd0;
				shiftLeft2_in = 1'd0;
				shiftRight1_in = 1'd0;
				shiftRight2_in = 1'd0;

					
				if(currX + blockSize*2 < maxBlockPixX && grid[gridX+2][gridY] == 3'd0 && grid[gridX+2][gridY+1] == 3'd0)
					canMoveRight_in = 1'd1;
				else
					canMoveRight_in = 1'd0;
					
				if(currX > minBlockPixX && grid[gridX-1][gridY] == 3'd0 && grid[gridX-1][gridY+1] == 3'd0)
					canMoveLeft_in = 1'd1;
				else
					canMoveLeft_in = 1'd0;
					
				if(currY + blockSize*2 < maxBlockPixY && grid[gridX][gridY+2] == 3'd0 && grid[gridX+1][gridY+2] == 3'd0)
					canMoveDown_in = 1'd1;
				else
					canMoveDown_in = 1'd0;
			end
		default:
			begin
				shiftUp2_in = 1'd0;
				shiftUp1_in = 1'd0;
				canMoveRight_in = 1'd1;
				canMoveLeft_in = 1'd1;
				canMoveDown_in = 1'd1;
				canRotateCounterClockwise_in = 1'd1;
				canRotateClockwise_in = 1'd1;
				shiftLeft1_in = 1'd0;
				shiftLeft2_in = 1'd0;
				shiftRight1_in = 1'd0;
				shiftRight2_in = 1'd0;
			end
	endcase

	
end
end		
endmodule
