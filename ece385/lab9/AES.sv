/************************************************************************
AES Decryption Core Logic

Dong Kai Wang, Fall 2017

For use with ECE 385 Experiment 9
University of Illinois ECE Department
************************************************************************/

module AES (
	input	 logic CLK,
	input  logic RESET,
	input  logic AES_START,
	output logic AES_DONE,
	input  logic [127:0] AES_KEY,
	input  logic [127:0] AES_MSG_ENC,
	output logic [127:0] AES_MSG_DEC
);
logic[1407:0] key_s;
logic[127:0] in_state, out_state;
logic[127:0] invshift_t, invsub_t, invcol_t, round_t;
logic [4:0] selec_f; 
// 10000 expansion 01000 invshift 00100 invsub 00010 round 00001 invmic
logic[3:0] count, colcount;
logic[31:0] invcol_in, invcol_out;

assign AES_MSG_DEC=in_state;

always_ff @ (posedge CLK or posedge RESET)
begin
if(RESET)
	in_state<=AES_MSG_ENC;
else
	in_state<=out_state;
end

KeyExpansion expandkey(.clk(CLK), .Cipherkey(AES_KEY), .KeySchedule(key_s));
state state0(.CLK(CLK), .RESET(RESET), .aes_start(AES_START), 
			    .aes_done(AES_DONE), .f_selec(selec_f), .roundcount(count), 
				 .colcount(colcount));
InvShiftRows invshiftrows0(.data_in(in_state), .data_out(invshift_t));
invsubact subact0(.CLK(CLK), .in(in_state), .out(invsub_t));
roundkeyact keyact0(.in(in_state), .schedule(key_s), .index(count), .out(round_t));
InvMixColumns invmix0(.in(invcol_in), .out(invcol_out));

