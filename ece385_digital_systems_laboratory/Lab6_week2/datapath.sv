module datapath(input logic Clk, Reset, GateMARMUX, GatePC, GateALU, GateMDR, LD_MAR, LD_MDR, LD_PC, LD_IR, mio, LD_CC, LD_REG, LD_BEN,
					 input logic addr1mux, sr2_selec, drmux, sr1mux, LD_LED,
					 input logic[1:0] pc_s, addr2mux, ALUK,
					 input logic[15:0] MDR_INS, 
					 output logic[11:0] LED,
					 output logic[15:0] ir_t, mdr_t, pc_t, mar_t,
					 output logic Ben);
	logic[15:0] pc_mout, mdr_mout, ALU_t, add_mux, sr1, addr1_out, sign11_out, sign9_out, sign6_out, addr2_out;
	logic[15:0] sign5_out, sr2, sr2_mout;
	logic[2:0]  dr_mout, sr1_mout;
	logic[15:0] bus;
	logic N, Z, P, N_F, Z_F, P_F;
	
four_mux pc_m(.selec(pc_s), .s0(pc_t+16'h0001), .s1(add_mux), .s2(bus), .s3(), .out(pc_mout));
two_mux mdr_m(.selec(mio), .s0(bus), .s1(MDR_INS), .out(mdr_mout));

flip_flop  PC0(.*, .D(pc_mout), .LD(LD_PC), .Q(pc_t));
flip_flop MDR0(.*, .D(mdr_mout), .LD(LD_MDR), .Q(mdr_t));
flip_flop MAR0(.*, .D(bus), .LD(LD_MAR), .Q(mar_t));
flip_flop  IR0(.*, .D(bus), .LD(LD_IR), .Q(ir_t));


nzp_logic NZP0(.bus(bus), .n(N), .z(Z), .p(P));
ff_3bit nzp_ff(.*, .LD(LD_CC), .n(N), .z(Z), .p(P), .n_f(N_F), .z_f(Z_F), .p_f(P_F));
ben_ff ben0(.*, .N(N_F), .Z(Z_F), .P(P_F), .LD(LD_BEN), .ir(ir_t[11:9]), .ben(Ben));


two_mux addr1(.selec(addr1mux), .s0(pc_t), .s1(sr1), .out(addr1_out));

sext11 sign11(.A(ir_t[10:0]), .out(sign11_out));
sext8 sign8(.A(ir_t[8:0]), .out(sign9_out));
sext6 sign6(.A(ir_t[5:0]), .out(sign6_out));
four_mux addr2(.selec(addr2mux), .s0(sign11_out), .s1(sign9_out), .s2(sign6_out), .s3(16'h0000), .out(addr2_out));

sixteen_adder addtwo(.A(addr1_out), .B(addr2_out), .S(add_mux));

sext5 sign5(.A(ir_t[4:0]), .out(sign5_out));
two_mux sr2_m(.selec(sr2_selec), .s0(sr2), .s1(sign5_out), .out(sr2_mout));

ALU choose(.A(sr1), .B(sr2_mout), .Sel(ALUK), .out(ALU_t));

two_mux drm(.selec(drmux), .s0(ir_t[11:9]), .s1(3'b111), .out(dr_mout));
two_mux sr1m(.selec(sr1mux), .s0(ir_t[8:6]), .s1(ir_t[11:9]), .out(sr1_mout));
Reg_f regfile(.*, .LD_REG(LD_REG), .in(bus), .sr2(ir_t[2:0]), .dr_m(dr_mout), .sr1_m(sr1_mout), .sr2_out(sr2), .sr1_out(sr1));

LED_12 LED_O(.*, .D(ir_t[11:0]), .LD(LD_LED), .Q(LED));

	always_comb
	begin
	if(GatePC==1'b1)
		bus=pc_t;
	else if(GateMDR==1'b1)
		bus=mdr_t;
	else if(GateALU==1'b1)
		bus=ALU_t;
	else if(GateMARMUX==1'b1)
		bus=add_mux;
	else
		bus=0;
	end

endmodule
