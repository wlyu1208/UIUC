onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /testbench/CLK
add wave -noupdate /testbench/RESET
add wave -noupdate /testbench/AES_START
add wave -noupdate /testbench/AES_DONE
add wave -noupdate -radix hexadecimal /testbench/AES_KEY
add wave -noupdate -radix hexadecimal /testbench/AES_MSG_ENC
add wave -noupdate -radix hexadecimal /testbench/AES_MSG_DEC
add wave -noupdate -radix hexadecimal /testbench/encrypt/state0/present
add wave -noupdate -radix hexadecimal /testbench/encrypt/in_state
add wave -noupdate -radix hexadecimal /testbench/encrypt/key_s
add wave -noupdate -radix hexadecimal /testbench/encrypt/out_state
add wave -noupdate -radix hexadecimal /testbench/encrypt/count
add wave -noupdate /testbench/encrypt/state0/counter/out
add wave -noupdate /testbench/encrypt/selec_f
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {1261035 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 218
configure wave -valuecolwidth 194
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
WaveRestoreZoom {1235610 ps} {1331327 ps}
