/* syscall.c - implements all the syscalls , video mapping, getting arguamnets 
* getting current PCB function 
 */

#include "syscall.h"
#include "lib.h"
#include "rtc.h"
#include "terminal.h"
#include "filesystem.h"
#include "x86_desc.h"
#include "page.h"
#include "scheduler.h"

uint8_t process_queue[PROCESS_MAX_NUMBER];
uint8_t magical_number_array[FILE_DATA_BUF] = {0x7f,0x45,0x4c,0x46}; // magical number for the first 4 bits of file
/* init_process_queue: initialize the process queue
* Parameter: None
* Return: None
* side effect: set all process to be no use
*/
void init_process_queue()
{
    /* Initialized process queue to 0 */
    int i;
    for(i=0;i<PROCESS_MAX_NUMBER;i++)
    {
        process_queue[i] = 0;
    }
    return;
}

/* read_void:placeholder for read
* Parameter: None
* Return: None
* side effect: none
*/
int32_t read_void(int32_t fd, void* buf, int32_t nbytes)
{
  return -1;
} 
/* read_void:placeholder for write
* Parameter: None
* Return: None
* side effect: none
*/
int32_t write_void(int32_t fd, const void* buf, int32_t nbytes)
{
  return -1;
}
/* read_void:placeholder for close
* Parameter: None
* Return: None
* side effect: none
*/
int32_t close_void(int32_t fd)
{
  return -1;
}
/* read_void:placeholder for open
* Parameter: None
* Return: None
* side effect: none
*/
int32_t open_void(const uint8_t* filename)
{
  return -1;
}

/* jump table for stand in, out and no operation */
file_op_jmp_table stdin =  {(void*)terminal_read,(void*)write_void,(void*)terminal_close,(void*)terminal_open};
file_op_jmp_table stdout={(void*)read_void,(void*)terminal_write,(void*)terminal_close,(void*)terminal_open};
file_op_jmp_table no_op = {(void*)read_void,(void*)write_void,(void*)close_void,(void*)open_void};

/* jump table for rtc, directory, file */
file_op_jmp_table rtc_table	= { (void*)rtc_read, (void*)rtc_write, (void*)rtc_close, (void*)rtc_open };
file_op_jmp_table dir_table	= { (void*)dir_read, (void*)dir_write, (void*)dir_close, (void*)dir_open };
file_op_jmp_table file_table	= { (void*)file_read, (void*)file_write, (void*)file_close, (void*)file_open };

