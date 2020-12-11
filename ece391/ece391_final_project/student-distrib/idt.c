/* idt.c - initializing the Interrupt Descriptor Table, 
Exception numbers 
 * vim:ts=4 noexpandtab
 */

#include "idt.h"
#include "lib.h"
#include "i8259.h"
#include "x86_desc.h"
#include "keyboard.h"
#include "rtc.h"
#include "scheduler.h"
#include "syscall.h"

/** void EXCEPTION(); - default exception handler template
 *  Inputs: void
 *  Return value: none
 *  Function: masks interrupts, prints exception, then hangs */
#define EXCEPTION(NAME, ERR_MSG) void NAME() { printf("%s\n", ERR_MSG); halt(EXCEPTION_RET); while(1); }
/* create all the exception handlers for each exception defined in idt.h */
EXCEPTION(DIVIDE_ERROR_EXCEPTION, "DIVIDE_ERROR_EXCEPTION");
EXCEPTION(DEBUG_EXCEPTION, "DEBUG_EXCEPTION");
EXCEPTION(NMI_INTERRUPT, "NMI_INTERRUPT");
EXCEPTION(BREAKPOINT_EXCEPTION, "BREAKPOINT_EXCEPTION");
EXCEPTION(OVERFLOW_EXCEPTION, "OVERFLOW_EXCEPTION");
EXCEPTION(BOUND_RANGE_EXCEEDED_EXCEPTION, "BOUND_RANGE_EXCEEDED_EXCEPTION");
EXCEPTION(INVALID_OPCODE_EXCEPTION, "INVALID_OPCODE_EXCEPTION");
EXCEPTION(DEVICE_NOT_AVAILABLE_EXCEPTION, "DEVICE_NOT_AVAILABLE_EXCEPTION");
EXCEPTION(DOUBLE_FAULT_EXCEPTION, "DOUBLE_FAULT_EXCEPTION");
EXCEPTION(COPROCESSOR_SEGMENT_OVERRUN, "COPROCESSOR_SEGMENT_OVERRUN");
EXCEPTION(INVALID_TSS_EXCEPTION, "INVALID_TSS_EXCEPTION");
EXCEPTION(SEGMENT_NOT_PRESENT_EXCEPTION, "SEGMENT_NOT_PRESENT_EXCEPTION");
EXCEPTION(STACK_FAULT_EXCEPTION, "STACK_FAULT_EXCEPTION");
EXCEPTION(GENERAL_PROTECTION_EXCEPTION, "GENERAL_PROTECTION_EXCEPTION");
EXCEPTION(PAGE_FAULT_EXCEPTION, "PAGE_FAULT_EXCEPTION");
EXCEPTION(x87_FLOATING_POINT_ERROR, "x87_FLOATING_POINT_ERROR");
EXCEPTION(ALIGNMENT_CHECK_EXCEPTION, "ALIGNMENT_CHECK_EXCEPTION");
EXCEPTION(MACHINE_CHECK_EXCEPTION, "MACHINE_CHECK_EXCEPTION");
EXCEPTION(SIMD_FLOATING_POINT_EXCEPTION, "SIMD_FLOATING_POINT_EXCEPTION");

/** void UNKNOWN_EXCEPTION(); default unknown exception handler template
 *  Inputs: void
 *  Return value: none
 *  Function: masks interrupts, prints exception, then unmasks */
#define UNKNOWN_EXCEPTION(NAME, ERR_MSG) void NAME() { printf("%s\n", #ERR_MSG); }
UNKNOWN_EXCEPTION(UNKNOWN_INTERRUPT, "UNKNOWN_INTERRUPT");

/** void init_idt(); idt initializer
 *  Inputs: void
 *  Return value: 0 on success (always)
 *  Function: maps the appropriate traps/interrupts into the idt table */
