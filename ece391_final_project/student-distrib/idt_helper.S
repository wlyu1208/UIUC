# idt_helper.S - Functions to help with keyboard and rtc interrupts

#define ERROR         -1
#define MAX_SYSCALL   10
#define MIN_SYSCALL   1

.text
# make variable global
.globl KEYBOARD_INTERRUPT
.globl RTC_INTERRUPT
.globl PIT_INTERRUPT
.globl SYSCALL_INTERRUPT

KEYBOARD_INTERRUPT:
  pushfl                  
  pushal                  # push flag and all register
  call keyboard_handler   # call c function of printing keyboard
  popal                   
  popfl                   # pop flag and all register
  iret

RTC_INTERRUPT:        
  pushfl                  # push the necessary registers
  pushal
  call rtc_handler        # call c function of handling rtc
  popal
  popfl                   # pop flag and all register
  iret

PIT_INTERRUPT:
  pushfl                  # push the necessary registers
  pushal
  call pit_handler        # call c function of handling pit
  popal
  popfl                   # pop flag and all register
  iret

SYSCALL_INTERRUPT:
  pushfl                  # push flag  
  pushl %ebp              # push base pointer
  pushl %esi              # push index registers
  pushl %edi              
  pushl %edx              # start pushing arguments onto stack
  pushl %ecx
  pushl %ebx

  cmpl  $MAX_SYSCALL, %eax            # check for valid syscall number
  jg    _invalid_num                  # if too large return -1
  cmpl  $MIN_SYSCALL, %eax
  jl    _invalid_num                  # if too small return -1

  call  *syscall_jumptable(, %eax, 4) # call the corresponding syscall
  jmp   _syscall_restore              # restore registers

_invalid_num:
  movl  $ERROR, %eax        # put error into eax
_syscall_restore:
  popl  %ebx                # restore registers
  popl  %ecx
  popl  %edx
  popl  %edi
  popl  %esi
  popl  %ebp                    
  popfl
  iret                      # interrupt return

syscall_jumptable:
  .long 0x0, halt, execute, read, write, open, close, getargs, vidmap, set_handler, sigreturn  # add 4 more later