/* syscall_execute: attempts to load and execute a new program
* Parameter: 
*           command: the first word is the name of file to be execute,
                    the rest of the command should be provided to the new program 
* Return: depends on what to execute, -1 on failure, 256 on exception 
*         value by halt if is halt
*         should we return 0 if success? maybe!
*/
int32_t execute(const uint8_t* command) {
  // puts(command);
  // printf("\nexecute start:%s\n",&command);
  /* 1. parse the command including cmd and arg*/
  int32_t start, i;
  int8_t cmd_buf[CMD_BUF_SIZE], arg_buf[ARG_BUF_SIZE];  
  int32_t user_stack, status, data_buf_size;
  uint32_t entry_point;
  pcb_t *old_pcb, *new_pcb;  
  dentry_t new_dentry;
  uint8_t file_data_buf[FILE_DATA_BUF];
  cli();
 // initialize command buffer 
  for(i=0;i<CMD_BUF_SIZE;i++)
  {
    cmd_buf[i] = '\0';
  }


  for(i=0;i<ARG_BUF_SIZE;i++)
  {
    arg_buf[i] = '\0';
  }

  start = 0; // start of cmd
  while(command[start]==' '||command[start]=='\n') // rm empty space
  {
    start++;
  }
  for(i=0;command[start]!=' '&&command[start]!='\0'&&command[start]!='\n';i++) // store cmd
  {
    cmd_buf[i] = command[start];
    start++;
  }
  cmd_buf[start] = '\0';
  // arg
  while(command[start]==' '||command[start]=='\n')
  {
    start++;
  }
  for(i=0;command[start]!='\0'&&command[start]!='\n'&&command[start]!=' ';i++) // store argument
  {
    arg_buf[i] = command[start];
    start++;
  }
  arg_buf[start] = '\0';

  /* 2. executable check */
  if(-1==read_dentry_by_name((uint8_t*)cmd_buf,&new_dentry))
  {
    return -1;
  };  
  data_buf_size = read_data(new_dentry.inode_num,FILE_START_POS,file_data_buf,FILE_DATA_BUF); // read the first 4 bits into buffer

  // check elf, if not equals to the magic number, not executable
  for(i=0;i<FILE_DATA_BUF;i++)
  {
    if(file_data_buf[i]!=magical_number_array[i]) // check each magical number
    {
      printf("not elf");
      return -1;
    }
  }
  
  current_process_number=get_new_process_number();  // get a free process
  if(current_process_number==-1) 
  { 
    return -1;
  }
  /* 3. paging, map 128mb to correct physical memory */
  map_from_vir_to_phy(VIR_USER_PROGRAM,MEM_8MB+(current_process_number)*mem_4mb);

  /* 4. file loader, load file into physical memory */

  read_data(new_dentry.inode_num,FILE_START_POS,(uint8_t*)(LOADER_ADDRESS),READ_DATA_LARGE_NUMBER); // load the file img into program address
  
  /* 5. set up pcb */
  new_pcb = (pcb_t *)(MEM_8MB - (current_process_number + 1) * MEM_8KB);  
  asm volatile( "movl %%esp,%0;"
                "movl %%ebp,%1;"
                :"=r"(new_pcb->parent_esp),"=r"(new_pcb->parent_ebp) // store last process for halt!
                :
                :"cc");

  if(terminals[schedule_terminal_num].exist==0) // first program, return to shell
  {
    // we might need to execute shell firstly
    terminals[schedule_terminal_num].exist = 1;
    new_pcb->parent_process_number = current_process_number;
    terminals[schedule_terminal_num].process_id = current_process_number;
    terminals[schedule_terminal_num].running_process = current_process_number;
  }
  else // not the first one, store last process, but how to do it? left for 
  {
    old_pcb = (pcb_t*)get_current_pcb(); // get old pcb
    new_pcb->parent_process_number = old_pcb->process_number;
    terminals[schedule_terminal_num].running_process = current_process_number;
  }

  new_pcb->process_number = current_process_number; 
  new_pcb->ebp = MEM_8MB-(current_process_number)*MEM_8KB-ADDRESS_LENGTH; // new stack
  new_pcb->esp = new_pcb->ebp; //esp starts at ebp
  read_data(new_dentry.inode_num,ENTRY_POINT_OFFSET,(uint8_t*)&entry_point,ENTRY_POINT_OFFSET_LENGTH); // 24=address of entry point, 4 means 32B

  for(i=0;i<MAX_FILES_NUMBER;i++) // init all fd in pcb
  {
    if(i==0) // initialize the stdin
    {
      new_pcb->file_descriptor_array[i].file_position=FILE_START_POS;
      new_pcb->file_descriptor_array[i].file_op_table = &stdin;
      new_pcb->file_descriptor_array[i].flags_in_use = FLAG_IN_USE; // in use
      new_pcb->file_descriptor_array[i].inode_index = new_dentry.inode_num;
      continue;
    }
    if(i==1) // initialize the stdout
    {
      new_pcb->file_descriptor_array[i].file_position=FILE_START_POS;
      new_pcb->file_descriptor_array[i].file_op_table = &stdout;
      new_pcb->file_descriptor_array[i].flags_in_use = FLAG_IN_USE; // in use
      new_pcb->file_descriptor_array[i].inode_index = new_dentry.inode_num;
      continue;
    }
    // initialize all unopen files 
    new_pcb->file_descriptor_array[i].file_position=FILE_START_POS;
    new_pcb->file_descriptor_array[i].file_op_table = &no_op;
    new_pcb->file_descriptor_array[i].flags_in_use = FLAG_NOT_IN_USE; // not in use
    new_pcb->file_descriptor_array[i].inode_index = new_dentry.inode_num;
  }
  memcpy(new_pcb->arg, arg_buf, ARG_BUF_SIZE); // copy arg into pcb
  /* 6. context switch */
  tss.ss0 = KERNEL_DS;
  tss.esp0 = new_pcb->esp;
  /* left for artificial iret context 
   * eip,cs,eflags,esp,ss,user-stack,ds then iret
   * eip = entry_point
   * cs,ss=user_code_segment
   * eflags = easy
   * ds = user_ds
   */    
  user_stack = VIR_USER_PROGRAM+mem_4mb-ADDRESS_LENGTH; // the stack pointer
  sti();
  asm volatile(
                "andl $0x00FF, %%eax;" // get ds
                "movw %%ax,%%ds;"
                "pushl %%eax;" // push user ds
                "pushl %%ecx;" // push user esp
                "pushfl;"     // push eflags
                "pushl %%edx;" // push user cs
                "pushl %%ebx;"  //push entry point
                "iret;"
                "ret_from_halt:"
                "movl %%eax, %0;"
              : "=r"(status)
              : "a"(USER_DS),"b"(entry_point),"c"(user_stack),"d"(USER_CS)
              : "cc", "memory");

  if (status == EXCEPTION_RET) return EXCEPTION_STATUS;
  return status;
}

