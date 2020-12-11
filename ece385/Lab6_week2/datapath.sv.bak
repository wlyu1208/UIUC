module datapath(input logic Clk, Reset, GateMARMUX, GatePC, GateALU, GateMDR, LD_MAR, LD_MDR, LD_PC, LD_IR, mio,
					 input logic[1:0] pc_s,
					 input logic[15:0] MDR_INS, 
					 output logic[15:0] ir_t, mdr_t, pc_t, mar_t,
					 output logic[19:0] mar_z);
	logic[15:0] pc_mout, mdr_mout, ALU_t;
	logic[15:0] bus;
	
four_mux pc_m(.selec(pc_s), .s0(pc_t+16'h0001), .s1(), .s2(), .s3(), .out(pc_mout));
two_mux mdr_m(.selec(mio), .s0(bus), .s1(MDR_INS), .out(mdr_mout));

flip_flop  PC0(.*, .D(pc_mout), .LD(LD_PC), .Q(pc_t));
flip_flop MDR0(.*, .D(mdr_mout), .LD(LD_MDR), .Q(mdr_t));
flip_flop MAR0(.*, .D(bus), .LD(LD_MAR), .Q(mar_t));
flip_flop  IR0(.*, .D(bus), .LD(LD_IR), .Q(ir_t));

	
	always_comb
	begin
	if(GatePC==1'b1)
		bus=pc_t;
	else if(GateMDR==1'b1)
		bus=mdr_t;
	else
		bus=0;
	end

endmodule
