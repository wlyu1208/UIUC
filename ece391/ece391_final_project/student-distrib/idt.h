/* idt.h - Defines for initializing the Interrupt Descriptor Table
 * vim:ts=4 noexpandtab
 */
#ifndef _IDT_H
#define _IDT_H

/* indexes into the idt table for each exception */
#define DIVIDE_ERROR_EXCEPTION_VEC          0x00
#define DEBUG_EXCEPTION_VEC                 0x01
#define NMI_INTERRUPT_VEC                   0x02
#define BREAKPOINT_EXCEPTION_VEC            0x03
#define OVERFLOW_EXCEPTION_VEC              0x04
#define BOUND_RANGE_EXCEEDED_EXCEPTION_VEC  0x05
#define INVALID_OPCODE_EXCEPTION_VEC        0x06
#define DEVICE_NOT_AVAILABLE_EXCEPTION_VEC  0x07
#define DOUBLE_FAULT_EXCEPTION_VEC          0x08
#define COPROCESSOR_SEGMENT_OVERRUN_VEC     0x09
#define INVALID_TSS_EXCEPTION_VEC           0x0A
#define SEGMENT_NOT_PRESENT_EXCEPTION_VEC   0x0B
#define STACK_FAULT_EXCEPTION_VEC           0x0C
#define GENERAL_PROTECTION_EXCEPTION_VEC    0x0D
#define PAGE_FAULT_EXCEPTION_VEC            0x0E
#define x87_FLOATING_POINT_ERROR_VEC        0x10
#define ALIGNMENT_CHECK_EXCEPTION_VEC       0x11
#define MACHINE_CHECK_EXCEPTION_VEC         0x12
#define SIMD_FLOATING_POINT_EXCEPTION_VEC   0x13

/* indexes into the idt table for each interrupt */
#define PIT_INTERRUPT_VEC                   0x20
#define KEYBOARD_INTERRUPT_VEC              0x21
#define RTC_INTERRUPT_VEC                   0x28
#define SYSCALL_INTERRUPT_VEC               0x80

#define RESERVED_EXCEPTIONS                 0x14

/* linkers to idt_helper.S */
extern void KEYBOARD_INTERRUPT();
extern void RTC_INTERRUPT();
extern void PIT_INTERRUPT();
extern void SYSCALL_INTERRUPT();
/* initializes idt */
int init_idt();

#endif