/* get_new_process_number: get a new process number
* Parameter: None
* Return: the index of new process in the queue, -1 if none left
*/
int32_t get_new_process_number()
{
  int i;
  for(i=0;i<PROCESS_MAX_NUMBER;i++)
  {
    if(process_queue[i]==0)
    {
      process_queue[i]=1;
      return i;
    }
  }
  printf("process number reaches the limit!\n");
  return -1;
}

/* find_pcb_by_process_number: find a pcb based using the process number
* Parameter: process number = the index of current process in the process queue
* Return: the address of pcb
*/
pcb_t *find_pcb_by_process_number(int32_t process_number)
{
  return (pcb_t *)(MEM_8MB - (process_number+1)*MEM_8KB);
}

/* syscall_halt: terminates a process and returns to parent process
* Parameter: 
*            status: status of syscall
* Return: 0 on success, -1 on failure
*/
int32_t halt(uint8_t status) {
  int32_t i;
  cli();
  pcb_t *current_pcb = get_current_pcb();
  //pcb_t *parent_pcb = find_pcb_by_process_number(current_pcb->parent_process_number);
  
  /* set tss back to parent */
  tss.esp0 = MEM_8MB-(current_pcb->parent_process_number)*MEM_8KB-ADDRESS_LENGTH;
  /* free up process queue */
  process_queue[current_pcb->process_number] = 0;  
  /* remap virtual to parent pcb */
  map_from_vir_to_phy(VIR_USER_PROGRAM,MEM_8MB+(current_pcb->parent_process_number)*mem_4mb);
  /* close all open files */
  for (i = 0; i < MAX_FILES_NUMBER; i++) {
    if (current_pcb->file_descriptor_array[i].flags_in_use) close(i);
  }
  terminals[schedule_terminal_num].running_process = current_pcb->parent_process_number;
  if (current_pcb->process_number == current_pcb->parent_process_number)
  {
    printf("return from the first shell:%d\n",schedule_terminal_num);
    terminals[schedule_terminal_num].exist = 0;
    execute((uint8_t*)"shell");
  }
  sti();
  asm volatile( 
                "movl %0, %%eax;"
                "movl %1, %%esp;"
                "movl %2, %%ebp;"
                "jmp ret_from_halt;"
                :
                :"r"((uint32_t)status), "r"(current_pcb->parent_esp), "r"(current_pcb->parent_ebp)
                :"%eax" );

  /* should never get here */
  return 0;
}

