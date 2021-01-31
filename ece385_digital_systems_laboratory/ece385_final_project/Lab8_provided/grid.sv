module grid(	input clk,
					input Reset,
					input [9:0] DrawX,
					input [9:0] DrawY,
					input [9:0] xPos, ghostPosX,
					input [9:0] yPos, ghostPosY,
					input [2:0] shape,
					input [1:0] state,
					input canMoveDown, delaySigReset, isHardDrop,
					input [9:0] minBlockPixX, minBlockPixY, maxBlockPixX, maxBlockPixY, blockSize,
					
					output [9:0][21:0][2:0] grid_out ,
					output [9:0] gridX, gridXGhost,
					output [9:0] gridY, gridYGhost,
					output [23:0] color,
					output newTetramino,
					output [2:0] gridColor,
					output [14:0] totalLinesCleared);

					
					
					
logic [23:0] backRGB;
logic [23:0] tetraRGB;
logic [7:0] index;
logic [9:0][21:0][2:0] grid  = '{default:3'd0};
logic [9:0][21:0][2:0] grid_temp ='{default:3'd0};
logic [9:0] currX;
logic [9:0] currY;
logic delay;
logic [3:0] clearedLines = 0;
logic [14:0] totalLinesCleared_in = 0;
logic [2:0] fullRow [21:0];



