//-------------------------------------------------------------------------
//    Color_Mapper.sv                                                    --
//    Stephen Kempf                                                      --
//    3-1-06                                                             --
//                                                                       --
//    Modified by David Kesler  07-16-2008                               --
//    Translated by Joe Meng    07-07-2013                               --
//    Modified by Po-Han Huang  10-06-2017                               --
//                                                                       --
//    Fall 2017 Distribution                                             --
//                                                                       --
//    For use with ECE 385 Lab 8                                         --
//    University of Illinois ECE Department                              --
//-------------------------------------------------------------------------

// color_mapper: Decide which color to be output to VGA for each pixel.
module  color_mapper ( input              clk,         // Whether current pixel belongs to ball 
								input logic 		[9:0] ballX, ballY,
                                                              //   or background (computed in ball.sv)
                       input        [9:0] DrawX, DrawY,       // Current pixel coordinates
                       output logic [7:0] VGA_R, VGA_G, VGA_B // VGA RGB output
                     );
							
	 // 640x480 display resolution
    
    logic [7:0] Red, Green, Blue;
	 logic [23:0] RGBI;
	 logic [23:0] RGBJ;
	 logic [9:0] colorIndexI = 10'b0;
	 logic [9:0] colorIndexJ = 10'b0;
    
    // Output colors to VGA
    assign VGA_R = Red;
    assign VGA_G = Green;
    assign VGA_B = Blue;
	 
	 parameter blockSize = 10'd16;
	 logic [9:0] IblockX;
	 logic [9:0] IblockY;
	 logic [9:0] JblockX;
	 logic [9:0] JblockY;
	 
	 assign IblockX = ballX;
	 assign IblockY = ballY;
	 assign JblockX = 10'd320;
	 assign JblockY = 10'd240; 

	 assign colorIndexI = DrawX - IblockX + (DrawY - IblockY)*blockSize;
	 assign colorIndexJ = DrawX - JblockX + (DrawY - JblockY)*blockSize;
	 
	 BlockColor colorI(.Clk(clk),.read_address(colorIndexI),.data_Out(RGBI),.blockType(3'd1));
	 BlockColor colorJ(.Clk(clk),.read_address(colorIndexJ),.data_Out(RGBJ),.blockType(3'd3));
	 
	 

    // Assign color based on is_ball signal
    always_comb
    begin
		  if(DrawX <= 10'd160)
		  begin
				Red = 8'h00;
				Green = 8'h00;
				Blue = 8'h00;
		  end
		  else if(DrawX >= 10'd480)
		  begin
				Red = 8'h00;
				Green = 8'h00;
				Blue = 8'h00;
		  end
		  
		  else if(DrawX >= IblockX && DrawX <= IblockX+blockSize && DrawY >= IblockY && DrawY <= IblockY+blockSize)
		  begin
				Red = RGBI[23:16];
				Green = RGBI[15:8];
				Blue = RGBI[7:0];
		  end
		  
		  else if(DrawX >= JblockX && DrawX <= JblockX+blockSize && DrawY >= JblockY && DrawY <= JblockY+blockSize)
		  begin
				Red = RGBJ[23:16];
				Green = RGBJ[15:8];
				Blue = RGBJ[7:0];
		  end
		  
		  else
        begin
            // Background with nice color gradient
            Red = 8'h3f; 
            Green = 8'h00;
            Blue = 8'h7f - {1'b0, DrawX[9:3]};
        end
    end 
    
endmodule
