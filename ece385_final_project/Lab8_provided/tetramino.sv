
// KEY
//
// shape:
// 1 - I piece
//  State:
//  0 - Vertical right side
//  1 - Horizontal bottom
//  2 - Vertical left side
//  3 - Horizontal top
//
// 2 - L piece
//  State:
//  0 - right side up
//  1 - clockwise rotation 1
//  2 - clockwise rotation 2
//  3 - clockwise rotation 3
//
// 3 - J piece
//  State:
//  0 - right side up
//  1 - clockwise rotation 1
//  2 - clockwise rotation 2
//  3 - clockwise rotation 3
//
// 4 - T piece
//  State:
//  0 - right side up
//  1 - left side heavy
//  2 - upside down
//  3 - right side heavy
//
// 5 - S piece
//  State:
//  0 - right side up
//  1 - sideways
//
// 6 - Z piece
//  State:
//  0 - right side up
//  1 - sideways
//
// 7 - O piece
//  State:
//  0 - placeholder
//	 1 - placeholder
//  2 - placeholder
//  3 - placeholder

module tetramino(
    input logic clk,
	 input logic	isGhost,
    input logic[2:0] shape,
    input logic[1:0] state,
    input logic[9:0] xPos,
    input logic[9:0] yPos,
    input logic[9:0] currX,
    input logic[9:0] currY,
	 input 		[9:0] blockSize,

    output logic[23:0] RGB,
    output logic isTetramino

);

logic [9:0] tetraminoIndecies [3:0] = '{10'd0, 10'd0, 10'd0, 10'd0};
logic [23:0] RGBblock [3:0];
logic [23:0] pixelColor = 24'h000000;
logic colorThisPixel = 1'd0;
logic [2:0] blockColor;


assign RGB = pixelColor;
assign isTetramino = colorThisPixel;


// instantiate each block according to the correct block
Block block0(.Clk(clk), .read_address(tetraminoIndecies[0]), .data_Out(RGBblock[0]), .blockType(blockColor));
Block block1(.Clk(clk), .read_address(tetraminoIndecies[1]), .data_Out(RGBblock[1]), .blockType(blockColor));
Block block2(.Clk(clk), .read_address(tetraminoIndecies[2]), .data_Out(RGBblock[2]), .blockType(blockColor));
Block block3(.Clk(clk), .read_address(tetraminoIndecies[3]), .data_Out(RGBblock[3]), .blockType(blockColor));



always_comb
begin

if(isGhost)
	blockColor = 3'd0;
else
	blockColor = shape;