always_comb
begin
	if(grid[0][21]!=0 && grid[1][21]!=0 && grid[2][21]!=0 && grid[3][21]!=0 && grid[4][21]!=0 
	&& grid[5][21]!=0 && grid[6][21]!=0 && grid[7][21]!=0 && grid[8][21]!=0 && grid[9][21]!=0)
		fullRow[21]=1'b1;
	else
		fullRow[21]=1'b0;

	if(grid[0][20]!=0 && grid[1][20]!=0 && grid[2][20]!=0 && grid[3][20]!=0 && grid[4][20]!=0 
	&& grid[5][20]!=0 && grid[6][20]!=0 && grid[7][20]!=0 && grid[8][20]!=0 && grid[9][20]!=0)
		fullRow[20]=1'b1;
	else
		fullRow[20]=1'b0;

	if(grid[0][19]!=0 && grid[1][19]!=0 && grid[2][19]!=0 && grid[3][19]!=0 && grid[4][19]!=0 
	&& grid[5][19]!=0 && grid[6][19]!=0 && grid[7][19]!=0 && grid[8][19]!=0 && grid[9][19]!=0)
		fullRow[19]=1'b1;
	else
		fullRow[19]=1'b0;
		
	if(grid[0][18]!=0 && grid[1][18]!=0 && grid[2][18]!=0 && grid[3][18]!=0 && grid[4][18]!=0 
	&& grid[5][18]!=0 && grid[6][18]!=0 && grid[7][18]!=0 && grid[8][18]!=0 && grid[9][18]!=0)
		fullRow[18]=1'b1;
	else
		fullRow[18]=1'b0;
		
	if(grid[0][17]!=0 && grid[1][17]!=0 && grid[2][17]!=0 && grid[3][17]!=0 && grid[4][17]!=0 
	&& grid[5][17]!=0 && grid[6][17]!=0 && grid[7][17]!=0 && grid[8][17]!=0 && grid[9][17]!=0)
		fullRow[17]=1'b1;
	else
		fullRow[17]=1'b0;
		
	if(grid[0][16]!=0 && grid[1][16]!=0 && grid[2][16]!=0 && grid[3][16]!=0 && grid[4][16]!=0 
	&& grid[5][16]!=0 && grid[6][16]!=0 && grid[7][16]!=0 && grid[8][16]!=0 && grid[9][16]!=0)
		fullRow[16]=1'b1;
	else
		fullRow[16]=1'b0;
		
	if(grid[0][15]!=0 && grid[1][15]!=0 && grid[2][15]!=0 && grid[3][15]!=0 && grid[4][15]!=0 
	&& grid[5][15]!=0 && grid[6][15]!=0 && grid[7][15]!=0 && grid[8][15]!=0 && grid[9][15]!=0)
		fullRow[15]=1'b1;
	else
		fullRow[15]=1'b0;
		
	if(grid[0][14]!=0 && grid[1][14]!=0 && grid[2][14]!=0 && grid[3][14]!=0 && grid[4][14]!=0 
	&& grid[5][14]!=0 && grid[6][14]!=0 && grid[7][14]!=0 && grid[8][14]!=0 && grid[9][14]!=0)
		fullRow[14]=1'b1;
	else
		fullRow[14]=1'b0;
		
	if(grid[0][13]!=0 && grid[1][13]!=0 && grid[2][13]!=0 && grid[3][13]!=0 && grid[4][13]!=0 
	&& grid[5][13]!=0 && grid[6][13]!=0 && grid[7][13]!=0 && grid[8][13]!=0 && grid[9][13]!=0)
		fullRow[13]=1'b1;
	else
		fullRow[13]=1'b0;
		
	if(grid[0][12]!=0 && grid[1][12]!=0 && grid[2][12]!=0 && grid[3][12]!=0 && grid[4][12]!=0 
	&& grid[5][12]!=0 && grid[6][12]!=0 && grid[7][12]!=0 && grid[8][12]!=0 && grid[9][12]!=0)
		fullRow[12]=1'b1;
	else
		fullRow[12]=1'b0;
		
	if(grid[0][11]!=0 && grid[1][11]!=0 && grid[2][11]!=0 && grid[3][11]!=0 && grid[4][11]!=0 
	&& grid[5][11]!=0 && grid[6][11]!=0 && grid[7][11]!=0 && grid[8][11]!=0 && grid[9][11]!=0)
		fullRow[11]=1'b1;
	else
		fullRow[11]=1'b0;
		
	if(grid[0][10]!=0 && grid[1][10]!=0 && grid[2][10]!=0 && grid[3][10]!=0 && grid[4][10]!=0 
	&& grid[5][10]!=0 && grid[6][10]!=0 && grid[7][10]!=0 && grid[8][10]!=0 && grid[9][10]!=0)
		fullRow[10]=1'b1;
	else
		fullRow[10]=1'b0;
		
	if(grid[0][ 9]!=0 && grid[1][ 9]!=0 && grid[2][ 9]!=0 && grid[3][ 9]!=0 && grid[4][ 9]!=0 
	&& grid[5][ 9]!=0 && grid[6][ 9]!=0 && grid[7][ 9]!=0 && grid[8][ 9]!=0 && grid[9][ 9]!=0)
		fullRow[9]=1'b1;
	else
		fullRow[9]=1'b0;
		
	if(grid[0][ 8]!=0 && grid[1][ 8]!=0 && grid[2][ 8]!=0 && grid[3][ 8]!=0 && grid[4][ 8]!=0 
	&& grid[5][ 8]!=0 && grid[6][ 8]!=0 && grid[7][ 8]!=0 && grid[8][ 8]!=0 && grid[9][ 8]!=0)
		fullRow[8]=1'b1;
	else
		fullRow[8]=1'b0;
		
	if(grid[0][ 7]!=0 && grid[1][ 7]!=0 && grid[2][ 7]!=0 && grid[3][ 7]!=0 && grid[4][ 7]!=0 
	&& grid[5][ 7]!=0 && grid[6][ 7]!=0 && grid[7][ 7]!=0 && grid[8][ 7]!=0 && grid[9][ 7]!=0)
		fullRow[7]=1'b1;
	else
		fullRow[7]=1'b0;
		
	if(grid[0][ 6]!=0 && grid[1][ 6]!=0 && grid[2][ 6]!=0 && grid[3][ 6]!=0 && grid[4][ 6]!=0 
	&& grid[5][ 6]!=0 && grid[6][ 6]!=0 && grid[7][ 6]!=0 && grid[8][ 6]!=0 && grid[9][ 6]!=0)
		fullRow[6]=1'b1;
	else
		fullRow[6]=1'b0;
		
	if(grid[0][ 5]!=0 && grid[1][ 5]!=0 && grid[2][ 5]!=0 && grid[3][ 5]!=0 && grid[4][ 5]!=0 
	&& grid[5][ 5]!=0 && grid[6][ 5]!=0 && grid[7][ 5]!=0 && grid[8][ 5]!=0 && grid[9][ 5]!=0)
		fullRow[5]=1'b1;
	else
		fullRow[5]=1'b0;
		
	if(grid[0][ 4]!=0 && grid[1][ 4]!=0 && grid[2][ 4]!=0 && grid[3][ 4]!=0 && grid[4][ 4]!=0 
	&& grid[5][ 4]!=0 && grid[6][ 4]!=0 && grid[7][ 4]!=0 && grid[8][ 4]!=0 && grid[9][ 4]!=0)
		fullRow[4]=1'b1;
	else
		fullRow[4]=1'b0;
		
	if(grid[0][ 3]!=0 && grid[1][ 3]!=0 && grid[2][ 3]!=0 && grid[3][ 3]!=0 && grid[4][ 3]!=0 
	&& grid[5][ 3]!=0 && grid[6][ 3]!=0 && grid[7][ 3]!=0 && grid[8][ 3]!=0 && grid[9][ 3]!=0)
		fullRow[3]=1'b1;
	else
		fullRow[3]=1'b0;
		
	if(grid[0][ 2]!=0 && grid[1][ 2]!=0 && grid[2][ 2]!=0 && grid[3][ 2]!=0 && grid[4][ 2]!=0 
	&& grid[5][ 2]!=0 && grid[6][ 2]!=0 && grid[7][ 2]!=0 && grid[8][ 2]!=0 && grid[9][ 2]!=0)
		fullRow[2]=1'b1;
	else
		fullRow[2]=1'b0;
		