always_comb
begin
out_state=in_state;
invcol_in=32'h0000;
if(selec_f==5'b10000)
	out_state=AES_MSG_ENC;
else if(selec_f==5'b01000)
	out_state=invshift_t;
else if(selec_f==5'b00100)
	out_state=invsub_t;
else if(selec_f==5'b00010)
	out_state=round_t;
else if(selec_f==5'b00001)
	if(colcount==4'b1000)
		begin
		invcol_in=in_state[127:96];
		out_state[127:96]=invcol_out;
		end
	else if(colcount==4'b0100)
		begin
		invcol_in=in_state[95:64];
		out_state[95:64]=invcol_out;
		end
	else if(colcount==4'b0010)
		begin
		invcol_in=in_state[63:32];
		out_state[63:32]=invcol_out;
		end
	else if(colcount==4'b0001)
		begin
		invcol_in=in_state[31:0];
		out_state[31:0]=invcol_out;
		end
	else
		out_state=out_state;
else
	out_state=out_state;
end
endmodule

module roundkeyact(
	input logic[127:0] in,
	input logic[1407:0] schedule,
	input logic[3:0] index,
	output logic[127:0] out);
	always_comb 
	begin
	out=in;
	if(index==4'b0000)
		out=in^schedule[127:0];
	else if(index==4'b0001)
		out=in^schedule[255:128];
	else if(index==4'b0010)
		out=in^schedule[383:256];
	else if(index==4'b0011)
		out=in^schedule[511:384];
	else if(index==4'b0100)
		out=in^schedule[639:512];
	else if(index==4'b0101)
		out=in^schedule[767:640];
	else if(index==4'b0110)
		out=in^schedule[895:768];
	else if(index==4'b0111)
		out=in^schedule[1023:896];
	else if(index==4'b1000)
		out=in^schedule[1151:1024];
	else if(index==4'b1001)
		out=in^schedule[1279:1152];
	else if(index==4'b1010)
		out=in^schedule[1407:1280];
	else
		out=out;
	end
endmodule

module invsubact(
	input logic CLK,
	input logic[127:0] in,
	output logic[127:0] out);
	InvSubBytes sub0(.clk(CLK), .in(in[7:0]),     .out(out[7:0]));
	InvSubBytes sub1(.clk(CLK), .in(in[15:8]),    .out(out[15:8]));
	InvSubBytes sub2(.clk(CLK), .in(in[23:16]),   .out(out[23:16]));
	InvSubBytes sub3(.clk(CLK), .in(in[31:24]),   .out(out[31:24]));
	InvSubBytes sub4(.clk(CLK), .in(in[39:32]),   .out(out[39:32]));
	InvSubBytes sub5(.clk(CLK), .in(in[47:40]),   .out(out[47:40]));
	InvSubBytes sub6(.clk(CLK), .in(in[55:48]),   .out(out[55:48]));
	InvSubBytes sub7(.clk(CLK), .in(in[63:56]),   .out(out[63:56]));
	InvSubBytes sub8(.clk(CLK), .in(in[71:64]),   .out(out[71:64]));
	InvSubBytes sub9(.clk(CLK), .in(in[79:72]),   .out(out[79:72]));
	InvSubBytes suba(.clk(CLK), .in(in[87:80]),   .out(out[87:80]));
	InvSubBytes subb(.clk(CLK), .in(in[95:88]),   .out(out[95:88]));
	InvSubBytes subc(.clk(CLK), .in(in[103:96]),  .out(out[103:96]));
	InvSubBytes subd(.clk(CLK), .in(in[111:104]), .out(out[111:104]));
	InvSubBytes sube(.clk(CLK), .in(in[119:112]), .out(out[119:112]));
	InvSubBytes subf(.clk(CLK), .in(in[127:120]), .out(out[127:120]));
endmodule

module state ( 
	input logic CLK,
	input logic RESET,
	input logic aes_start,
	output logic[4:0] f_selec,
	output logic aes_done,
	output logic[3:0] roundcount, 
	output logic[3:0] colcount
);

logic c_reset, c_ld;
logic[3:0] c_out;

enum logic[3:0] {WAIT, DONE, keyexpand, firstroundkey, addroundkey, invshiftrows, invsubbytes, 
					 invmixcolumns0, invmixcolumns1, invmixcolumns2, invmixcolumns3} present, next_state;
ff counter(.CLK(CLK), .ff_reset(c_reset), .load(c_ld), .out(c_out));

always_ff @ (posedge CLK)
begin
if(RESET)
	present<=WAIT;
else
	present<=next_state;
end

always_comb
begin
next_state=present;

aes_done=1'b0;
c_reset=1'b0;
c_ld=1'b0;
c_reset=1'b0;
f_selec=5'b00000;
roundcount=4'b0000;
colcount=4'b0000;

unique case(present)
	WAIT :
		if(aes_start==1'b1)
			next_state=keyexpand;
		else
			next_state=WAIT;
	DONE :
		if(aes_start==1'b0)
			next_state=WAIT;
		else
			next_state=DONE;
	keyexpand :
		if(c_out==4'b1010)
			next_state=firstroundkey;
	firstroundkey :
		next_state=invshiftrows;
	invshiftrows :
		next_state=invsubbytes;
	invsubbytes :
		next_state=addroundkey;
	addroundkey :
		if(c_out==4'b1010)
			next_state=DONE;
		else
			next_state=invmixcolumns0;
	invmixcolumns0 :
		next_state=invmixcolumns1;
	invmixcolumns1 :
		next_state=invmixcolumns2;
	invmixcolumns2 :
		next_state=invmixcolumns3;
	invmixcolumns3 :
		next_state=invshiftrows;
	default :
		next_state=WAIT;
endcase

case(present)
	WAIT :
		begin
		c_reset=1'b1;
		end
	DONE :
		begin
		aes_done=1'b1;
		end
	keyexpand :
		begin
			c_ld=1'b1;
			f_selec=5'b10000;
		end
	firstroundkey :
		begin
			c_reset=1'b1;
			f_selec=5'b00010;
			roundcount=4'b0000;
		end
	invshiftrows :
		begin
			c_reset=1'b0;
			c_ld=1'b1;
			f_selec=5'b01000;
		end
	invsubbytes :
		begin
		f_selec=5'b00100;
		end
	addroundkey :
		begin
		roundcount=c_out;
		f_selec=5'b00010;
		end
	invmixcolumns0 :
		begin
		f_selec=5'b00001;
		colcount=4'b0001;
		end
	invmixcolumns1 :
		begin
		f_selec=5'b00001;
		colcount=4'b0010;
		end
	invmixcolumns2 :
		begin
		f_selec=5'b00001;
		colcount=4'b0100;
		end
	invmixcolumns3 :
		begin
		f_selec=5'b00001;
		colcount=4'b1000;
		end
	default : ;
		
	endcase
end
endmodule

module ff( input logic CLK,
			  input logic ff_reset,
		     input logic load,
			  output logic[3:0] out  
);
always_ff @ (posedge CLK or posedge ff_reset)
begin
	if(ff_reset)
		out<=4'b0000;
	else if(load==1'b1)
		out<=out+4'b0001;
	else
		out<=out;
		
end
endmodule
