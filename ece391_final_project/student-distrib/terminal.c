/* terminal.c - initialize all the terminals and handles writing and reading from terminal 
 * reading keyboard input
 * saving and loading terminal for teerm switch 
 */


#include "terminal.h"
#include "syscall.h"
#include "page.h"
#include "scheduler.h"
// #include "lib.h"
/* terminal_read(input)
 * description: read the input
 * from keyboard,change mode based on it
 * or change the buf
 * 
 * input: input = read in key value
 * output:none
 * side effect: change buf and mode
 */

// #define shiftispressed (leftshiftpressed|rightshiftpressed)
// #define ctrlispressed (leftctrlpressed|rightctrlpressed)
// terminal_t terminals[max_terminal];
// int current_terminal;
// static unsigned int buf_index=0;
// static uint8_t buf[buffer_size];
// static uint8_t terminal_buf[buffer_size];
static int leftshiftpressed = 0;
static int rightshiftpressed = 0;
static int ctrlpressed = 0;
static unsigned int capspressed = 0;
static int enable_write = 0;
static int altpressed = 0;
static int enterpressed = 0;
//static terminal_t terminal_list[1];
int terminal_fd = 6;

/* initterm()
 * description: initialize the terminal 
 * input: none
 * output: none
 * side effect: initialize the terminal     
 */
void initterm()
{
    uint8_t index;
    uint32_t indice;
    cli();
    for(index=0;index<max_terminal;index++)
    {
        terminals[index].id = index;
        terminals[index].running_process = -1;
        terminals[index].xpos = 0;
        terminals[index].ypos = 0;
        terminals[index].buf_index = 0;
        terminals[index].exist = 0;
        terminals[index].execute_flag = 0;
        for(indice=0;indice<pixel_size;indice++) // very important, should know the size!
        {
            terminals[index].buf[indice] = '\0';
        }
        // allocate new physical memory, or frame
        uint32_t video_memory = VIDEO+one_4k_page_size*(index+1);
        // map a virtual address to new physical address
        map_from_vir_to_phy_with_page_index(vir_video,video_memory,index+1);
        // assign video memory to terminal
        terminals[index].video_memory = (uint8_t*) vir_video+(index+1)*one_4k_page_size;
        init_color(index);
    }
    current_terminal = 0;
    init_keyboard_buffer(0);
    load_term(0);
    sti();
}

/* switch_term()
 * description: switch to another terminal
 * input: id = number for which terminal
 * output: return 0 if succeed or -1 for fail
 * side effect: switch to different terminal based on id
 */
int32_t switch_term(uint8_t id)
{
    if(id>max_terminal-1)
    {   
        return -1;
    }
    else if(id==current_terminal) 
    {
        return 0;
    }
    else
    {
        cli();
        save_term(current_terminal);
        load_term(id);
        current_terminal = id;
        terminal_write_flag =0;
        sti();
        return 0;
    }

}

/* init_color()
 * description: change terminal with different color
 * input: id = number for terminal to change
 * output: none
 * side effect: change terminal color based on id    
 */
void init_color(uint8_t id)
{
    printf("into init color");
    uint32_t indice;

    if(id==0)
    {
        for(indice=0;indice<pixel_size;indice++)
        {
            *(uint8_t*) (terminals[id].video_memory+(indice<<1)) = ' '; // store " " into content
            *(uint8_t*) (terminals[id].video_memory+(indice<<1)+1) = green; // change the color based on id
        }
        return;
    }
    else if(id==1)
    {
        for(indice=0;indice<pixel_size;indice++)
        {
            *(uint8_t*) (terminals[id].video_memory+(indice<<1)) = ' ';
            *(uint8_t*) (terminals[id].video_memory+(indice<<1)+1) = orange;
        }
        return;
    }
    else if(id==2)
    {
        for(indice=0;indice<pixel_size;indice++)
        {
            *(uint8_t*) (terminals[id].video_memory+(indice<<1)) = ' ';
            *(uint8_t*) (terminals[id].video_memory+(indice<<1)+1) = grey;
        }
        return;
    }
    else
    {
        printf("init color fail!\n");
        return ;
    }
    
}

/* save_term()
 * description: saves old terminal
 * input: id = number for terminal
 * output: return 0 unless fail than return -1
 * side effect: save the old terminal data    
 */
int32_t save_term(uint8_t id)
{
    if(id>2) return -1;
    terminals[id].xpos = get_screen_x();
    terminals[id].ypos = get_screen_y();
    // copy shown video into scheduled video
    memcpy((uint8_t *) terminals[id].video_memory,(uint8_t*)VIDEO,memory_size);
    return 0;
}

