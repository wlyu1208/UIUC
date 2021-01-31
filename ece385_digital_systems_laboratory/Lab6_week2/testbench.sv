module testbench();

timeunit 10ns;	// Half clock cycle at 50 MHz
			// This is the amount of time represented by #1 
timeprecision 1ns;


//logic [15:0] S;
//logic Clk, Reset, Run, Continue;
//logic [11:0] LED;
//logic [6:0] HEX0, HEX1, HEX2, HEX3, HEX4, HEX5, HEX6, HEX7;
//logic CE, UB, LB, OE, WE;
//logic [19:0] ADDR;
//wire [15:0] Data;
//
//slc3 scl0(.*);

logic [15:0] S;
logic Clk, Reset, Run, Continue;
logic [11:0] LED;
logic [6:0] HEX0, HEX1, HEX2, HEX3, HEX4, HEX5, HEX6, HEX7;
logic CE, UB, LB, OE, WE;
logic [19:0] ADDR;
wire [15:0] Data;


lab6_toplevel L6(.*);


//logic Clk, Reset, GateMARMUX, GatePC, GateALU, GateMDR, LD_MAR, LD_MDR, LD_PC, LD_IR, mio;
//logic[1:0] pc_s;
//logic[15:0] MDR_INS;
//logic[15:0] mar_t, ir_t, mdr_t;
//datapath d0(.*);


always begin : CLOCK_GENERATION
#1 Clk = ~Clk;
end

initial begin: CLOCK_INITIALIZATION
    Clk = 0;
end 

// Testing begins here
// The initial block is not synthesizable
// Everything happens sequentially inside an initial block
// as in a software program
initial begin: TEST_VECTORS
//Reset=0;		//datapath testbench
//LD_MAR = 1'b0;
//LD_MDR = 1'b0;
//LD_IR = 1'b0;
//LD_PC = 1'b0;
//
//GatePC = 1'b0;
//GateMDR = 1'b0;
//GateALU = 1'b0;
//GateMARMUX = 1'b0;
//
//pc_s = 2'b00;
//MDR_INS = 1'b0;
//
//#2 Reset=1;
//
//#2 GatePC = 1'b1;
//#2 LD_MAR = 1'b1;
//#2 pc_s = 2'b00;
//#2 LD_PC = 1'b1;
//#2 
//#2 MDR_INS=1'b0;
//#2 
//#2 MDR_INS=1'b0;
//#2 LD_MDR=1'b1;
//#2 
//#2 GateMDR = 1'b1;
//#2 LD_IR = 1'b1;

Reset=0;
Run=1;
Continue=1;


#2 Reset=1;
#2 Run=0;
#2 Run=1;
S=16'h0002;
#22 Continue=0;
#22 Continue=1;

#22 Continue=0;
#22 Continue=1;

end

endmodule
