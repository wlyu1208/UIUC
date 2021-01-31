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

/* Citation: 
	Used color for blocks from https://tetris.fandom.com/wiki/Tetromino
	Found RGB values of colors from https://www.rapidtables.com/web/color/RGB_Color.html*/
	
module  color_mapper ( input              is_ball,            // Whether current pixel belongs to ball or background (computed in ball.sv)
                       input        [9:0] DrawX, DrawY,       // Current pixel coordinates
                       input			[2:0] block_color, 
							  output logic [7:0] VGA_R, VGA_G, VGA_B // VGA RGB output
                     );
    
    logic [7:0] Red, Green, Blue;
    
    // Output colors to VGA
    assign VGA_R = Red;
    assign VGA_G = Green;
    assign VGA_B = Blue;
	 
	 always_comb
	 begin
	 	Red = 8'h00;
		Green = 8'h00;
		Blue = 8'h00;
		if(is_ball)
			begin
				Red = 8'h00;
				Green = 8'hff;
				Blue = 8'hff;
			end
		else if(DrawX<=10'd160 | DrawX>=10'd480)
			begin
				Red = 8'h00;
				Green = 8'h00;
				Blue = 8'h00;	
			end
		else if(DrawX>=10'd160 | DrawX<=10'd480)
			begin
				Red = 8'hff;
				Green = 8'hff;
				Blue = 8'hff;
			end
		else
			begin
			case(block_color)
				3'b000: // I block skyblue color
					begin
					Red = 8'h00;
					Green = 8'hff;
					Blue = 8'hff;
					end
				3'b001: // J block navy color
					begin
					Red = 8'h00;
					Green = 8'h00;
					Blue = 8'h99;
					end
				3'b010: // L block orange color
					begin
					Red = 8'hff;
					Green = 8'h80;
					Blue = 8'h00;
					end
				3'b011: // O block yellow color
					begin
					Red = 8'hff;
					Green = 8'hff;
					Blue = 8'h00;
					end
				3'b100: // S block light green color
					begin
					Red = 8'h00;
					Green = 8'hff;
					Blue = 8'h00;
					end
				3'b101: // T block purple color
					begin
					Red = 8'h99;
					Green = 8'h00;
					Blue = 8'h99;
					end
				3'b110: // Z block red color
					begin
					Red = 8'hff;
					Green = 8'h00;
					Blue = 8'h00;
					end
				default: ;
				endcase
			end
	 end    
endmodule
