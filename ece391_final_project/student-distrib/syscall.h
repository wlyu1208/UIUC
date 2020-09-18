/* syscall.h: header file syscall.c
*/ 


#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#include "types.h"
#include "filesystem.h"
#include "x86_desc.h"

#define MAX_FILES_PER_BLOCK_INDEX 7
#define MAX_FILES_NUMBER          8
#define MIN_FILESYSTEM_FD         2
#define EIGHT_MB                  8388608
#define EIGHT_KB                  8192
#define MASK                      0xFFFFE000
#define EXCEPTION_STATUS          256
#define EXCEPTION_RET             69   /* REALLY sketchy, fix later */
#define CMD_BUF_SIZE 1000
#define ARG_BUF_SIZE 1000
#define FILE_DATA_BUF 4
#define VIR_USER_PROGRAM 0x8000000
#define VIR_USER_PROGRAM_END 0x8400000
#define READ_DATA_LARGE_NUMBER 1000000
#define LOADER_ADDRESS 0x8048000
#define FILE_START_POS 0
#define FLAG_IN_USE 1
#define FLAG_NOT_IN_USE 0
#define PROCESS_MAX_NUMBER 6
#define ENTRY_POINT_OFFSET 24
#define ENTRY_POINT_OFFSET_LENGTH 4
#define ADDRESS_LENGTH 4
#define MEM_8MB 0x800000
#define MEM_8KB 0x2000
#define vir_video (VIR_USER_PROGRAM+MEM_8MB*2) // a preset virtual memory
/* ========================================
 * function prototype
 * ========================================
 */
// extern int32_t execute( const uint8_t* command);
// extern int32_t halt(uint8_t status);
// extern int32_t open(const uint8_t* filename);
// extern int32_t close(int32_t fd);
// extern int32_t read(int32_t fd, void* buf, uint8_t nbytes);
// extern int32_t write(int32_t fd, const void* buf, uint8_t nbytes);
//pcb_t get_current_pcb(); // not implemented yet!!!
extern int32_t getargs(uint8_t* buf, int32_t nbytes);
extern int32_t vidmap(uint8_t** screen_start);
extern int32_t set_handler(int32_t signum, void* handler_address);
extern int32_t sigreturn(void);
/* ========================================
 * global variable
 * ========================================
 */

// static uint8_t current_process_number;

/* ========================================
 * data structure
 * ====================================
 */
// static uint8_t process_queue[2] = {0,0};

/* structure for jump table of pointer */
typedef struct 
{
    int32_t (*read)(int32_t fd, void* buf, int32_t nbytes); // pointer to the read code
    int32_t (*write)(int32_t fd, const void* buf, int32_t nbytes);
    int32_t (*close)(int32_t fd);
    int32_t (*open)(const uint8_t* filename);
}file_op_jmp_table; 

/* structure for file descriptor element */
typedef struct // indexed by fd
{
    file_op_jmp_table *file_op_table;
    uint32_t inode_index;
    uint32_t file_position;
    uint32_t flags_in_use;
} file_descriptor_array_element;

/* structure for PCB */
typedef struct  //(on the top of each process, process=8kb)
{
    file_descriptor_array_element file_descriptor_array[MAX_FILES_NUMBER]; // maintain the file information for each process
    uint8_t process_number; // pid
    uint8_t parent_process_number; // parent pid
    uint32_t esp; // esp of current process
    uint32_t ebp; // ebp of current process
    uint32_t parent_esp; // esp of parent process
    uint32_t parent_ebp; // ebp of parent process
    int8_t arg[ARG_BUF_SIZE]; // argument of process
    uint32_t pcb_freq; //rtc frequency of process (for virtual rtc)
    uint32_t pcb_freq_wait; // number of rtc interrupts required (for virtual rtc)
} pcb_t;
/* ========================================
 * function prototype
 * ========================================
 */

/* System call functions */
extern int32_t execute( const uint8_t* command);
extern int32_t halt(uint8_t status);
extern int32_t open(const uint8_t* filename);
extern int32_t close(int32_t fd);
extern int32_t read(int32_t fd, void* buf, int32_t nbytes);
extern int32_t write(int32_t fd, const void* buf, int32_t nbytes);

/* get current pcb address */
extern pcb_t *get_current_pcb();

/* get new process number */
extern int32_t get_new_process_number();

/* Initialized process queue */
void init_process_queue();

extern pcb_t *find_pcb_by_process_number(int32_t process_number);
/* ========================================
 * global variable
 * ========================================
 */
int8_t current_process_number;

#endif
