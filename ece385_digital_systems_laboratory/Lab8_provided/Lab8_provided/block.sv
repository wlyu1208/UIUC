module block (input Clk, Reset, frame_clk,
				  input[9:0] DrawX, DrawY,
				  output logic is_block,
				  input[7:0] keycode);

logic frame_clk_delayed, frame_clk_rising_edge;
always_ff @ (posedge Clk) begin
	frame_clk_delayed <= frame_clk;
   frame_clk_rising_edge <= (frame_clk == 1'b1) && (frame_clk_delayed == 1'b0);
end

parameter[9:0] block_xmin=10'd160;
parameter[9:0] block_xmax=10'd479;

parameter[9:0] block_ymin=10'd0;
parameter[9:0] block_ymax=10'd479;

parameter[9:0] block_xcenter=10'd320;
parameter[9:0] block_ycenter=10'd240;

parameter[9:0] block_xstep=10'd16;
parameter[9:0] block_ystep=10'd1;

parameter[9:0] block_size=10'd10;

parameter[9:0] block_xstart=10'd320;
parameter[9:0] block_ystart=10'd450;

logic[9:0] block_xpos, block_ypos, block_xmotion, block_ymotion;
logic[9:0] block_xposin, block_yposin, block_xmotonin, block_ymotionin;

always_ff @(posedge Clk)
begin
	if(Reset)
	begin
		block_xpos<=block_xstart;
		block_ypos<=block_ystart;
		block_xmotion<=10'd0;
		block_ymotion<=block_ystep;
	end
	else
	begin
		block_xpos<=block_xposin;
		block_ypos<=block_yposin;
		block_xmotion<=block_xposin;
		block_ymotion<=block_yposin;
	end
end

always_comb
begin
	block_xposin=ball_xpos;
	block_yposin=block_ypos;
	block_xmotionin=block_xmotion;
	block_ymotionin=block_ymotion;
	
	if(fram_clk_rising_edge)
	begin
		case(keycode) 
		8'h50: //left
			begin
			block_ymotionin=10'd0;
			block_xmotionin=(~(block_xstep)+1'b1);
			end
		8'h4f: //right
			begin
			block_ymotionin=10'd0;
			block_xmotionin=block_xstep;
			end
		8'h51: //down
			begin
			block_xmotionin=10'd0;
			block_ymotionin=block_ystep;
			end
		default:
			begin
				block_xmotionin=block_xmotion;
				block_ymotionin=block_ymotion;
			end
		endcase
	//need boundary check
	
	block_xposin=block_xpos+block_xmotion; //update block position
	block_yposin=block_ypos+block_ymotion;
	end
end

int distx, disty, size;
assign distx=DrawX=block_xpos;
assign disty=DrawY=block_ypos;
always_comb
begin
	if( (distx*distx+disty*disty) <=(size*size) )
		is_block=1'b1;
	else
		is_block=1'b0;
end
	
endmodule