/* load_term()
 * description: lead the old terminal
 * input: id = number for terminal
 * output: return 0 unless fail than return -1
 * side effect: load the old terminal data    
 */
int32_t load_term(uint8_t id)
{
    if(id>2) return -1;
    // place the shown cursor to the scheduled terminal 
    place_cursor(terminals[id].xpos,terminals[id].ypos);
    // copy scheduled video into shown one
    memcpy((uint8_t*)VIDEO,(uint8_t *)terminals[id].video_memory,memory_size);
    return 0;
}

/* terminal_open()
 * description: init buffer!!
 * input: none
 * output: none
 * return: none
 * side effect: init buffer
 */
int32_t terminal_open(const uint8_t* filename)
{
    return 0;
}

/* terminal_close()
 * description: close terminal
 * input: none
 * output: none
 * return: 0
 * side effect: close the terminal
 */
int32_t terminal_close(int32_t fd)
{
    return 0;
}

/* init_keyboard_buffer()
 * description: initialize the keyboard buffer 
 * input: index -- which terminal to reset buffer
 * output: none
 * side effect: set buf and bug_index to 0
 */
void init_keyboard_buffer(uint32_t index)
{
    int i;
    for(i=0;i<buffer_size;i++)
    {
        terminals[index].buf[i] = 0; // reset every buffer data to 0
    }
    terminals[index].buf_index = 0; // set buffer index to 0
}

/* read_keyboard_input()
 * description: read input of keyboard and tell which 
 * based keys are pressed
 * input: input - data from the keyboard
 * output: none
 * return: none
 * side effect: trigger variable based on pressed key
 */
void read_keyboard_input(unsigned int input)
{
    /* based on input, trigger neccessary key pressed variable and released */
    if(input==alt_pressed){   
        altpressed=1;
        return;
    }
    if(input==alt_released){
        altpressed=0;
        return;
    }
    if(input==left_shift_pressed)
    {
        leftshiftpressed=1;
        return;
    }
    if(input==left_shift_released)
    {
        leftshiftpressed=0;
        return;
    }
    if(input==right_shift_presssed)
    {
        rightshiftpressed=1;
        return;
    }
    if(input==right_shift_released)
    {
        rightshiftpressed = 0;
        return;
    }
    if(input==caps_pressed) // if caps pressed 
    {
        capspressed = (1+capspressed)%2;  // update capspressed
        return;
    }
    if(input==ctrl_pressed) 
    {
        ctrlpressed = 1;
        return;
    }
    if(input==ctrl_released)
    {
        ctrlpressed = 0;
        return;
    }
    
    /* if enter is on, scroll up */
    if(input==enter_pressed && enterpressed == 0)
    {        
        /* if enter pressed, set execute flag to 1 and scroll up */
        terminals[current_terminal].execute_flag = 1;
        enterpressed = 1;
        scroll_up(); 
        
        return;
    } else if (input!=enter_pressed && enterpressed == 1) {
        enterpressed = 0;
    }
    
    /* if ctrl+L pressed, clear the screen */
    if(ctrlpressed)
    {
        if(input==0x2E)  // 0x2E is the scancode of character 'c'
        {
            // halt(0);
            // switch_term(0);
            // clear_screen();
            // init_keyboard_buffer();
            // place_cursor(0,0); // place the cursor to the upper left corner
            return;
        } // c = 0x2E
        if(input ==0x26) // 0x26 is the scancode of character 'l' or 'L', clear screen if ctrl+L
        {   
            clear_screen();
            init_keyboard_buffer(current_terminal);
            place_cursor(0,0); // place the cursor to the upper left corner
            puts("391OS> ");
            return;
        }
    }

    /* Do back space if backspace triggered */
    if(input==BACKSPACE)
    {
        back_space(); // if backspace, delete one character
        return;
    }

    /* if alt pressed do nothing */
    if(altpressed){
        if(input==0x3B) // 0x3B is key f1
        {
            switch_term(0); // switch to terminal 0
            return ;
        }
        else if(input==0x3C) // 0x3C is key f2
        {
            switch_term(1); // switch to terminal 1
            return ;
        }
        else if(input==0x3D) // 0x3D is key f3
        {
            switch_term(2); // switch to terminal 2
            return ;
        }
        else return;
    }
    
    /* print character if inside of table of ps/2 scancode */
    if(input<keyboard_table_size)
    {
        int mode;

        mode = capspressed | ((leftshiftpressed|rightshiftpressed)<<1); // mode dependent on shift and ctrl

        if(ctrlpressed==0&&terminals[current_terminal].buf_index<buffer_size-1) // buffer not full
        {
            add_one_char_to_buffer(input,mode);
        }
        else
        {
            enable_write = 0;
        }
        
    }
    return;
}


