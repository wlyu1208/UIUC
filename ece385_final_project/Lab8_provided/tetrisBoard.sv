

module tetrisBoard(  input  clk,
							input  Reset,
							input  [9:0] DrawX,
							input  [9:0] DrawY,
							input  [7:0] keycode,
							input	 key3,
							output [7:0] VGA_R, VGA_G, VGA_B);

 // 640x480 display resolution

logic [2:0]  controlledShape = 3'd2;
logic [2:0]  randomNextShape = 3'd2;
logic [1:0]  controlledState = 2'd0;
logic [9:0]  xPos, yPos;
logic 		 drawTetramino, drawGhost, canGhostMoveDown;	
logic [23:0] TetraminoRGB, ghostRGB;
logic [23:0] gridRGB;
logic canMoveRight;
logic canMoveLeft;
logic canMoveDown;
logic canRotateCounterClockwise;
logic canRotateClockwise;
logic shiftLeft1;
logic shiftLeft2;
logic shiftRight1;
logic shiftRight2;
logic shiftUp1;
logic shiftUp2;
logic newTetramino;
logic [9:0] blockSize = 10'd16;
logic [9:0] minBlockPixX = 10'd240;
logic [9:0] minBlockPixY = 10'd80;
logic [9:0] maxBlockPixX = 10'd400;  
logic [9:0] maxBlockPixY = 10'd400;
logic [9:0][21:0][2:0] grid  = '{default:3'd0};
logic [9:0] gridX, ghostPosX, ghostPosY, gridXGhost, gridYGhost;
logic [9:0] gridY;
logic delaySigReset, isHardDrop, signalStart;
logic [14:0] totalLinesCleared;

assign filled = grid[4][0] != 0 || grid[5][0] != 0 || grid[6][0] != 0;



game_state gameStart(.clk, .Reset, .Run(key3), .filled(filled), .start(signalStart));

tetramino controlledTetramino( .clk, .currX(DrawX), .currY(DrawY), .shape(controlledShape), .state(controlledState), .xPos, .yPos, .isTetramino(drawTetramino),
										 .RGB(TetraminoRGB), .blockSize(blockSize), .isGhost(1'd0));
										 
tetramino ghostPiece( .clk, .currX(DrawX), .currY(DrawY), .shape(controlledShape), .state(controlledState), .xPos(ghostPosX), .yPos(ghostPosY), .isTetramino(drawGhost),
										 .RGB(ghostRGB), .blockSize(blockSize), .isGhost(1'd1));
										 
keyboardControl keyboard( .Clk(clk), .Reset, .DrawX, .DrawY, .state_out(controlledState), .tetraminoX(xPos), .tetraminoY(yPos), .keycode, .canRotateCounterClockwise,
									.canMoveRight, .canMoveLeft, .canMoveDown, .newTetramino, .randomNextShape, .shape_out(controlledShape), .canRotateClockwise, .shiftUp1, .signalStart,
									.shiftLeft1, .shiftLeft2, .shiftRight1, .shiftRight2, .shiftUp2, .delaySigReset, .ghostPosX, .ghostPosY, .canGhostMoveDown, .gridYGhost, .hardDrop(isHardDrop),
									.totalLinesCleared);
									
color_mapper boardColored2019(.clk, .DrawX, .DrawY, .drawTetramino, .TetraminoRGB, .VGA_R, .VGA_G, .VGA_B, .gridRGB, .minBlockPixX, .minBlockPixY,
											.maxBlockPixX, .maxBlockPixY, .drawGhost, .ghostRGB);

collisionDetection collisionDetector(.blockSize(blockSize), .minBlockPixX, .maxBlockPixX, .maxBlockPixY, .currX(xPos), .currY(yPos), .grid, .gridX, .gridY, .clk,
													.controlledShape(controlledShape), .controlledState(controlledState), .canMoveRight, .canMoveLeft, .canMoveDown, .canRotateCounterClockwise,
													.canRotateClockwise, .shiftUp1, .shiftLeft1, .shiftLeft2, .shiftRight1, .shiftRight2, .shiftUp2, .signalStart);

grid grid0(.clk, .xPos, .yPos, .shape(controlledShape), .state(controlledState), .color(gridRGB), .DrawX, .DrawY, .canMoveDown, .Reset, .newTetramino, .minBlockPixX, .isHardDrop,
				.minBlockPixY, .maxBlockPixX, .maxBlockPixY, .blockSize, .grid_out(grid), .gridX, .gridY, .delaySigReset, .gridXGhost, .gridYGhost, .ghostPosX, .ghostPosY, .totalLinesCleared);

randNum newTetraminoGenerator(.clk, .Reset, .randomNextShape, .signalStart);


collisionDetection collisionDetectorGhost(.blockSize(blockSize), .minBlockPixX, .maxBlockPixX, .maxBlockPixY, .currX(ghostPosX), .currY(ghostPosY), .grid, .gridX(gridXGhost), .gridY(gridYGhost), .clk,
													.controlledShape(controlledShape), .controlledState(controlledState), .canMoveDown(canGhostMoveDown)); 





endmodule
