/* terminal.h: header file terminal.c 
  definig all the constants for keyboard, color for diff terminals
  */

#ifndef _SYSCALL_H
#define _SYSCALL_H

#include "keyboard.h"
#include "lib.h"
#include "i8259.h"
#include "x86_desc.h"
#include "types.h"

/* set neccessary varibles */
#define pixel_size 2000
#define one_4k_page_size 0x1000
#define nextline_mode 0
#define clear_screen_mode 1
#define buffer_size 128
#define alt_mode        4
#define max_terminal 3
#define VIRT_VIDEO 0x4800000
#define memory_size 4000
#define green 0x2
#define orange 0x6
#define grey 0x8
uint8_t term_in_use_num;
uint8_t switch_flag;
uint8_t target_terminal;
int current_terminal;
volatile uint8_t fetch_mode;
volatile int32_t terminal_write_flag;
/* structure for one terminal */
typedef struct terminal_t
{
    uint8_t id;
    uint8_t process_id;
    uint8_t running_process;
    uint32_t xpos;
    uint32_t ypos;
    unsigned int buf_index;
    uint8_t buf[pixel_size]; // include \0 and feed character
    // uint8_t *video_memory;/
    uint8_t * video_memory;
    uint32_t term_esp;
    uint32_t term_ebp;
    uint8_t exist;
    volatile uint8_t execute_flag;
}terminal_t;
terminal_t terminals[max_terminal];
/* function */

void initterm();
int32_t switch_term(uint8_t id);
int32_t save_term(uint8_t id);
int32_t load_term(uint8_t id);

/* Open terminal */
extern int32_t terminal_open(const uint8_t* filename);

/* Close terminal*/
extern int32_t terminal_close(int32_t fd);

/* Initialized buffer */
extern void init_keyboard_buffer();

/* read the keyboard */
extern void read_keyboard_input(unsigned int input);

/* based on input and mode, put data to buffer */
extern void add_one_char_to_buffer(unsigned int input,unsigned int mode);

/* write the keyboard */
extern void display_keyboard_input();

/* handle keyboard interrupt */
extern void keyboard_handler();

/* handle back space when occurred */
extern void back_space();

/* read buffer from terminal */
extern int32_t terminal_read(int32_t fd,void *buf,int32_t size);

/* write buffer from terminal to screen */
extern int32_t terminal_write(int32_t fd, const void* input_buffer, int32_t nbytes);

/* fetch new term number */
int32_t fetch_new_term_number(uint8_t mode);

/* init color */
void init_color(uint8_t id);

#endif
