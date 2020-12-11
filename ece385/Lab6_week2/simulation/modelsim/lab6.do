onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /testbench/L6/S
add wave -noupdate /testbench/L6/Clk
add wave -noupdate /testbench/L6/Reset
add wave -noupdate /testbench/L6/Run
add wave -noupdate /testbench/L6/Continue
add wave -noupdate /testbench/L6/LED
add wave -noupdate /testbench/L6/HEX0
add wave -noupdate /testbench/L6/HEX1
add wave -noupdate /testbench/L6/HEX2
add wave -noupdate /testbench/L6/HEX3
add wave -noupdate /testbench/L6/HEX4
add wave -noupdate /testbench/L6/HEX5
add wave -noupdate /testbench/L6/HEX6
add wave -noupdate /testbench/L6/HEX7
add wave -noupdate /testbench/L6/CE
add wave -noupdate /testbench/L6/UB
add wave -noupdate /testbench/L6/LB
add wave -noupdate /testbench/L6/OE
add wave -noupdate /testbench/L6/WE
add wave -noupdate -radix hexadecimal /testbench/L6/ADDR
add wave -noupdate -radix hexadecimal /testbench/L6/Data
add wave -noupdate -radix hexadecimal /testbench/L6/my_slc/state_controller/State
add wave -noupdate -radix hexadecimal /testbench/L6/my_slc/d0/ir_t
add wave -noupdate -radix hexadecimal /testbench/L6/my_slc/d0/mdr_t
add wave -noupdate -radix hexadecimal /testbench/L6/my_slc/d0/pc_t
add wave -noupdate -radix hexadecimal /testbench/L6/my_slc/d0/mar_t
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {191827 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 252
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ns
update
WaveRestoreZoom {0 ps} {1842548 ps}
