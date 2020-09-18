/* scheduler.h: header file for initializing and handling the PIT and scheduling */

#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "types.h"

#define BASE_FREQ           1193182
#define DIVIDE_100_HZ       11931
#define CHANNEL_0_PORT      0x40    
#define CMD_PORT            0x43
#define INIT_CMD            0x36  /* 0b00110110 channel 0, lo+hibyte, square gen */
#define BYTEMASK            0xFF
#define BYTESIZE            8
#define PIT_IRQ             0x00

/* pit initialization function */
extern void pit_init();

/* handle pit interrupts (scheduler) */
extern void pit_handler();

/* which terminal scheduler is on */
uint32_t schedule_terminal_num;

#endif