int init_idt() {
    int i;
    /* Loop through idt and initialize table to unknown interrupt */
    for (i = 0; i < NUM_VEC; i++) {
        idt[i].seg_selector = KERNEL_CS;
        idt[i].reserved4 = 0x0;
        idt[i].reserved3 = 0x0, idt[i].reserved2 = 0x1, idt[i].reserved1 = 0x1;
        idt[i].size = 0x1;
        idt[i].reserved0 = 0x0;
        idt[i].dpl = 0x0;
        idt[i].present = 0x0;

        SET_IDT_ENTRY(idt[i], UNKNOWN_INTERRUPT);               
    }

    /* for reserved exceptions also change a bit to mark it as a trap */
    for (i = 0; i < RESERVED_EXCEPTIONS; i++) {
        idt[i].reserved3 = 0x1;
        idt[i].present = 0x1;
    }

    /* start manually setting the rest of the reserved exceptions */
    SET_IDT_ENTRY(idt[DIVIDE_ERROR_EXCEPTION_VEC], DIVIDE_ERROR_EXCEPTION);
    SET_IDT_ENTRY(idt[DEBUG_EXCEPTION_VEC], DEBUG_EXCEPTION);
    SET_IDT_ENTRY(idt[NMI_INTERRUPT_VEC], NMI_INTERRUPT);
    SET_IDT_ENTRY(idt[BREAKPOINT_EXCEPTION_VEC], BREAKPOINT_EXCEPTION);
    SET_IDT_ENTRY(idt[OVERFLOW_EXCEPTION_VEC], OVERFLOW_EXCEPTION);
    SET_IDT_ENTRY(idt[BOUND_RANGE_EXCEEDED_EXCEPTION_VEC], BOUND_RANGE_EXCEEDED_EXCEPTION);
    SET_IDT_ENTRY(idt[INVALID_OPCODE_EXCEPTION_VEC], INVALID_OPCODE_EXCEPTION);
    SET_IDT_ENTRY(idt[DEVICE_NOT_AVAILABLE_EXCEPTION_VEC], DEVICE_NOT_AVAILABLE_EXCEPTION);
    SET_IDT_ENTRY(idt[DOUBLE_FAULT_EXCEPTION_VEC], DOUBLE_FAULT_EXCEPTION);
    SET_IDT_ENTRY(idt[COPROCESSOR_SEGMENT_OVERRUN_VEC], COPROCESSOR_SEGMENT_OVERRUN);
    SET_IDT_ENTRY(idt[INVALID_TSS_EXCEPTION_VEC], INVALID_TSS_EXCEPTION);
    SET_IDT_ENTRY(idt[SEGMENT_NOT_PRESENT_EXCEPTION_VEC], SEGMENT_NOT_PRESENT_EXCEPTION);
    SET_IDT_ENTRY(idt[STACK_FAULT_EXCEPTION_VEC], STACK_FAULT_EXCEPTION);
    SET_IDT_ENTRY(idt[GENERAL_PROTECTION_EXCEPTION_VEC], GENERAL_PROTECTION_EXCEPTION);
    SET_IDT_ENTRY(idt[PAGE_FAULT_EXCEPTION_VEC], PAGE_FAULT_EXCEPTION);
    SET_IDT_ENTRY(idt[x87_FLOATING_POINT_ERROR_VEC], x87_FLOATING_POINT_ERROR);
    SET_IDT_ENTRY(idt[ALIGNMENT_CHECK_EXCEPTION_VEC], ALIGNMENT_CHECK_EXCEPTION);
    SET_IDT_ENTRY(idt[MACHINE_CHECK_EXCEPTION_VEC], MACHINE_CHECK_EXCEPTION);
    SET_IDT_ENTRY(idt[SIMD_FLOATING_POINT_EXCEPTION_VEC], SIMD_FLOATING_POINT_EXCEPTION);

    /* set the other interrupts, modifying it to be present */
    // idt[TIMER_CHIP_INTERRUPT_VEC].present = 0x1;
    // SET_IDT_ENTRY(idt[TIMER_CHIP_INTERRUPT_VEC], TIMER_CHIP_INTERRUPT);

    // store address of second para to first para, so second para should be like an address
    idt[KEYBOARD_INTERRUPT_VEC].present = 0x1;
    SET_IDT_ENTRY(idt[KEYBOARD_INTERRUPT_VEC], KEYBOARD_INTERRUPT);
    idt[RTC_INTERRUPT_VEC].present = 0x1;
    SET_IDT_ENTRY(idt[RTC_INTERRUPT_VEC], RTC_INTERRUPT);
    idt[PIT_INTERRUPT_VEC].present = 0x1;
    SET_IDT_ENTRY(idt[PIT_INTERRUPT_VEC], PIT_INTERRUPT);

    /* change bits for syscalls, ring 3 for syscalls as should be allowed to be called from userspace */
    idt[SYSCALL_INTERRUPT_VEC].reserved3 = 0x1, idt[SYSCALL_INTERRUPT_VEC].dpl = 0x3;
    idt[SYSCALL_INTERRUPT_VEC].present = 0x1;
    SET_IDT_ENTRY(idt[SYSCALL_INTERRUPT_VEC], SYSCALL_INTERRUPT);
    
    /* load the idt */
    lidt(idt_desc_ptr);

    return 0;
}
