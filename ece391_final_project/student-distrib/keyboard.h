/* keyboard.h - header file for initializing and handling the keyboard to print to screen
 * vim:ts=4 noexpandtab
 */

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#define keyboard_irq_number     1
#define keyboard_data_port      0x60
#define keyboard_command_port   0x64
#define keyboard_table_size     59
#define table_max_size          60

//null key
#define NULLKEY                 '\0'

/* special keys*/
#define left_shift_pressed      0x2A
#define right_shift_presssed    0x36
#define caps_pressed            0x3A
#define enter_pressed           0x1C
#define ctrl_pressed            0x1D
#define ctrl_released           0x9D
#define alt_pressed             0x38
#define alt_released            0xB8
#define BACKSPACE               0X0E
#define left_shift_released     0xAA
#define right_shift_released    0xB6
#define caps_released           0xBA

/* Case directory*/
#define no_s_c                  0
#define only_c                  1
#define only_s                  2
#define c_and_s                 3


/* keyboard initialization function */
extern void keyboard_init();
// extern void add_char_to_buffer(unsigned int input);
/* keyboard data print to console function */
// extern void keyboard_handler();
extern int get_single_char(unsigned int input,unsigned int mode);
#endif
