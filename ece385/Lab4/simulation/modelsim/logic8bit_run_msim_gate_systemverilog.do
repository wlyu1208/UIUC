transcript on
if {[file exists gate_work]} {
	vdel -lib gate_work -all
}
vlib gate_work
vmap work gate_work

vlog -sv -work work +incdir+. {logic8bit_7_1200mv_85c_slow.svo}

vlog -sv -work work +incdir+D:/ECE385/Lab4/logic_processor_4bit {D:/ECE385/Lab4/logic_processor_4bit/testbench_8.sv}

vsim -t 1ps +transport_int_delays +transport_path_delays -L altera_ver -L cycloneive_ver -L gate_work -L work -voptargs="+acc"  testbench

add wave *
view structure
view signals
run -all
