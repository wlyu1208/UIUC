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
module  color_mapper (input clk,         // Whether current pixel belongs to ball 
                      input        [9:0] DrawX, DrawY,       // Current pixel coordinates
							 input 				  drawTetramino, drawGhost,
							 input		 [23:0] TetraminoRGB, ghostRGB,
							 input		 [23:0] gridRGB,
							 input 		 [9:0]  minBlockPixX, minBlockPixY, maxBlockPixX, maxBlockPixY,
                      output logic [7:0] VGA_R, VGA_G, VGA_B // VGA RGB output
                     );
							
	 // 640x480 display resolution
    
   logic [7:0] Red, Green, Blue;
	logic [14:0] radius;
	logic delay;
	
	backgroundCounter count(.clk, .maxVal(15'd13000), .radius, .delay);
	delayGenerator delay0(.clk, .offset(30'd7000000), .delaySig(delay));

    
    // Output colors to VGA
    assign VGA_R = Red;
    assign VGA_G = Green;
    assign VGA_B = Blue;

    // Assign color based on is_ball signal
    always_comb
    begin
	 
		  if(drawTetramino)
		  begin
				Red = TetraminoRGB[23:16];
				Green = TetraminoRGB[15:8];
				Blue = TetraminoRGB[7:0];
		  end
		  
		  else if(drawGhost)
		  begin
				Red = ghostRGB[23:16];
				Green = ghostRGB[15:8];
				Blue = ghostRGB[7:0];
		  end
		  
		  else if(DrawX >= minBlockPixX && DrawX <= maxBlockPixX && DrawY >= minBlockPixY && DrawY <= maxBlockPixY)
		  begin
				Red = gridRGB[23:16];
				Green = gridRGB[15:8];
				Blue = gridRGB[7:0];
		  end
		  
		  
		  // border color 210ec9
		  else if(DrawX >= minBlockPixX - 4'd10 && DrawX < minBlockPixX && DrawY >= minBlockPixY && DrawY <= maxBlockPixY)
		  begin
				Red = 8'h21;
				Green = 8'h0e;
				Blue = 8'hc9;
		  end
		  
		  else if(DrawX >= maxBlockPixX && DrawX <= maxBlockPixX + 4'd10 && DrawY >= minBlockPixY && DrawY <= maxBlockPixY)
		  begin
				Red = 8'h21;
				Green = 8'h0e;
				Blue = 8'hc9;
		  end
		  
		  else if(DrawX >= minBlockPixX - 4'd10 && DrawX <= maxBlockPixX + 4'd10 && DrawY >= maxBlockPixY && DrawY <= maxBlockPixY + 4'd10)
		  begin
				Red = 8'h21;
				Green = 8'h0e;
				Blue = 8'hc9;
		  end
		  
		  else
        begin
            // Background with nice color gradient
            Red = (DrawX*DrawX + DrawY*DrawY - radius*radius) %   8'd255 + 8'd0;     	// 8'd104 + 8'd0;
            Green = (DrawX*DrawX + DrawY*DrawY - radius*radius) % 8'd255 + 8'd0;			// 8'd49 + 8'd8;
            Blue = (DrawX*DrawX + DrawY*DrawY - radius*radius) % 	8'd255 + 8'd0;			// 8'd249 + 8'd186;
        end
    end 
    
endmodule


module backgroundCounter(input clk, input [14:0] maxVal, input delay, output [14:0] radius);

logic [14:0] count = 0;

assign radius = count;

always_ff @ (posedge (clk & delay))
begin
	if(count == maxVal)
		count <= 0;
	else
		count <= count + 1;
end


endmodule



module delayGenerator(input clk, input [29:0] offset, output delaySig);

logic [29:0] count = 0;

always_ff @ (posedge clk)
begin
	if(count == offset)
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

