/************************************************************************
Avalon-MM Interface for AES Decryption IP Core

Dong Kai Wang, Fall 2017

For use with ECE 385 Experiment 9
University of Illinois ECE Department

Register Map:

 0-3 : 4x 32bit AES Key
 4-7 : 4x 32bit AES Encrypted Message
 8-11: 4x 32bit AES Decrypted Message
   12: Not Used
	13: Not Used
   14: 32bit Start Register
   15: 32bit Done Register

************************************************************************/

module avalon_aes_interface (
	// Avalon Clock Input
	input logic CLK,
	
	// Avalon Reset Input
	input logic RESET,
	
	// Avalon-MM Slave Signals
	input  logic AVL_READ,					// Avalon-MM Read
	input  logic AVL_WRITE,					// Avalon-MM Write
	input  logic AVL_CS,						// Avalon-MM Chip Select
	input  logic [3:0] AVL_BYTE_EN,		// Avalon-MM Byte Enable
	input  logic [3:0] AVL_ADDR,			// Avalon-MM Address
	input  logic [31:0] AVL_WRITEDATA,	// Avalon-MM Write Data
	output logic [31:0] AVL_READDATA,	// Avalon-MM Read Data
	
	// Exported Conduit
	output logic [31:0] EXPORT_DATA		// Exported Conduit Signal to LEDs
);
logic[31:0] key0, key1, key2, key3;
logic[31:0] enc0, enc1, enc2, enc3;
logic[31:0] dec0, dec1, dec2, dec3;

logic[31:0] key0_d, key1_d, key2_d, key3_d;
logic[31:0] enc0_d, enc1_d, enc2_d, enc3_d;
logic[31:0] dec0_d, dec1_d, dec2_d, dec3_d;

logic[31:0] start0, done0, temp_w;
logic[31:0] start_d, done_d;

logic[127:0] result;
logic finish;

always_comb
begin
	temp_w=32'd0;
	if(AVL_WRITE & AVL_CS)
		begin
		if(AVL_BYTE_EN[3])
			temp_w[31:24]=AVL_WRITEDATA[31:24];
		if(AVL_BYTE_EN[2])
			temp_w[23:16]=AVL_WRITEDATA[23:16];
		if(AVL_BYTE_EN[1])
			temp_w[15:8]=AVL_WRITEDATA[15:8];
		if(AVL_BYTE_EN[0])
			temp_w[7:0]=AVL_WRITEDATA[7:0];
		end
end

always_comb
begin
key0=key0_d;
enc0=enc0_d;
dec0=dec0_d;

key1=key1_d;
enc1=enc1_d;
dec1=dec1_d;

key2=key2_d;
enc2=enc2_d;
dec2=dec2_d;

key3=key3_d;
enc3=enc3_d;
dec3=dec3_d;

start0=start_d;
done0=done_d;

if(AVL_CS && AVL_WRITE)
	begin
	case(AVL_ADDR)
	4'h0: key0=temp_w;
	4'h1: key1=temp_w;
	4'h2: key2=temp_w;
	4'h3: key3=temp_w;
	4'h4: enc0=temp_w;
	4'h5: enc1=temp_w;
	4'h6: enc2=temp_w;
	4'h7: enc3=temp_w;
	4'h8: dec0=result[31:0];
	4'h9: dec1=result[63:32];
	4'ha:	dec2=result[95:64];
	4'hb:	dec3=result[127:96];
	4'he: start0=temp_w;
	endcase
	end
else if(start_d[0] | finish)
begin
dec3=result[31:0];
dec2=result[63:32];
dec1=result[95:64];
dec0=result[127:96];
end
	if(finish)
		done0=32'd1;
end

always_ff @(posedge CLK)
begin
key0_d<=key0;
enc0_d<=enc0;
dec0_d<=dec0;

key1_d<=key1;
enc1_d<=enc1;
dec1_d<=dec1;

key2_d<=key2;
enc2_d<=enc2;
dec2_d<=dec2;

key3_d<=key3;
enc3_d<=enc3;
dec3_d<=dec3;

start_d<=start0;
done_d<=done0;
end

always_comb
begin
AVL_READDATA=32'haaaa;
if(AVL_CS && AVL_READ)
	begin
	case(AVL_ADDR)
	4'h0: AVL_READDATA=key0_d;
	4'h1: AVL_READDATA=key1_d;
	4'h2: AVL_READDATA=key2_d;
	4'h3: AVL_READDATA=key3_d;
	4'h4: AVL_READDATA=enc0_d;
	4'h5: AVL_READDATA=enc1_d;
	4'h6: AVL_READDATA=enc2_d;
	4'h7: AVL_READDATA=enc3_d;
	4'h8: AVL_READDATA=dec0_d;
	4'h9: AVL_READDATA=dec1_d;
	4'ha: AVL_READDATA=dec2_d;
	4'hb: AVL_READDATA=dec3_d;
	4'he: AVL_READDATA=start_d;
	4'hf: AVL_READDATA=done_d;
	endcase
	end
end

assign EXPORT_DATA={dec0_d[31:16], dec3_d[15:0]};

AES aes(.CLK(CLK), .RESET(RESET), .AES_START(start_d[0]), .AES_DONE(finish), 
	     .AES_KEY({key0_d, key1_d, key2_d, key3_d}), 
		  .AES_MSG_ENC({enc0_d, enc1_d, enc2_d, enc3_d}), 
		  .AES_MSG_DEC(result) );

endmodule

