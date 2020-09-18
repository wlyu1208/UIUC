/* rtc.h - header file for initializing and handling the real time clock
 * vim:ts=4 noexpandtab
 */
#ifndef _RTC_H
#define _RTC_H

//#ifndef ASM

#include "types.h"
#include "idt.h"
#include "x86_desc.h"
#include "i8259.h"
#include "lib.h"
#include "syscall.h"
#include "terminal.h"

#define RTC_ADDR_PORT 0x70
#define RTC_DATA_PORT 0x71
#define RTC_INTR_NUM 0x08
#define RTC_OPEN_RATE 0x06 // freq 1024 Hz

#define RTC_USER_MAX_FREQ 1024
#define terminal_max_number 3

/* rtc initialization function */
extern void rtc_init();

/* handle rtc interrupts */
extern void rtc_handler();


// initializes RTC freq
extern int32_t rtc_open(const uint8_t *filename);

// does nothing
extern int32_t rtc_close(int32_t fd);

//block until next interrupt
extern int32_t rtc_read(int32_t fd, void* buf, int32_t nbytes);

// chnage RTC freq
extern int32_t rtc_write(int32_t fd, const void* buf , int32_t nbytes);

#endif
