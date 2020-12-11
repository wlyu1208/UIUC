module testbench();

timeunit 10ns;	// Half clock cycle at 50 MHz
			// This is the amount of time represented by #1 
timeprecision 1ns;

// These signals are internal because the processor will be 
// instantiated as a submodule in testbench.
logic clk = 0;
logic Reset, ClrA_LdB, Run;
logic [7:0] Sw;
logic [7:0] Aval,
		 Bval;
logic [6:0] AhexL,
		 AhexU,
		 BhexL,
		 BhexU;
logic X; 

// To store expected results
logic [7:0] ans_1a;
				
// A counter to count the instances where simulation results
// do no match with expected results
integer ErrorCnt = 0;
		
// Instantiating the DUT
// Make sure the module and signal names match with those in your design
processor processor0(.*);	

// Toggle the clock
// #1 means wait for a delay of 1 timeunit
always begin : CLOCK_GENERATION
#1 clk = ~clk;
end

initial begin: CLOCK_INITIALIZATION
    clk = 0;
end 

// Testing begins here
// The initial block is not synthesizable
// Everything happens sequentially inside an initial block
// as in a software program
initial begin: TEST_VECTORS
Reset = 0;		// Toggle Rest
ClrA_LdB=1;
Run=1;
Sw = 8'hC5;	// Specify Din, F, and R

#2 Reset = 1;

#2 ClrA_LdB = 0;	// Toggle LoadA
#2 ClrA_LdB = 1;

#2 Sw=8'h07;  //Change Din again

#2 Run = 0;	// Toggle Execute
   
#22 Run = 1;
    ans_1a = 8'hFE; // Expected result of 1st cycle
    // Aval is expected to be 8’h33 XOR 8’h55
    // Bval is expected to be the original 8’h55
    if (Aval != ans_1a)
	 ErrorCnt++;
    if (Bval != 8'h63)
	 ErrorCnt++;


if (ErrorCnt == 0)
	$display("Success!");  // Command line output in ModelSim
else
	$display("%d error(s) detected. Try again!", ErrorCnt);
end
endmodule
