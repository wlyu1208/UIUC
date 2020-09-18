/* keyboard.c - initializing and handling the keyboard to print to screen
 * vim:ts=4 noexpandtab */

#include "keyboard.h"
#include "lib.h"
#include "i8259.h"
#include "x86_desc.h"
#include "lib.h"
#include "types.h"

/* Scancode table based on https://wiki.osdev.org/PS/2_Keyboard 
 * ( " ' \ ) These three character needs precede of \ to print out
 * table only for no shift and no caps not fancy */
uint8_t lower_keyboard_table[keyboard_table_size] = {
    0,  0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, // 15
    0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0, // 14
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, //14
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, 0, 0, ' ', 0 //16
    // 0X37 (keypad * pressed) not sure
};

/* shift and no cap */
uint8_t shift_keyboard_table[keyboard_table_size] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0,
    0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0,
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0,
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, 0, 0, ' ', 0
};

/* cap and no shift  */
uint8_t cap_keyboard_table[keyboard_table_size] = {
    0,  0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,
    0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', 0,
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'','`', 0,
    '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 0, 0, 0, ' ', 0
};

/* cap and shift*/
int8_t cap_shift_keyboard_table[keyboard_table_size] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0,
    0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}', 0,
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '\"', '~', 0,
    '|', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '<', '>', '?', 0, 0, 0, ' ', 0
};

/* keyboard_init
 * description: initialize the keyboard with correct irq number
 * inputs: none
 * outputs: none
 * side effect: enable keyboard irq to initialize */
void keyboard_init(){
    enable_irq(keyboard_irq_number); // enable keyboard with irq of 1
}

/* get_single_char
 * description: Return correct character based on correct mode
 * inputs: input - key pressed data     mode - shift and caps data
 * outputs: character of keypressed on correct mode
 * side effect: Based on input and mode, return correct character that will be print
 * mode 0: none
 * mode 1: caps
 * mode 2: shift
 * mode 3: caps and shift
 */
int get_single_char(unsigned int input,unsigned int mode)
{
    // printf("into get single char mode =:%d \n",mode);
    int ret;
    
    /* Based on the mode, get data from correct table and put on ret variable */
    switch (mode)
    {
    case no_s_c: 
        ret = lower_keyboard_table[input];
        // printf("return value is: %d\n",ret);
        return ret;
        break;

    case only_c:
        ret = cap_keyboard_table[input];
        // printf("return value is: %d\n",ret);
        return ret;
        break;

    case only_s:
        ret =  shift_keyboard_table[input];
        // printf("return value is: %d\n",ret);
        return ret;
        break;

    case c_and_s:
        ret =  cap_shift_keyboard_table[input];
        // printf("return value is: %d\n",ret);
        return ret;
        break;

    default:
        printf("error in single char!");
        return 0; // if no mode is chosen, return empty
        break;
    }
    return 0;
}