/* syscall_open: add file descriptor to a process's PCB
* Parameter: 
*           filename: the name of the file to add
* Return: , file descriptor number , 0 if rtc, -1 on failure
*/
int32_t open(const uint8_t* filename) {
  /* Initialize vars and flags */
  if(filename[0]=='\0') return -1;

  int i;
  uint32_t rtc_flag = 0;
  uint32_t dir_flag = 0; 
  uint32_t file_flag = 0; 
  dentry_t dentry; 
  int32_t retval_file_read; 

  /* get number of read file and check if not 0, if 0 return -1*/
  retval_file_read = read_dentry_by_name(filename, &dentry);
  if (retval_file_read != 0) return -1;

  // get the current PCB 
  pcb_t* pcb_task = get_current_pcb();

  /* check file system after stdout so start from 2 */
  for (i = MIN_FILESYSTEM_FD; i < MAX_FILES_NUMBER; i++){
      if (!(pcb_task->file_descriptor_array[i].flags_in_use)){
        int32_t retval;
        // determine the file type 
        if (dentry.file_type == 0) {
          retval = rtc_open(filename);
          rtc_flag = 1;
        }
        if(dentry.file_type == 1){
          retval = dir_open(filename);
          dir_flag = 1;
        }
        if(dentry.file_type == MIN_FILESYSTEM_FD){
          retval = file_open(filename);
          file_flag = 1;
        }
        //else retval = file_open(filename);
        //if (retval) return -1; 

        // mark the file descriptor table entry used 
        pcb_task->file_descriptor_array[i].flags_in_use = 1;

        /* if rtc occurs, change current pcb with rtc */
        if (rtc_flag){
          pcb_task->file_descriptor_array[i].file_op_table=&rtc_table;
          pcb_task->file_descriptor_array[i].inode_index=NULL;
          pcb_task->file_descriptor_array[i].file_position=FILE_START_POS;
          pcb_task->file_descriptor_array[i].flags_in_use=FLAG_IN_USE;
        }

        /* if directory occurs, change current pcb with directory */
        if(dir_flag){
          pcb_task->file_descriptor_array[i].file_op_table=&dir_table;
          pcb_task->file_descriptor_array[i].inode_index=retval;
          pcb_task->file_descriptor_array[i].file_position=FILE_START_POS;
          pcb_task->file_descriptor_array[i].flags_in_use=FLAG_IN_USE;
        }  

        /* if file system occurs, change current pcb with file system */
        if(file_flag){
          pcb_task->file_descriptor_array[i].file_op_table=&file_table;
          pcb_task->file_descriptor_array[i].inode_index=retval;
          pcb_task->file_descriptor_array[i].file_position=FILE_START_POS;
          pcb_task->file_descriptor_array[i].flags_in_use=FLAG_IN_USE;
        } 
        return i;
      }
  }
  // printf("opening\n");
  return -1;
}


/* syscall_close: close a file descriptor from a process's PCB
* Parameter: 
*           fd: the file descriptor of the open file to close 
* Return: 0 on success, -1 on failure
*/
int32_t close(int32_t fd){
  // don't allow user to close default (0, 1) files
  if (fd == 0 || fd == 1) return -1;  
  // sanity check for invalid descriptors
  if ((fd < 0) || (fd >= MAX_FILES_NUMBER)) return -1; 
  uint32_t retval;
  // get the current PCB 
  pcb_t* pcb_task = get_current_pcb(); 
  if (!pcb_task->file_descriptor_array[fd].flags_in_use) return -1;
  retval = pcb_task->file_descriptor_array[fd].file_op_table->close(fd);
  if (retval) return -1;

  // mark the file descriptor table entry free 
  pcb_task->file_descriptor_array[fd].flags_in_use = 0;
  return 0;
}


/* syscall_read: 
* Description: reads data from a file
* Parameter: 
*           fd: the file descriptor of the file to read
*           buf: the buffer  to be written
*           nbytes: number of bytes to read
* Return: number of bytes read on success, -1 on failure
*/
int32_t read(int32_t fd, void* buf, int32_t nbytes) {
  pcb_t* curr_pcb;
  /* sanity check for read */
  if (!buf) return -1; 
  /* sanity check for invalid descriptors */
  if ((fd < 0) || (fd > MAX_FILES_PER_BLOCK_INDEX)) return -1;
  /* sanity check for non-virtual buf addresses */
  if ((uint32_t)buf < VIR_USER_PROGRAM || (uint32_t)buf + (uint32_t)nbytes >= VIR_USER_PROGRAM_END) return -1;
  
  // get current PCB 
  curr_pcb = get_current_pcb();

  // if no open file related to this file descriptor error 
  if(curr_pcb->file_descriptor_array[fd].flags_in_use == 0) return -1;
  
  //read specific file using pointer 
  return (int32_t)(curr_pcb->file_descriptor_array[fd].file_op_table->read(fd, buf, nbytes));
}


