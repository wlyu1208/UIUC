transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -sv -work work +incdir+D:/ECE385/Lab5 {D:/ECE385/Lab5/Control.sv}
vlog -sv -work work +incdir+D:/ECE385/Lab5 {D:/ECE385/Lab5/HexDriver.sv}
vlog -sv -work work +incdir+D:/ECE385/Lab5 {D:/ECE385/Lab5/reg_unit.sv}
vlog -sv -work work +incdir+D:/ECE385/Lab5 {D:/ECE385/Lab5/flip_flop.sv}
vlog -sv -work work +incdir+D:/ECE385/Lab5 {D:/ECE385/Lab5/nineadder.sv}
vlog -sv -work work +incdir+D:/ECE385/Lab5 {D:/ECE385/Lab5/sync.sv}
vlog -sv -work work +incdir+D:/ECE385/Lab5 {D:/ECE385/Lab5/processor.sv}

vlog -sv -work work +incdir+D:/ECE385/Lab5 {D:/ECE385/Lab5/testbench.sv}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L cycloneive_ver -L rtl_work -L work -voptargs="+acc"  testbench

add wave *
view structure
view signals
run 1000 ns