/* add_one_char_to_buffer(input)
 * description: add one single char to buffer
 * input: input = scancode
 *          mode = caps,shift mode
 *          0 none, 1 caps, 2 shift, 3 cap and shift, 4 alt
 * output: none
 * return: none
 * side effect: buffer changed.
 */
void add_one_char_to_buffer(unsigned int input,unsigned int mode)
{
    // printf("into add one char\n");
    int single_char;
    single_char = get_single_char(input,mode); // get char from table 
    if(single_char!=0)// update buffer if not unknown scancode
    {
        terminals[current_terminal].buf[terminals[current_terminal].buf_index] = get_single_char(input,mode); 
        terminals[current_terminal].buf_index++;
        terminals[current_terminal].buf[terminals[current_terminal].buf_index] = NULLKEY;
        enable_write = 1 ; // enable display character to the screen.
        return;
    }
    enable_write = 0;
    return;
}

/* display_keyboard_input(input)
 * description: print out the input
 * input: input = scancode
 * output: none
 * return: none
 * side effect: print out data on buffer
 */
void display_keyboard_input(unsigned int input)
{
    if((enable_write==1)&&input<keyboard_table_size) // print if enable write and scancode is in table
    {
        if(terminals[current_terminal].buf_index<buffer_size)
        {
            putc(terminals[current_terminal].buf[terminals[current_terminal].buf_index-1]); // display the last character
        }
    }
    enable_write = 0;
    
    return;
}

/* keyboard_handler()
 * description: handle interrupt for keyboard
 * input: none
 * output: none
 * return: none
 * side effect: trigger that keyboard currently works
 */
void keyboard_handler(){
    // printf("into keyboard handler!\n");
    uint32_t input;
    cli();
    input = inb(keyboard_data_port); // get data of keyboard
    send_eoi(keyboard_irq_number); // send interrupt signal 
    /* As input number is inside of table, print out correct character on the console */
    read_keyboard_input(input);
    display_keyboard_input(input);
    sti();
}

/* back_space()
 * description: In correct boudnary, delete before buffer
 * input: none
 * output: none
 * return: none
 * side effect: delete buffer data and set new cursor position
 */
void back_space()
{
    if(terminals[current_terminal].buf_index>0)
    {
        backspace_helper(terminals[current_terminal].buf_index);
        terminals[current_terminal].buf_index--;
        terminals[current_terminal].buf[terminals[current_terminal].buf_index] = NULLKEY; // update buffer 
        puts((int8_t *)terminals[current_terminal].buf);  
        if(terminals[current_terminal].buf_index==0) set_cursor_pos(); // if index=0, set to leftmost
    }
    return;
}



/* terminal_read()
 * description: read the keyboard buffer into input buffer
 * input: fd = file descriptor for terminal
 *        input_buf = buffer that read keyboard buffer in
 *        input_size = size of buffer to read
 * output: length of bytes read from keyboard into
 * side effect: read the terminal
 */
int32_t terminal_read(int32_t fd,void *input_buf,int32_t input_size)
{
    int8_t *tmp_buffer = (int8_t*)input_buf;    
    while (!terminals[schedule_terminal_num].execute_flag);    
    cli();
    terminals[schedule_terminal_num].execute_flag = 0;
    if(input_size>buffer_size) // upper bound is keyboard buffer size
    {
        input_size = buffer_size;
    }
    memcpy(tmp_buffer,terminals[schedule_terminal_num].buf,input_size);
    *((int8_t*)input_buf+input_size) = '\0';
    init_keyboard_buffer(schedule_terminal_num);
    sti();
    return input_size;
}

/* terminal_write()
 * description: write buffer from terminal to screen 
 * input: fd = fd for terminal
 *         input_buffer = buffer written to terminal
 *          nbytes = size of written terminal
 * output: none
 * side effect: writes the terminal    
 */
int32_t terminal_write(int32_t fd, const void* input_buffer, int32_t nbytes)
{
	register int32_t index = 0; // high frequency used counter.
    cli();
    if(current_terminal==schedule_terminal_num)// use putc to print to screen if shown terminal is scheduled one
    {
        while (index < nbytes) {
            putc((((int8_t*)input_buffer)[index]));
            index++;
        }
    }
    else// use new putc_terminal_running to store buffer into terminal while other terminals are scheduled.
    {
        while(index < nbytes) { 
            putc_terminal_running((((int8_t*)input_buffer)[index]));
            index++;
        }
    }
    sti();
	return index;
}