/* syscall_write: writes data to a file 
* Parameter: 
*           fd: the file descriptor of the file to write
*           buf: the buffer containing the writing
*           nbytes: number of bytes to write
* Return: number of bytes written on success, -1 on failure
*/
int32_t write(int32_t fd, const void* buf, int32_t nbytes) {
  /* sanity check for write */
  if (!buf) return -1; 
  /* sanity check for invalid descriptors */
  if ((fd < 0) || (fd > MAX_FILES_PER_BLOCK_INDEX)) return -1;
  /* sanity check for non-virtual buf addresses */
  if ((uint32_t)buf < VIR_USER_PROGRAM || (uint32_t)buf + (uint32_t)nbytes >= VIR_USER_PROGRAM_END) return -1;

  // get current PCB 
  pcb_t* curr_pcb;
  curr_pcb = get_current_pcb();

  // if no open file related to this file descriptor error 
  if(curr_pcb->file_descriptor_array[fd].flags_in_use == 0) return -1;

  //write specific file using pointer 
  return (int32_t)(curr_pcb->file_descriptor_array[fd].file_op_table->write(fd, buf, nbytes));
}

/* GETS CURRENT PCB
*   Parameter:
*             none
*   Return: pointer to current PCB 
*/
pcb_t  *get_current_pcb(){
  uint32_t process_ESP;
  asm volatile("movl %%esp, %0;" : "=r" (process_ESP));
  return (pcb_t*) (process_ESP & MASK);
}


/* getargs: read the command line into input buffer.
* Parameter:
*           buf: user-level buffer to read into
*           nbytes: number bytes to read
* Return: -1 on failure, 0 on success
*/ 
int32_t getargs(uint8_t* buf, int32_t nbytes){
  // sanity checks
  if (!buf) return -1;
  if ((uint32_t)buf < VIR_USER_PROGRAM  || 
  (uint32_t)buf >= VIR_USER_PROGRAM_END) return -1; 

  // initialize the buf
  int i;
  for(i=0;i<nbytes;i++)
  {
    buf[i] = '\0';
  }
  // get current process control block
  pcb_t * curr_pcb = get_current_pcb();

  if ('\0'==curr_pcb->arg[0]||'\n'==curr_pcb->arg[0]) return -1; // no arguments

  if (nbytes < strlen(curr_pcb->arg) + 1) return -1;  // not enough space 

  strcpy ((int8_t *)buf, curr_pcb->arg); 

  return 0; 
}


/* maps text-mode video memory into user space at a pre-set virtual address
* Parameter:
*         screen_start: starting adress of video memory
* return: 0 if success. -1 if there is exception.
*/
int32_t vidmap(uint8_t** screen_start){
  // sanity check
  // the pointer should lie in the user program
  if ((uint32_t)screen_start < VIR_USER_PROGRAM  || (uint32_t)screen_start >= VIR_USER_PROGRAM_END) return -1; 
  // check NULL pointer
  if (!(uint32_t)screen_start) return -1; 
  // map the physical address of video memory to a preset virtual memory
  // if(schedule_terminal_num==current_terminal)
  // {
  map_from_vir_to_phy_with_4kb_size_page((uint32_t)(vir_video),(uint32_t)VIDEO);
  // }
  // pointer points at the virtual memory
  *screen_start = (uint8_t*)(vir_video);
  return 0;
}

/* Extra credit: not done */

/* signal handling 
* Parameter:
*           signum: signal number 
*           handler_address
* Return: -1 on failure, 0 on success
*/
int32_t set_handler(int32_t signum, void* handler_address){
  return 0; 
}

/* signal handling 
* Parameter:
*           none
* Return: -1 on failure, 0 on success
*/
int32_t sigreturn(void){
  return 0; 
}