//	if(grid[0][ 1]!=0 && grid[1][ 1]!=0 && grid[2][ 1]!=0 && grid[3][ 1]!=0 && grid[4][ 1]!=0 
//	&& grid[5][ 1]!=0 && grid[6][ 1]!=0 && grid[7][ 1]!=0 && grid[8][ 1]!=0 && grid[9][ 1]!=0)
//		fullRow[1]=1'b1;
//	else
//		fullRow[1]=1'b0;
//		
//	if(grid[0][ 0]!=0 && grid[1][ 0]!=0 && grid[2][ 0]!=0 && grid[3][ 0]!=0 && grid[4][ 0]!=0 
//	&& grid[5][ 0]!=0 && grid[6][ 0]!=0 && grid[7][ 0]!=0 && grid[8][ 0]!=0 && grid[9][ 0]!=0)
//		fullRow[0]=1'b1;
//	else
//		fullRow[0]=1'b0;
		
end
			  




BlockBackground background0(.read_address(index), .Clk(clk), .data_Out(backRGB));
Block block0(.Clk(clk), .read_address(index), .data_Out(tetraRGB), .blockType(gridColor));
delayCounter delay0(.clk, .reset(delaySigReset), .offset(25'd25000000), .delaySig(delay));

always_ff @ (posedge clk or posedge Reset)
begin 
	


	if(Reset)
	begin
		grid_temp<='{default:3'd0};
		totalLinesCleared <= 0;
	end
	else if(!canMoveDown && delay)
		begin
			case(shape)
				3'd1:
					begin
					case(state)
						2'd0:
							begin
								grid_temp[gridX][gridY]<=shape;
								grid_temp[gridX+1][gridY]<=shape;
								grid_temp[gridX-1][gridY]<=shape;
								grid_temp[gridX-2][gridY]<=shape;
							end
						2'd1:
							begin
								grid_temp[gridX-1][gridY]<=shape;
								grid_temp[gridX-1][gridY+1]<=shape;
								grid_temp[gridX-1][gridY-1]<=shape;
								grid_temp[gridX-1][gridY-2]<=shape;
							end
						2'd2:
							begin
								grid_temp[gridX][gridY-1]<=shape;
								grid_temp[gridX+1][gridY-1]<=shape;
								grid_temp[gridX-1][gridY-1]<=shape;
								grid_temp[gridX-2][gridY-1]<=shape;
							end
						2'd3:
							begin
								grid_temp[gridX][gridY]<=shape;
								grid_temp[gridX][gridY+1]<=shape;
								grid_temp[gridX][gridY-1]<=shape;
								grid_temp[gridX][gridY-2]<=shape;
							end
					endcase
					end
				3'd2:
					begin
					case(state)
						2'd0:
							begin
								grid_temp[gridX][gridY]<=shape;
								grid_temp[gridX+1][gridY-1]<=shape;
								grid_temp[gridX+1][gridY]<=shape;
								grid_temp[gridX-1][gridY]<=shape;
							end
						2'd1:
							begin
								grid_temp[gridX][gridY]<=shape;
								grid_temp[gridX][gridY+1]<=shape;
								grid_temp[gridX][gridY-1]<=shape;
								grid_temp[gridX+1][gridY+1]<=shape;
							end
						2'd2:
							begin
								grid_temp[gridX][gridY]<=shape;
								grid_temp[gridX-1][gridY+1]<=shape;
								grid_temp[gridX-1][gridY]<=shape;
								grid_temp[gridX+1][gridY]<=shape;
							end
						2'd3:
							begin
								grid_temp[gridX][gridY]<=shape;
								grid_temp[gridX][gridY+1]<=shape;
								grid_temp[gridX][gridY-1]<=shape;
								grid_temp[gridX-1][gridY-1]<=shape;
							end
					endcase
					end
				3'd3:
					begin
					case(state)
						2'd0:
							begin
								grid_temp[gridX][gridY]<=shape;
								grid_temp[gridX-1][gridY]<=shape;
								grid_temp[gridX+1][gridY]<=shape;
								grid_temp[gridX-1][gridY-1]<=shape;
							end
						2'd1:
							begin
								grid_temp[gridX][gridY]<=shape;
								grid_temp[gridX][gridY-1]<=shape;
								grid_temp[gridX][gridY+1]<=shape;
								grid_temp[gridX+1][gridY-1]<=shape;
							end
						2'd2:
							begin
								grid_temp[gridX][gridY]<=shape;
								grid_temp[gridX-1][gridY]<=shape;
								grid_temp[gridX+1][gridY]<=shape;
								grid_temp[gridX+1][gridY+1]<=shape;
							end
						2'd3:
							begin
								grid_temp[gridX][gridY]<=shape;
								grid_temp[gridX][gridY-1]<=shape;
								grid_temp[gridX][gridY+1]<=shape;
								grid_temp[gridX-1][gridY+1]<=shape;
							end
					endcase
					end
				3'd4:
					begin
					case(state)
						2'd0:
							begin
								grid_temp[gridX][gridY]<=shape;
								grid_temp[gridX][gridY-1]<=shape;
								grid_temp[gridX-1][gridY]<=shape;
								grid_temp[gridX+1][gridY]<=shape;
							end
						2'd1:
							begin
								grid_temp[gridX][gridY]<=shape;
								grid_temp[gridX][gridY+1]<=shape;
								grid_temp[gridX][gridY-1]<=shape;
								grid_temp[gridX+1][gridY]<=shape;
							end
						2'd2:
							begin
								grid_temp[gridX][gridY]<=shape;
								grid_temp[gridX+1][gridY]<=shape;
								grid_temp[gridX-1][gridY]<=shape;
								grid_temp[gridX][gridY+1]<=shape;
							end
						2'd3:
							begin
								grid_temp[gridX][gridY]<=shape;
								grid_temp[gridX][gridY-1]<=shape;
								grid_temp[gridX-1][gridY]<=shape;
								grid_temp[gridX][gridY+1]<=shape;
							end
					endcase
					end
				3'd5:
					begin
					case(state)
						2'd0:
							begin
								grid_temp[gridX][gridY]<=shape;
								grid_temp[gridX][gridY-1]<=shape;
								grid_temp[gridX-1][gridY]<=shape;
								grid_temp[gridX+1][gridY-1]<=shape;
							end
						2'd1:
							begin
								grid_temp[gridX][gridY]<=shape;
								grid_temp[gridX][gridY-1]<=shape;
								grid_temp[gridX+1][gridY]<=shape;
								grid_temp[gridX+1][gridY+1]<=shape;
							end
						2'd2:
							begin
								grid_temp[gridX][gridY]<=shape;
								grid_temp[gridX+1][gridY]<=shape;
								grid_temp[gridX][gridY+1]<=shape;
								grid_temp[gridX-1][gridY+1]<=shape;
							end
						2'd3:
							begin
								grid_temp[gridX][gridY]<=shape;
								grid_temp[gridX-1][gridY-1]<=shape;
								grid_temp[gridX-1][gridY]<=shape;
								grid_temp[gridX][gridY+1]<=shape;
							end
					endcase
					end
				3'd6:
					begin
					case(state)
						2'd0:
							begin
								grid_temp[gridX][gridY]<=shape;
								grid_temp[gridX-1][gridY-1]<=shape;
								grid_temp[gridX][gridY-1]<=shape;
								grid_temp[gridX+1][gridY]<=shape;
							end
						2'd1:
							begin
								grid_temp[gridX][gridY]<=shape;
								grid_temp[gridX][gridY+1]<=shape;
								grid_temp[gridX+1][gridY]<=shape;
								grid_temp[gridX+1][gridY-1]<=shape;
							end
						2'd2:
							begin
								grid_temp[gridX][gridY]<=shape;
								grid_temp[gridX-1][gridY]<=shape;
								grid_temp[gridX][gridY+1]<=shape;
								grid_temp[gridX+1][gridY+1]<=shape;
							end
						2'd3:
							begin
								grid_temp[gridX][gridY]<=shape;
								grid_temp[gridX][gridY-1]<=shape;
								grid_temp[gridX-1][gridY]<=shape;
								grid_temp[gridX-1][gridY+1]<=shape;
							end
					endcase
					end
				3'd7:
					begin
						grid_temp[gridX][gridY]<=shape;
						grid_temp[gridX+1][gridY]<=shape;
						grid_temp[gridX][gridY+1]<=shape;
						grid_temp[gridX+1][gridY+1]<=shape;
					end
			endcase
		end
	
	else if(fullRow[21])
	begin
		grid_temp[0][21:0]<={grid[0][20:0],3'd0};
		grid_temp[1][21:0]<={grid[1][20:0],3'd0};
		grid_temp[2][21:0]<={grid[2][20:0],3'd0};
		grid_temp[3][21:0]<={grid[3][20:0],3'd0};
		grid_temp[4][21:0]<={grid[4][20:0],3'd0};
		grid_temp[5][21:0]<={grid[5][20:0],3'd0};
		grid_temp[6][21:0]<={grid[6][20:0],3'd0};
		grid_temp[7][21:0]<={grid[7][20:0],3'd0};
		grid_temp[8][21:0]<={grid[8][20:0],3'd0};
		grid_temp[9][21:0]<={grid[9][20:0],3'd0};
		grid <= '{default:3'd0};
	end
	else if(fullRow[20])
	begin
		grid_temp[0][20:0]<={grid[0][19:0],3'd0};
		grid_temp[1][20:0]<={grid[1][19:0],3'd0};
		grid_temp[2][20:0]<={grid[2][19:0],3'd0};
		grid_temp[3][20:0]<={grid[3][19:0],3'd0};
		grid_temp[4][20:0]<={grid[4][19:0],3'd0};
		grid_temp[5][20:0]<={grid[5][19:0],3'd0};
		grid_temp[6][20:0]<={grid[6][19:0],3'd0};
		grid_temp[7][20:0]<={grid[7][19:0],3'd0};
		grid_temp[8][20:0]<={grid[8][19:0],3'd0};
		grid_temp[9][20:0]<={grid[9][19:0],3'd0};
		grid <= '{default:3'd0};
	end
	else if(fullRow[19])
	begin
		grid_temp[0][19:0]<={grid[0][18:0],3'd0};
		grid_temp[1][19:0]<={grid[1][18:0],3'd0};
		grid_temp[2][19:0]<={grid[2][18:0],3'd0};
		grid_temp[3][19:0]<={grid[3][18:0],3'd0};
		grid_temp[4][19:0]<={grid[4][18:0],3'd0};
		grid_temp[5][19:0]<={grid[5][18:0],3'd0};
		grid_temp[6][19:0]<={grid[6][18:0],3'd0};
		grid_temp[7][19:0]<={grid[7][18:0],3'd0};
		grid_temp[8][19:0]<={grid[8][18:0],3'd0};
		grid_temp[9][19:0]<={grid[9][18:0],3'd0};
		grid <= '{default:3'd0};
	end
	else if(fullRow[18])
	begin
		grid_temp[0][18:0]<={grid[0][17:0],3'd0};
		grid_temp[1][18:0]<={grid[1][17:0],3'd0};
		grid_temp[2][18:0]<={grid[2][17:0],3'd0};
		grid_temp[3][18:0]<={grid[3][17:0],3'd0};
		grid_temp[4][18:0]<={grid[4][17:0],3'd0};
		grid_temp[5][18:0]<={grid[5][17:0],3'd0};
		grid_temp[6][18:0]<={grid[6][17:0],3'd0};
		grid_temp[7][18:0]<={grid[7][17:0],3'd0};
		grid_temp[8][18:0]<={grid[8][17:0],3'd0};
		grid_temp[9][18:0]<={grid[9][17:0],3'd0};
		grid <= '{default:3'd0};
	end
	else if(fullRow[17])
	begin
		grid_temp[0][17:0]<={grid[0][16:0],3'd0};
		grid_temp[1][17:0]<={grid[1][16:0],3'd0};
		grid_temp[2][17:0]<={grid[2][16:0],3'd0};
		grid_temp[3][17:0]<={grid[3][16:0],3'd0};
		grid_temp[4][17:0]<={grid[4][16:0],3'd0};
		grid_temp[5][17:0]<={grid[5][16:0],3'd0};
		grid_temp[6][17:0]<={grid[6][16:0],3'd0};
		grid_temp[7][17:0]<={grid[7][16:0],3'd0};
		grid_temp[8][17:0]<={grid[8][16:0],3'd0};
		grid_temp[9][17:0]<={grid[9][16:0],3'd0};
		grid <= '{default:3'd0};
	end
	
	else if(fullRow[16])
	begin
		grid_temp[0][16:0]<={grid[0][15:0],3'd0};
		grid_temp[1][16:0]<={grid[1][15:0],3'd0};
		grid_temp[2][16:0]<={grid[2][15:0],3'd0};
		grid_temp[3][16:0]<={grid[3][15:0],3'd0};
		grid_temp[4][16:0]<={grid[4][15:0],3'd0};
		grid_temp[5][16:0]<={grid[5][15:0],3'd0};
		grid_temp[6][16:0]<={grid[6][15:0],3'd0};
		grid_temp[7][16:0]<={grid[7][15:0],3'd0};
		grid_temp[8][16:0]<={grid[8][15:0],3'd0};
		grid_temp[9][16:0]<={grid[9][15:0],3'd0};
		grid <= '{default:3'd0};
	end
	else if(fullRow[15])
	begin
		grid_temp[0][15:0]<={grid[0][14:0],3'd0};
		grid_temp[1][15:0]<={grid[1][14:0],3'd0};
		grid_temp[2][15:0]<={grid[2][14:0],3'd0};
		grid_temp[3][15:0]<={grid[3][14:0],3'd0};
		grid_temp[4][15:0]<={grid[4][14:0],3'd0};
		grid_temp[5][15:0]<={grid[5][14:0],3'd0};
		grid_temp[6][15:0]<={grid[6][14:0],3'd0};
		grid_temp[7][15:0]<={grid[7][14:0],3'd0};
		grid_temp[8][15:0]<={grid[8][14:0],3'd0};
		grid_temp[9][15:0]<={grid[9][14:0],3'd0};
		grid <= '{default:3'd0};
	end
	else if(fullRow[14])
	begin
		grid_temp[0][14:0]<={grid[0][13:0],3'd0};
		grid_temp[1][14:0]<={grid[1][13:0],3'd0};
		grid_temp[2][14:0]<={grid[2][13:0],3'd0};
		grid_temp[3][14:0]<={grid[3][13:0],3'd0};
		grid_temp[4][14:0]<={grid[4][13:0],3'd0};
		grid_temp[5][14:0]<={grid[5][13:0],3'd0};
		grid_temp[6][14:0]<={grid[6][13:0],3'd0};
		grid_temp[7][14:0]<={grid[7][13:0],3'd0};
		grid_temp[8][14:0]<={grid[8][13:0],3'd0};
		grid_temp[9][14:0]<={grid[9][13:0],3'd0};
		grid <= '{default:3'd0};
	end
	else if(fullRow[13])
	begin
		grid_temp[0][13:0]<={grid[0][12:0],3'd0};
		grid_temp[1][13:0]<={grid[1][12:0],3'd0};
		grid_temp[2][13:0]<={grid[2][12:0],3'd0};
		grid_temp[3][13:0]<={grid[3][12:0],3'd0};
		grid_temp[4][13:0]<={grid[4][12:0],3'd0};
		grid_temp[5][13:0]<={grid[5][12:0],3'd0};
		grid_temp[6][13:0]<={grid[6][12:0],3'd0};
		grid_temp[7][13:0]<={grid[7][12:0],3'd0};
		grid_temp[8][13:0]<={grid[8][12:0],3'd0};
		grid_temp[9][13:0]<={grid[9][12:0],3'd0};
		grid <= '{default:3'd0};
	end
	else if(fullRow[12])
	begin
		grid_temp[0][12:0]<={grid[0][11:0],3'd0};
		grid_temp[1][12:0]<={grid[1][11:0],3'd0};
		grid_temp[2][12:0]<={grid[2][11:0],3'd0};
		grid_temp[3][12:0]<={grid[3][11:0],3'd0};
		grid_temp[4][12:0]<={grid[4][11:0],3'd0};
		grid_temp[5][12:0]<={grid[5][11:0],3'd0};
		grid_temp[6][12:0]<={grid[6][11:0],3'd0};
		grid_temp[7][12:0]<={grid[7][11:0],3'd0};
		grid_temp[8][12:0]<={grid[8][11:0],3'd0};
		grid_temp[9][12:0]<={grid[9][11:0],3'd0};
		grid <= '{default:3'd0};
	end
	else if(fullRow[11])
	begin
		grid_temp[0][11:0]<={grid[0][10:0],3'd0};
		grid_temp[1][11:0]<={grid[1][10:0],3'd0};
		grid_temp[2][11:0]<={grid[2][10:0],3'd0};
		grid_temp[3][11:0]<={grid[3][10:0],3'd0};
		grid_temp[4][11:0]<={grid[4][10:0],3'd0};
		grid_temp[5][11:0]<={grid[5][10:0],3'd0};
		grid_temp[6][11:0]<={grid[6][10:0],3'd0};
		grid_temp[7][11:0]<={grid[7][10:0],3'd0};
		grid_temp[8][11:0]<={grid[8][10:0],3'd0};
		grid_temp[9][11:0]<={grid[9][10:0],3'd0};
		grid <= '{default:3'd0};
	end
	else if(fullRow[10])
	begin
		grid_temp[0][10:0]<={grid[0][9:0],3'd0};
		grid_temp[1][10:0]<={grid[1][9:0],3'd0};
		grid_temp[2][10:0]<={grid[2][9:0],3'd0};
		grid_temp[3][10:0]<={grid[3][9:0],3'd0};
		grid_temp[4][10:0]<={grid[4][9:0],3'd0};
		grid_temp[5][10:0]<={grid[5][9:0],3'd0};
		grid_temp[6][10:0]<={grid[6][9:0],3'd0};
		grid_temp[7][10:0]<={grid[7][9:0],3'd0};
		grid_temp[8][10:0]<={grid[8][9:0],3'd0};
		grid_temp[9][10:0]<={grid[9][9:0],3'd0};
		grid <= '{default:3'd0};
	end
	else if(fullRow[9])
	begin
		grid_temp[0][9:0]<={grid[0][8:0],3'd0};
		grid_temp[1][9:0]<={grid[1][8:0],3'd0};
		grid_temp[2][9:0]<={grid[2][8:0],3'd0};
		grid_temp[3][9:0]<={grid[3][8:0],3'd0};
		grid_temp[4][9:0]<={grid[4][8:0],3'd0};
		grid_temp[5][9:0]<={grid[5][8:0],3'd0};
		grid_temp[6][9:0]<={grid[6][8:0],3'd0};
		grid_temp[7][9:0]<={grid[7][8:0],3'd0};
		grid_temp[8][9:0]<={grid[8][8:0],3'd0};
		grid_temp[9][9:0]<={grid[9][8:0],3'd0};
		grid <= '{default:3'd0};
	end
	else if(fullRow[8])
	begin
		grid_temp[0][8:0]<={grid[0][7:0],3'd0};
		grid_temp[1][8:0]<={grid[1][7:0],3'd0};
		grid_temp[2][8:0]<={grid[2][7:0],3'd0};
		grid_temp[3][8:0]<={grid[3][7:0],3'd0};
		grid_temp[4][8:0]<={grid[4][7:0],3'd0};
		grid_temp[5][8:0]<={grid[5][7:0],3'd0};
		grid_temp[6][8:0]<={grid[6][7:0],3'd0};
		grid_temp[7][8:0]<={grid[7][7:0],3'd0};
		grid_temp[8][8:0]<={grid[8][7:0],3'd0};
		grid_temp[9][8:0]<={grid[9][7:0],3'd0};
		grid <= '{default:3'd0};
	end
	else if(fullRow[7])
	begin
		grid_temp[0][7:0]<={grid[0][6:0],3'd0};
		grid_temp[1][7:0]<={grid[1][6:0],3'd0};
		grid_temp[2][7:0]<={grid[2][6:0],3'd0};
		grid_temp[3][7:0]<={grid[3][6:0],3'd0};
		grid_temp[4][7:0]<={grid[4][6:0],3'd0};
		grid_temp[5][7:0]<={grid[5][6:0],3'd0};
		grid_temp[6][7:0]<={grid[6][6:0],3'd0};
		grid_temp[7][7:0]<={grid[7][6:0],3'd0};
		grid_temp[8][7:0]<={grid[8][6:0],3'd0};
		grid_temp[9][7:0]<={grid[9][6:0],3'd0};
		grid <= '{default:3'd0};
	end
	else if(fullRow[6])
	begin
		grid_temp[0][6:0]<={grid[0][5:0],3'd0};
		grid_temp[1][6:0]<={grid[1][5:0],3'd0};
		grid_temp[2][6:0]<={grid[2][5:0],3'd0};
		grid_temp[3][6:0]<={grid[3][5:0],3'd0};
		grid_temp[4][6:0]<={grid[4][5:0],3'd0};
		grid_temp[5][6:0]<={grid[5][5:0],3'd0};
		grid_temp[6][6:0]<={grid[6][5:0],3'd0};
		grid_temp[7][6:0]<={grid[7][5:0],3'd0};
		grid_temp[8][6:0]<={grid[8][5:0],3'd0};
		grid_temp[9][6:0]<={grid[9][5:0],3'd0};
		grid <= '{default:3'd0};
	end
	else if(fullRow[5])
	begin
		grid_temp[0][5:0]<={grid[0][4:0],3'd0};
		grid_temp[1][5:0]<={grid[1][4:0],3'd0};
		grid_temp[2][5:0]<={grid[2][4:0],3'd0};
		grid_temp[3][5:0]<={grid[3][4:0],3'd0};
		grid_temp[4][5:0]<={grid[4][4:0],3'd0};
		grid_temp[5][5:0]<={grid[5][4:0],3'd0};
		grid_temp[6][5:0]<={grid[6][4:0],3'd0};
		grid_temp[7][5:0]<={grid[7][4:0],3'd0};
		grid_temp[8][5:0]<={grid[8][4:0],3'd0};
		grid_temp[9][5:0]<={grid[9][4:0],3'd0};
		grid <= '{default:3'd0};
	end
	else if(fullRow[4])
	begin
		grid_temp[0][4:0]<={grid[0][3:0],3'd0};
		grid_temp[1][4:0]<={grid[1][3:0],3'd0};
		grid_temp[2][4:0]<={grid[2][3:0],3'd0};
		grid_temp[3][4:0]<={grid[3][3:0],3'd0};
		grid_temp[4][4:0]<={grid[4][3:0],3'd0};
		grid_temp[5][4:0]<={grid[5][3:0],3'd0};
		grid_temp[6][4:0]<={grid[6][3:0],3'd0};
		grid_temp[7][4:0]<={grid[7][3:0],3'd0};
		grid_temp[8][4:0]<={grid[8][3:0],3'd0};
		grid_temp[9][4:0]<={grid[9][3:0],3'd0};
		grid <= '{default:3'd0};
	end
	else if(fullRow[3])
	begin
		grid_temp[0][3:0]<={grid[0][2:0],3'd0};
		grid_temp[1][3:0]<={grid[1][2:0],3'd0};
		grid_temp[2][3:0]<={grid[2][2:0],3'd0};
		grid_temp[3][3:0]<={grid[3][2:0],3'd0};
		grid_temp[4][3:0]<={grid[4][2:0],3'd0};
		grid_temp[5][3:0]<={grid[5][2:0],3'd0};
		grid_temp[6][3:0]<={grid[6][2:0],3'd0};
		grid_temp[7][3:0]<={grid[7][2:0],3'd0};
		grid_temp[8][3:0]<={grid[8][2:0],3'd0};
		grid_temp[9][3:0]<={grid[9][2:0],3'd0};
		grid <= '{default:3'd0};
	end
	else if(fullRow[2])
	begin
		grid_temp[0][2:0]<={grid[0][1:0],3'd0};
		grid_temp[1][2:0]<={grid[1][1:0],3'd0};
		grid_temp[2][2:0]<={grid[2][1:0],3'd0};
		grid_temp[3][2:0]<={grid[3][1:0],3'd0};
		grid_temp[4][2:0]<={grid[4][1:0],3'd0};
		grid_temp[5][2:0]<={grid[5][1:0],3'd0};
		grid_temp[6][2:0]<={grid[6][1:0],3'd0};
		grid_temp[7][2:0]<={grid[7][1:0],3'd0};
		grid_temp[8][2:0]<={grid[8][1:0],3'd0};
		grid_temp[9][2:0]<={grid[9][1:0],3'd0};
		grid <= '{default:3'd0};
	end
	else if(fullRow[1])
	begin
		grid_temp[0][1:0]<={grid[0][0],3'd0};
		grid_temp[1][1:0]<={grid[1][0],3'd0};
		grid_temp[2][1:0]<={grid[2][0],3'd0};
		grid_temp[3][1:0]<={grid[3][0],3'd0};
		grid_temp[4][1:0]<={grid[4][0],3'd0};
		grid_temp[5][1:0]<={grid[5][0],3'd0};
		grid_temp[6][1:0]<={grid[6][0],3'd0};
		grid_temp[7][1:0]<={grid[7][0],3'd0};
		grid_temp[8][1:0]<={grid[8][0],3'd0};
		grid_temp[9][1:0]<={grid[9][0],3'd0};
		grid <= '{default:3'd0};
	end
	else if(fullRow[0])
	begin
		grid_temp[0][0]<=3'd0;
		grid_temp[1][0]<=3'd0;
		grid_temp[2][0]<=3'd0;
		grid_temp[3][0]<=3'd0;
		grid_temp[4][0]<=3'd0;
		grid_temp[5][0]<=3'd0;
		grid_temp[6][0]<=3'd0;
		grid_temp[7][0]<=3'd0;
		grid_temp[8][0]<=3'd0;
		grid_temp[9][0]<=3'd0;
		grid <= '{default:3'd0};
	end
	else 
		grid <= grid_temp;
		
	totalLinesCleared <= totalLinesCleared_in;
	


end


always_comb
begin
	
	totalLinesCleared_in = totalLinesCleared;
	
	
	clearedLines = fullRow[0] + fullRow[1] + fullRow[2] + fullRow[3] + fullRow[4] + fullRow[5] + fullRow[6] + fullRow[7] + fullRow[8] + fullRow[9] + fullRow[10] + 
						fullRow[11] + fullRow[12] + fullRow[13] + fullRow[14] + fullRow[15] + fullRow[16] + fullRow[17] + fullRow[18] + fullRow[19] + fullRow[20] + fullRow[21];
	
	
	totalLinesCleared_in = totalLinesCleared + clearedLines;
	
	
	gridX = (xPos - minBlockPixX) / blockSize;
	gridXGhost = (ghostPosX - minBlockPixX) / blockSize;
	gridColor = grid[currX][currY];
	grid_out = grid;
	
	gridY = (yPos - (minBlockPixY - 0)) / blockSize;
	gridYGhost = (ghostPosY - (minBlockPixY - 0)) / blockSize;

	
//	if(yPos > minBlockPixY)
//	begin
//		gridY = (yPos - minBlockPixY) / blockSize;
//		gridYGhost = (ghostPosY - minBlockPixY) / blockSize;
//	end
//	else
//	begin
//		gridY = 10'hFF;
//		gridYGhost = 10'hFF;
//	end
		
	if(DrawX >= minBlockPixX && DrawX <= maxBlockPixX && DrawY >= minBlockPixY && DrawY <= maxBlockPixY)
	begin
		currX = (DrawX - minBlockPixX) / blockSize;
		currY = (DrawY - minBlockPixY) / blockSize;
		index = (DrawX - minBlockPixX) % blockSize + (DrawY - minBlockPixY)*blockSize;
	end
	else
	begin
		currX = 10'hFF;
		currY = 10'hFF;
	   index = 10'hFF;
	end
	
	if((!canMoveDown && delay) || isHardDrop)
		newTetramino = 1;
	else
		newTetramino = 0;

	if(grid[currX][currY] == 3'd0)
	begin
		color = backRGB;
	end
	else
	begin
		color = tetraRGB;
	end
	
	
end

endmodule


module delayCounter(input clk, input [24:0] offset, input reset, output delaySig);

logic [24:0] count = 0;

always_ff @ (posedge clk or posedge reset)
begin
	if(reset)
		count <= 0;
	else
		count <= count+1 % offset;
end

always_comb
begin
	if(count == offset/2)
		delaySig = 1;
	else
		delaySig = 0;
end

endmodule


