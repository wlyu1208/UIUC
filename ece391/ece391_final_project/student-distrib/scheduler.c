/* scheduler.c - initializing PIT for scheduling 
handler for PIT 
 */

#include "scheduler.h"
#include "syscall.h"
#include "lib.h"
#include "terminal.h"
#include "x86_desc.h"
#include "page.h"
#include "i8259.h"

pcb_t* schedule_process; /* pcb of process to run */

/* Initializes the PIT
  Input: none
  Output: none
   Side effect: PIT interrupt enabled
*/
void pit_init() {
    outb(INIT_CMD, CMD_PORT);
    outb(DIVIDE_100_HZ & BYTEMASK, CHANNEL_0_PORT);
    outb((DIVIDE_100_HZ >> BYTESIZE) & BYTEMASK, CHANNEL_0_PORT);
    enable_irq(PIT_IRQ);
}


/* pit_handler
 * description: handles pit interrupts
 * inputs: none
 * outputs: none
 * side effect: handle pit for scheduling processes
   */
void pit_handler() {

    send_eoi(PIT_IRQ);
    cli();    
    //schedule next task
    /* save esp/ebp of current process */
    schedule_process = find_pcb_by_process_number(terminals[schedule_terminal_num].running_process);
     // save old process's esp and ebp
    asm volatile( 
                "movl %%esp, %0;"
                "movl %%ebp, %1;"
                : "=r"(schedule_process->esp), "=r"(schedule_process->ebp)
                );

    /* switch process paging */
    schedule_terminal_num = (schedule_terminal_num + 1) % max_terminal;
    
    if (!terminals[schedule_terminal_num].exist) {
        switch_term(schedule_terminal_num);
        execute((uint8_t*)"shell");
    }
    schedule_process = find_pcb_by_process_number(terminals[schedule_terminal_num].running_process);

    // map the user program
    map_from_vir_to_phy(VIR_USER_PROGRAM,MEM_8MB+(terminals[schedule_terminal_num].running_process)*mem_4mb);
    // is scheduled terminal is the currently shown terminal, display on the screen
    if(schedule_terminal_num!=current_terminal)
    {
        map_from_vir_to_phy_with_4kb_size_page((uint32_t)vir_video,(uint32_t)terminals[schedule_terminal_num].video_memory);
    }
    // else display in the video memory allocated for that terminal
    else
    {
        map_from_vir_to_phy_with_4kb_size_page((uint32_t)vir_video,(uint32_t)VIDEO);
    }
    // /* set tss to new esp */
    tss.ss0 = KERNEL_DS;
    tss.esp0 = MEM_8MB-(terminals[schedule_terminal_num].running_process)*MEM_8KB-ADDRESS_LENGTH;

    /* update running video coordinates */
    /* restore next process esp/ebp */
    asm volatile( 
                "movl %0, %%esp;"
                "movl %1, %%ebp;"
                :
                : "r"(schedule_process->esp), "r"(schedule_process->ebp)
                : "memory");
    sti();
}