case(shape)
    3'd1: // I piece
        begin
            case(state)
                2'd0: // horizontal
                    begin
                        // get the correct indecies for each block
                        tetraminoIndecies[0] = currX - (xPos - blockSize) + (currY - yPos)*blockSize;
                        tetraminoIndecies[1] = currX - (xPos - blockSize*2) + (currY - yPos)*blockSize;
                        tetraminoIndecies[2] = currX - xPos + (currY - yPos)*blockSize;
                        tetraminoIndecies[3] = currX - (xPos + blockSize) + (currY - yPos)*blockSize;
                        
                        // check if the current pixel is over one of the blocks
                        if(currX >= (xPos - blockSize) && currX <= (xPos - blockSize) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[0];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos - blockSize*2) && currX <= (xPos - blockSize*2) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[1];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[2];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos + blockSize) && currX <= (xPos + blockSize) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[3];
                            colorThisPixel = 1'd1;
                        end
								else
								begin
									pixelColor = 24'h000000;
									colorThisPixel = 1'd0;
								end
                    end
					 2'd1:
							begin
								// get the correct indecies for each block
                        tetraminoIndecies[0] = currX - (xPos - blockSize) + (currY - yPos)*blockSize;
                        tetraminoIndecies[1] = currX - (xPos - blockSize) + (currY - (yPos + blockSize))*blockSize;
                        tetraminoIndecies[2] = currX - (xPos - blockSize) + (currY - (yPos - blockSize))*blockSize;
                        tetraminoIndecies[3] = currX - (xPos - blockSize) + (currY - (yPos - blockSize*2))*blockSize;
                        
                        // check if the current pixel is over one of the blocks
                        if(currX >= (xPos - blockSize) && currX <= (xPos - blockSize) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[0];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos - blockSize) && currX <= (xPos - blockSize) + blockSize && currY >= (yPos + blockSize) && currY <= (yPos + blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[1];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos - blockSize) && currX <= (xPos - blockSize) + blockSize && currY >= (yPos - blockSize) && currY <= (yPos - blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[2];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos - blockSize) && currX <= (xPos - blockSize) + blockSize && currY >= (yPos - blockSize*2) && currY <= (yPos - blockSize*2) + blockSize)
                        begin 
                            pixelColor = RGBblock[3];
                            colorThisPixel = 1'd1;
                        end
								else
								begin
									pixelColor = 24'h000000;
									colorThisPixel = 1'd0;
								end
							end
					 2'd2:
							begin
								// get the correct indecies for each block
                        tetraminoIndecies[0] = currX - xPos + (currY - (yPos - blockSize))*blockSize;
                        tetraminoIndecies[1] = currX - (xPos + blockSize) + (currY - (yPos - blockSize))*blockSize;
                        tetraminoIndecies[2] = currX - (xPos - blockSize) + (currY - (yPos - blockSize))*blockSize;
                        tetraminoIndecies[3] = currX - (xPos - blockSize*2) + (currY - (yPos - blockSize))*blockSize;
                        
                        // check if the current pixel is over one of the blocks
                        if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos - blockSize) && currY <= (yPos - blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[0];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos + blockSize) && currX <= (xPos + blockSize) + blockSize && currY >= (yPos - blockSize) && currY <= (yPos - blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[1];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos - blockSize) && currX <= (xPos - blockSize) + blockSize && currY >= (yPos - blockSize) && currY <= (yPos - blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[2];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos - blockSize*2) && currX <= (xPos - blockSize*2) + blockSize && currY >= (yPos - blockSize) && currY <= (yPos - blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[3];
                            colorThisPixel = 1'd1;
                        end
								else
								begin
									pixelColor = 24'h000000;
									colorThisPixel = 1'd0;
								end
							end
					 2'd3: // vertical 
                    begin
                        // get the correct indecies for each block
                        tetraminoIndecies[0] = currX - xPos + (currY - yPos)*blockSize;
                        tetraminoIndecies[1] = currX - xPos + (currY - (yPos + blockSize))*blockSize;
                        tetraminoIndecies[2] = currX - xPos + (currY - (yPos - blockSize))*blockSize;
                        tetraminoIndecies[3] = currX - xPos + (currY - (yPos - blockSize*2))*blockSize;
                
                        // check if the current pixel is over one of the blocks
                        if(currX >= xPos && currX <= xPos + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[0];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos + blockSize) && currY <= (yPos + blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[1];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos - blockSize) && currY <= (yPos - blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[2];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos - blockSize*2) && currY <= (yPos - blockSize*2) + blockSize)
                        begin 
                            pixelColor = RGBblock[3];
                            colorThisPixel = 1'd1;
                        end
								else
								begin
									pixelColor = 24'h000000;
									colorThisPixel = 1'd0;
								end
                    end
                default: 
                    begin
                        tetraminoIndecies[0] = 10'd0;
                        tetraminoIndecies[1] = 10'd0;
                        tetraminoIndecies[2] = 10'd0;
                        tetraminoIndecies[3] = 10'd0;
                        pixelColor = 24'h0;
                        colorThisPixel = 1'b0;
                    end
            endcase
        end

    3'd2: // L piece
        begin
            case(state)
					 2'd0: // clockwise rotation 3
                    begin
                        // get the correct indecies for each block
                        tetraminoIndecies[0] = currX - xPos + (currY - yPos)*blockSize;
                        tetraminoIndecies[1] = currX - (xPos - blockSize) + (currY - yPos)*blockSize;
                        tetraminoIndecies[2] = currX - (xPos + blockSize) + (currY - yPos)*blockSize;
                        tetraminoIndecies[3] = currX - (xPos + blockSize) + (currY - (yPos - blockSize))*blockSize;
                        
                        // check if the current pixel is over one of the blocks
                        if(currX >= xPos && currX <= xPos + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[0];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos - blockSize) && currX <= (xPos - blockSize) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[1];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos + blockSize) && currX <= (xPos + blockSize) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[2];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos + blockSize) && currX <= (xPos + blockSize) + blockSize && currY >= (yPos - blockSize) && currY <= (yPos - blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[3];
                            colorThisPixel = 1'd1;
                        end
								else
								begin
									pixelColor = 24'h000000;
									colorThisPixel = 1'd0;
								end
                    end
                2'd1: // upright
                    begin
                        // get the correct indecies for each block
                        tetraminoIndecies[0] = currX - xPos + (currY - yPos)*blockSize;
                        tetraminoIndecies[1] = currX - xPos + (currY - (yPos - blockSize))*blockSize;
                        tetraminoIndecies[2] = currX - xPos + (currY - (yPos + blockSize))*blockSize;
                        tetraminoIndecies[3] = currX - (xPos + blockSize) + (currY - (yPos + blockSize))*blockSize;
                        
                        // check if the current pixel is over one of the blocks
                        if(currX >= xPos && currX <= xPos + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[0];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos - blockSize) && currY <= (yPos - blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[1];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos + blockSize) && currY <= (yPos + blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[2];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos + blockSize) && currX <= (xPos + blockSize) + blockSize && currY >= (yPos + blockSize) && currY <= (yPos + blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[3];
                            colorThisPixel = 1'd1;
                        end
								else
								begin
									pixelColor = 24'h000000;
									colorThisPixel = 1'd0;
								end
                    end
                2'd2: // clockwise rotation 1
                    begin
                        // get the correct indecies for each block
                        tetraminoIndecies[0] = currX - xPos + (currY - yPos)*blockSize;
                        tetraminoIndecies[1] = currX - (xPos + blockSize) + (currY - yPos)*blockSize;
                        tetraminoIndecies[2] = currX - (xPos - blockSize) + (currY - yPos)*blockSize;
                        tetraminoIndecies[3] = currX - (xPos - blockSize) + (currY - (yPos + blockSize))*blockSize;
 
                        // check if the current pixel is over one of the blocks
                        if(currX >= xPos && currX <= xPos + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[0];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos + blockSize) && currX <= (xPos + blockSize) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[1];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos - blockSize) && currX <= (xPos - blockSize) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[2];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos - blockSize) && currX <= (xPos - blockSize) + blockSize && currY >= (yPos + blockSize) && currY <= (yPos + blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[3];
                            colorThisPixel = 1'd1;
                        end
								else
								begin
									pixelColor = 24'h000000;
									colorThisPixel = 1'd0;
								end
                    end
                2'd3: // clockwise rotation 2
                    begin
                        // get the correct indecies for each block
                        tetraminoIndecies[0] = currX - xPos + (currY - yPos)*blockSize;
                        tetraminoIndecies[1] = currX - xPos + (currY - (yPos + blockSize))*blockSize;
                        tetraminoIndecies[2] = currX - xPos + (currY - (yPos - blockSize))*blockSize; 
                        tetraminoIndecies[3] = currX - (xPos - blockSize) + (currY - (yPos - blockSize))*blockSize;

                        // check if the current pixel is over one of the blocks
                        if(currX >= xPos && currX <= xPos + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[0];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos + blockSize) && currY <= (yPos + blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[1];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos - blockSize) && currY <= (yPos - blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[2];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos - blockSize) && currX <= (xPos - blockSize) + blockSize && currY >= (yPos - blockSize) && currY <= (yPos - blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[3];
                            colorThisPixel = 1'd1;
                        end
								else
								begin
									pixelColor = 24'h000000;
									colorThisPixel = 1'd0;
								end
                    end
                default: 
                    begin
                        tetraminoIndecies[0] = 10'd0;
                        tetraminoIndecies[1] = 10'd0;
                        tetraminoIndecies[2] = 10'd0;
                        tetraminoIndecies[3] = 10'd0;
                        pixelColor = 24'h0;
                        colorThisPixel = 1'b0;
                    end
            endcase
            
        end

    3'd3: // J piece
        begin
            case(state)
                2'd0: // clockwise rotation 1
                    begin
                        // get the correct indecies for each block
                        tetraminoIndecies[0] = currX - xPos + (currY - yPos)*blockSize;
                        tetraminoIndecies[1] = currX - (xPos - blockSize) + (currY - yPos)*blockSize;
                        tetraminoIndecies[2] = currX - (xPos + blockSize) + (currY - yPos)*blockSize;
                        tetraminoIndecies[3] = currX - (xPos - blockSize) + (currY - (yPos - blockSize))*blockSize;
                        
                        // check if the current pixel is over one of the blocks
                        if(currX >= xPos && currX <= xPos + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[0];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos - blockSize) && currX <= (xPos - blockSize) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[1];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos + blockSize) && currX <= (xPos + blockSize) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[2];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos - blockSize) && currX <= (xPos - blockSize) + blockSize && currY >= (yPos - blockSize) && currY <= (yPos - blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[3];
                            colorThisPixel = 1'd1;
                        end
								else
								begin
									pixelColor = 24'h000000;
									colorThisPixel = 1'd0;
								end
                    end
                2'd1: // clockwise rotation 2
                    begin
                        // get the correct indecies for each block
                        tetraminoIndecies[0] = currX - xPos + (currY - yPos)*blockSize;
                        tetraminoIndecies[1] = currX - xPos + (currY - (yPos - blockSize))*blockSize;
                        tetraminoIndecies[2] = currX - xPos + (currY - (yPos + blockSize))*blockSize;
                        tetraminoIndecies[3] = currX - (xPos + blockSize) + (currY - (yPos - blockSize))*blockSize;

                        // check if the current pixel is over one of the blocks
                        if(currX >= xPos && currX <= xPos + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[0];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos - blockSize) && currY <= (yPos - blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[1];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos + blockSize) && currY <= (yPos + blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[2];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos + blockSize) && currX <= (xPos + blockSize) + blockSize && currY >= (yPos - blockSize) && currY <= (yPos - blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[3];
                            colorThisPixel = 1'd1;
                        end
								else
								begin
									pixelColor = 24'h000000;
									colorThisPixel = 1'd0;
								end
                    end
                2'd2: // clockwise rotation 3
                    begin
                        // get the correct indecies for each block
                        tetraminoIndecies[0] = currX - xPos + (currY - yPos)*blockSize;
                        tetraminoIndecies[1] = currX - (xPos - blockSize) + (currY - yPos)*blockSize;
                        tetraminoIndecies[2] = currX - (xPos + blockSize) + (currY - yPos)*blockSize;
                        tetraminoIndecies[3] = currX - (xPos + blockSize) + (currY - (yPos + blockSize))*blockSize;
                        
                        // check if the current pixel is over one of the blocks
                        if(currX >= xPos && currX <= xPos + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[0];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos - blockSize) && currX <= (xPos - blockSize) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[1];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos + blockSize) && currX <= (xPos + blockSize) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[2];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos + blockSize) && currX <= (xPos + blockSize) + blockSize && currY >= (yPos + blockSize) && currY <= (yPos + blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[3];
                            colorThisPixel = 1'd1;
                        end
								else
								begin
									pixelColor = 24'h000000;
									colorThisPixel = 1'd0;
								end
                    end
					 2'd3: // right-side up
                    begin
                        // get the correct indecies for each block
                        tetraminoIndecies[0] = currX - xPos + (currY - yPos)*blockSize;
                        tetraminoIndecies[1] = currX - xPos + (currY - (yPos - blockSize))*blockSize;
                        tetraminoIndecies[2] = currX - xPos + (currY - (yPos + blockSize))*blockSize;
                        tetraminoIndecies[3] = currX - (xPos - blockSize) + (currY - (yPos + blockSize))*blockSize;
                
                        // check if the current pixel is over one of the blocks
                        if(currX >= xPos && currX <= xPos + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[0];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos - blockSize) && currY <= (yPos - blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[1];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos + blockSize) && currY <= (yPos + blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[2];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos - blockSize) && currX <= (xPos - blockSize) + blockSize && currY >= (yPos + blockSize) && currY <= (yPos + blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[3];
                            colorThisPixel = 1'd1;
                        end
								else
								begin
									pixelColor = 24'h000000;
									colorThisPixel = 1'd0;
								end
                    end
                default: 
                    begin
                        tetraminoIndecies[0] = 10'd0;
                        tetraminoIndecies[1] = 10'd0;
                        tetraminoIndecies[2] = 10'd0;
                        tetraminoIndecies[3] = 10'd0;
                        pixelColor = 24'h0;
                        colorThisPixel = 1'b0;
                    end
            endcase
            
        end

    3'd4: // T piece
        begin
            case(state)
				    2'd0: // upside down
                    begin
                        // get the correct indecies for each block
                        tetraminoIndecies[0] = currX - xPos + (currY - yPos)*blockSize; // middle
                        tetraminoIndecies[1] = currX - xPos + (currY - (yPos - blockSize))*blockSize; // top
                        tetraminoIndecies[2] = currX - (xPos - blockSize) + (currY - yPos)*blockSize; // left 
                        tetraminoIndecies[3] = currX - (xPos + blockSize) + (currY - yPos)*blockSize; // right
                
                        // check if the current pixel is over one of the blocks
                        if(currX >= xPos && currX <= xPos + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[0];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos - blockSize) && currY <= (yPos - blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[1];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos - blockSize) && currX <= (xPos - blockSize) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[2];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos + blockSize) && currX <= (xPos + blockSize) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[3];
                            colorThisPixel = 1'd1;
                        end
								else
								begin
									pixelColor = 24'h000000;
									colorThisPixel = 1'd0;
								end
                    end
                2'd1: // right heavy
                    begin
                        // get the correct indecies for each block
                        tetraminoIndecies[0] = currX - xPos + (currY - yPos)*blockSize; // middle
                        tetraminoIndecies[1] = currX - xPos + (currY - (yPos + blockSize))*blockSize; // bottom
                        tetraminoIndecies[2] = currX - (xPos + blockSize) + (currY - yPos)*blockSize; // right
                        tetraminoIndecies[3] = currX - xPos + (currY - (yPos - blockSize))*blockSize; // top
                
                        // check if the current pixel is over one of the blocks
                        if(currX >= xPos && currX <= xPos + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[0];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos + blockSize) && currY <= (yPos + blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[1];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos + blockSize) && currX <= (xPos + blockSize) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[2];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos - blockSize) && currY <= (yPos - blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[3];
                            colorThisPixel = 1'd1;
                        end
								else
								begin
									pixelColor = 24'h000000;
									colorThisPixel = 1'd0;
								end
                    end
                2'd2: // right side up
                    begin
                        // get the correct indecies for each block
                        tetraminoIndecies[0] = currX - xPos + (currY - yPos)*blockSize; // middle
                        tetraminoIndecies[1] = currX - xPos + (currY - (yPos + blockSize))*blockSize; // bottom
                        tetraminoIndecies[2] = currX - (xPos - blockSize) + (currY - yPos)*blockSize; // left 
                        tetraminoIndecies[3] = currX - (xPos + blockSize) + (currY - yPos)*blockSize; // right
                
                        // check if the current pixel is over one of the blocks
                        if(currX >= xPos && currX <= xPos + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[0];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos + blockSize) && currY <= (yPos + blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[1];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos - blockSize) && currX <= (xPos - blockSize) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[2];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos + blockSize) && currX <= (xPos + blockSize) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[3];
                            colorThisPixel = 1'd1;
                        end
								else
								begin
									pixelColor = 24'h000000;
									colorThisPixel = 1'd0;
								end
                    end
                2'd3: // left heavy
                    begin
                        // get the correct indecies for each block
                        tetraminoIndecies[0] = currX - xPos + (currY - yPos)*blockSize; // middle
                        tetraminoIndecies[1] = currX - xPos + (currY - (yPos + blockSize))*blockSize; // bottom
                        tetraminoIndecies[2] = currX - (xPos - blockSize) + (currY - yPos)*blockSize; // left 
                        tetraminoIndecies[3] = currX - xPos + (currY - (yPos - blockSize))*blockSize; // top
                
                        // check if the current pixel is over one of the blocks
                        if(currX >= xPos && currX <= xPos + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[0];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos + blockSize) && currY <= (yPos + blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[1];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos - blockSize) && currX <= (xPos - blockSize) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[2];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos - blockSize) && currY <= (yPos - blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[3];
                            colorThisPixel = 1'd1;
                        end
								else
								begin
									pixelColor = 24'h000000;
									colorThisPixel = 1'd0;
								end
                    end
                default: 
                    begin
                        tetraminoIndecies[0] = 10'd0;
                        tetraminoIndecies[1] = 10'd0;
                        tetraminoIndecies[2] = 10'd0;
                        tetraminoIndecies[3] = 10'd0;
                        pixelColor = 24'h0;
                        colorThisPixel = 1'b0;
                    end
            endcase
            
        end

    3'd5: // S piece
        begin
            case(state)
                2'd0: // right side up
                    begin
                        // get the correct indecies for each block
                        tetraminoIndecies[0] = currX - xPos + (currY - yPos)*blockSize;
                        tetraminoIndecies[1] = currX - xPos + (currY - (yPos - blockSize))*blockSize;
                        tetraminoIndecies[2] = currX - (xPos - blockSize) + (currY - yPos)*blockSize; 
                        tetraminoIndecies[3] = currX - (xPos + blockSize) + (currY - (yPos - blockSize))*blockSize;
                
                        // check if the current pixel is over one of the blocks
                        if(currX >= xPos && currX <= xPos + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[0];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos - blockSize) && currY <= (yPos - blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[1];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos - blockSize) && currX <= (xPos - blockSize) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[2];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos + blockSize) && currX <= (xPos + blockSize) + blockSize && currY >= (yPos - blockSize) && currY <= (yPos - blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[3];
                            colorThisPixel = 1'd1;
                        end
								else
								begin
									pixelColor = 24'h000000;
									colorThisPixel = 1'd0;
								end
                    end
                2'd1: // sideways
                    begin
                        // get the correct indecies for each block
                        tetraminoIndecies[0] = currX - xPos + (currY - yPos)*blockSize; 
                        tetraminoIndecies[1] = currX - (xPos + blockSize) + (currY - (yPos + blockSize))*blockSize; 
                        tetraminoIndecies[2] = currX - (xPos + blockSize) + (currY - yPos)*blockSize; 
                        tetraminoIndecies[3] = currX - xPos + (currY - (yPos - blockSize))*blockSize; 
                
                        // check if the current pixel is over one of the blocks
                        if(currX >= xPos && currX <= xPos + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[0];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos + blockSize) && currX <= (xPos + blockSize) + blockSize && currY >= (yPos + blockSize) && currY <= (yPos + blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[1];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos + blockSize) && currX <= (xPos + blockSize) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[2];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos - blockSize) && currY <= (yPos - blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[3];
                            colorThisPixel = 1'd1;
                        end
								else
								begin
									pixelColor = 24'h000000;
									colorThisPixel = 1'd0;
								end
                    end
					 2'd2:
						  begin
								// get the correct indecies for each block
                        tetraminoIndecies[0] = currX - xPos + (currY - yPos)*blockSize; 
                        tetraminoIndecies[1] = currX - xPos + (currY - (yPos + blockSize))*blockSize; 
                        tetraminoIndecies[2] = currX - (xPos + blockSize) + (currY - yPos)*blockSize; 
                        tetraminoIndecies[3] = currX - (xPos - blockSize) + (currY - (yPos + blockSize))*blockSize; 
                
                        // check if the current pixel is over one of the blocks
                        if(currX >= xPos && currX <= xPos + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[0];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos + blockSize) && currY <= (yPos + blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[1];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos + blockSize) && currX <= (xPos + blockSize) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[2];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos - blockSize) && currX <= (xPos - blockSize) + blockSize && currY >= (yPos + blockSize) && currY <= (yPos + blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[3];
                            colorThisPixel = 1'd1;
                        end
								else
								begin
									pixelColor = 24'h000000;
									colorThisPixel = 1'd0;
								end
						  end
					 2'd3:
							begin
								// get the correct indecies for each block
                        tetraminoIndecies[0] = currX - xPos + (currY - yPos)*blockSize; 
                        tetraminoIndecies[1] = currX - (xPos - blockSize) + (currY - (yPos - blockSize))*blockSize; 
                        tetraminoIndecies[2] = currX - (xPos - blockSize) + (currY - yPos)*blockSize; 
                        tetraminoIndecies[3] = currX - xPos + (currY - (yPos + blockSize))*blockSize; 
                
                        // check if the current pixel is over one of the blocks
                        if(currX >= xPos && currX <= xPos + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[0];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos - blockSize) && currX <= (xPos - blockSize) + blockSize && currY >= (yPos - blockSize) && currY <= (yPos - blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[1];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos - blockSize) && currX <= (xPos - blockSize) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[2];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos + blockSize) && currY <= (yPos + blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[3];
                            colorThisPixel = 1'd1;
                        end
								else
								begin
									pixelColor = 24'h000000;
									colorThisPixel = 1'd0;
								end
							end
                default: 
                    begin
                        tetraminoIndecies[0] = 10'd0;
                        tetraminoIndecies[1] = 10'd0;
                        tetraminoIndecies[2] = 10'd0;
                        tetraminoIndecies[3] = 10'd0;
                        pixelColor = 24'h0;
                        colorThisPixel = 1'b0;
                    end
            endcase
            
        end

    3'd6: // Z piece 
        begin
            case(state)
                2'd0: // right side up
                    begin
                        // get the correct indecies for each block
                        tetraminoIndecies[0] = currX - xPos + (currY - yPos)*blockSize; 
                        tetraminoIndecies[1] = currX - xPos + (currY - (yPos - blockSize))*blockSize; 
                        tetraminoIndecies[2] = currX - (xPos - blockSize) + (currY - (yPos - blockSize))*blockSize; 
                        tetraminoIndecies[3] = currX - (xPos + blockSize) + (currY - yPos)*blockSize;
                
                        // check if the current pixel is over one of the blocks
                        if(currX >= xPos && currX <= xPos + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[0];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos - blockSize) && currY <= (yPos - blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[1];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos - blockSize) && currX <= (xPos - blockSize) + blockSize && currY >= (yPos - blockSize) && currY <= (yPos - blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[2];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos + blockSize) && currX <= (xPos + blockSize) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[3];
                            colorThisPixel = 1'd1;
                        end
								else
								begin
									pixelColor = 24'h000000;
									colorThisPixel = 1'd0;
								end
                    end
                2'd1: // sideways
                    begin
                        // get the correct indecies for each block
                        tetraminoIndecies[0] = currX - xPos + (currY - yPos)*blockSize; 
                        tetraminoIndecies[1] = currX - (xPos + blockSize) + (currY - (yPos - blockSize))*blockSize; 
                        tetraminoIndecies[2] = currX - (xPos + blockSize) + (currY - yPos)*blockSize; 
                        tetraminoIndecies[3] = currX - xPos + (currY - (yPos + blockSize))*blockSize; 
                
                        // check if the current pixel is over one of the blocks
                        if(currX >= xPos && currX <= xPos + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[0];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos + blockSize) && currX <= (xPos + blockSize) + blockSize && currY >= (yPos - blockSize) && currY <= (yPos - blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[1];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos + blockSize) && currX <= (xPos + blockSize) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[2];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos + blockSize) && currY <= (yPos + blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[3];
                            colorThisPixel = 1'd1;
                        end
								else
								begin
									pixelColor = 24'h000000;
									colorThisPixel = 1'd0;
								end
                    end
					 2'd2:
							begin
								// get the correct indecies for each block
                        tetraminoIndecies[0] = currX - xPos + (currY - yPos)*blockSize; 
                        tetraminoIndecies[1] = currX - (xPos + blockSize) + (currY - (yPos + blockSize))*blockSize; 
                        tetraminoIndecies[2] = currX - (xPos - blockSize) + (currY - yPos)*blockSize; 
                        tetraminoIndecies[3] = currX - xPos + (currY - (yPos + blockSize))*blockSize; 
                
                        // check if the current pixel is over one of the blocks
                        if(currX >= xPos && currX <= xPos + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[0];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos + blockSize) && currX <= (xPos + blockSize) + blockSize && currY >= (yPos + blockSize) && currY <= (yPos + blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[1];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos - blockSize) && currX <= (xPos - blockSize) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[2];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos + blockSize) && currY <= (yPos + blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[3];
                            colorThisPixel = 1'd1;
                        end
								else
								begin
									pixelColor = 24'h000000;
									colorThisPixel = 1'd0;
								end
							end
					 2'd3:
							begin
								// get the correct indecies for each block
                        tetraminoIndecies[0] = currX - xPos + (currY - yPos)*blockSize; 
                        tetraminoIndecies[1] = currX - (xPos - blockSize) + (currY - (yPos + blockSize))*blockSize; 
                        tetraminoIndecies[2] = currX - (xPos - blockSize) + (currY - yPos)*blockSize; 
                        tetraminoIndecies[3] = currX - xPos + (currY - (yPos - blockSize))*blockSize; 
                
                        // check if the current pixel is over one of the blocks
                        if(currX >= xPos && currX <= xPos + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[0];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos - blockSize) && currX <= (xPos - blockSize) + blockSize && currY >= (yPos + blockSize) && currY <= (yPos + blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[1];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos - blockSize) && currX <= (xPos - blockSize) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[2];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos - blockSize) && currY <= (yPos - blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[3];
                            colorThisPixel = 1'd1;
                        end
								else
								begin
									pixelColor = 24'h000000;
									colorThisPixel = 1'd0;
								end
							end
                default: 
                    begin
                        tetraminoIndecies[0] = 10'd0;
                        tetraminoIndecies[1] = 10'd0;
                        tetraminoIndecies[2] = 10'd0;
                        tetraminoIndecies[3] = 10'd0;
                        pixelColor = 24'h0;
                        colorThisPixel = 1'b0;
                    end
            endcase
            
        end

    3'd7: // box piece
        begin
            case(state)
                2'd0: // state placeholder (makes things easier when using the state input)
                    begin
                        // get the correct indecies for each block
                        tetraminoIndecies[0] = currX - xPos + (currY - yPos)*blockSize;
                        tetraminoIndecies[1] = currX - (xPos + blockSize) + (currY - yPos)*blockSize;
                        tetraminoIndecies[2] = currX - xPos + (currY - (yPos + blockSize))*blockSize;
                        tetraminoIndecies[3] = currX - (xPos + blockSize) + (currY - (yPos + blockSize))*blockSize;
                        
                        // check if the current pixel is over one of the blocks
                        if(currX >= xPos && currX <= xPos + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[0];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos + blockSize) && currX <= (xPos + blockSize) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[1];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos + blockSize) && currY <= (yPos + blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[2];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos + blockSize) && currX <= (xPos + blockSize) + blockSize && currY >= (yPos + blockSize) && currY <= (yPos + blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[3];
                            colorThisPixel = 1'd1;
                        end
								else
								begin
									pixelColor = 24'h000000;
									colorThisPixel = 1'd0;
								end
                    end
					 2'd1:
						  begin
								// get the correct indecies for each block
                        tetraminoIndecies[0] = currX - xPos + (currY - yPos)*blockSize;
                        tetraminoIndecies[1] = currX - (xPos + blockSize) + (currY - yPos)*blockSize;
                        tetraminoIndecies[2] = currX - xPos + (currY - (yPos + blockSize))*blockSize;
                        tetraminoIndecies[3] = currX - (xPos + blockSize) + (currY - (yPos + blockSize))*blockSize;
                        
                        // check if the current pixel is over one of the blocks
                        if(currX >= xPos && currX <= xPos + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[0];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos + blockSize) && currX <= (xPos + blockSize) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[1];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos + blockSize) && currY <= (yPos + blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[2];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos + blockSize) && currX <= (xPos + blockSize) + blockSize && currY >= (yPos + blockSize) && currY <= (yPos + blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[3];
                            colorThisPixel = 1'd1;
                        end
								else
								begin
									pixelColor = 24'h000000;
									colorThisPixel = 1'd0;
								end
						  end
					 2'd2:
						  begin
								// get the correct indecies for each block
                        tetraminoIndecies[0] = currX - xPos + (currY - yPos)*blockSize;
                        tetraminoIndecies[1] = currX - (xPos + blockSize) + (currY - yPos)*blockSize;
                        tetraminoIndecies[2] = currX - xPos + (currY - (yPos + blockSize))*blockSize;
                        tetraminoIndecies[3] = currX - (xPos + blockSize) + (currY - (yPos + blockSize))*blockSize;
                        
                        // check if the current pixel is over one of the blocks
                        if(currX >= xPos && currX <= xPos + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[0];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos + blockSize) && currX <= (xPos + blockSize) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[1];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos + blockSize) && currY <= (yPos + blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[2];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos + blockSize) && currX <= (xPos + blockSize) + blockSize && currY >= (yPos + blockSize) && currY <= (yPos + blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[3];
                            colorThisPixel = 1'd1;
                        end
								else
								begin
									pixelColor = 24'h000000;
									colorThisPixel = 1'd0;
								end
						  end
					 2'd3:
						  begin
								// get the correct indecies for each block
                        tetraminoIndecies[0] = currX - xPos + (currY - yPos)*blockSize;
                        tetraminoIndecies[1] = currX - (xPos + blockSize) + (currY - yPos)*blockSize;
                        tetraminoIndecies[2] = currX - xPos + (currY - (yPos + blockSize))*blockSize;
                        tetraminoIndecies[3] = currX - (xPos + blockSize) + (currY - (yPos + blockSize))*blockSize;
                        
                        // check if the current pixel is over one of the blocks
                        if(currX >= xPos && currX <= xPos + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[0];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos + blockSize) && currX <= (xPos + blockSize) + blockSize && currY >= yPos && currY <= yPos + blockSize)
                        begin 
                            pixelColor = RGBblock[1];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= xPos && currX <= xPos + blockSize && currY >= (yPos + blockSize) && currY <= (yPos + blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[2];
                            colorThisPixel = 1'd1;
                        end
                        else if(currX >= (xPos + blockSize) && currX <= (xPos + blockSize) + blockSize && currY >= (yPos + blockSize) && currY <= (yPos + blockSize) + blockSize)
                        begin 
                            pixelColor = RGBblock[3];
                            colorThisPixel = 1'd1;
                        end
								else
								begin
									pixelColor = 24'h000000;
									colorThisPixel = 1'd0;
								end
						  end
                default: 
                    begin
                        tetraminoIndecies[0] = 10'd0;
                        tetraminoIndecies[1] = 10'd0;
                        tetraminoIndecies[2] = 10'd0;
                        tetraminoIndecies[3] = 10'd0;
                        pixelColor = 24'h0;
                        colorThisPixel = 1'b0;
                    end
            endcase
        end

    default:
        begin
            tetraminoIndecies[0] = 10'd0;
            tetraminoIndecies[1] = 10'd0;
            tetraminoIndecies[2] = 10'd0;
            tetraminoIndecies[3] = 10'd0;
            pixelColor = 24'h0;
            colorThisPixel = 1'b0;
        end
endcase
end



endmodule
