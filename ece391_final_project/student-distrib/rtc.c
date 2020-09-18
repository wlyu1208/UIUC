/* rtc.c - initializing and handling the real time clock
rtc read, rtc write rtc open and close 
 * virtualize the rtc 
 */
#include "rtc.h"
#include "syscall.h"

//volatile static int32_t read_rtc; // read flag ( does initialization matter ?)
// static int row_count; // to print 1 for each rtc 

/* rtc_init
 * description: initialize the rtc with correct irq number
 * inputs: none
 * outputs: none
 * side effect: enable rtc irq to initialize, sets periodic interrupts at 1024 Hz */
void rtc_init() {
  	int32_t flags;
	uint8_t prev;
	uint8_t rate;
	uint32_t irq = RTC_INTR_NUM;   
	cli_and_save(flags);			// turn off all interrupts;
    outb(0x8B, RTC_ADDR_PORT);	// select register B, and disable NMI
	prev = inb(RTC_DATA_PORT);	// read the current value of register B
	outb(0x8B, RTC_ADDR_PORT);	// set the index again (a read will res				et the index to register D)
	outb(prev | 0x40, RTC_DATA_PORT);	// write the previous value ORed with 0x40. This turns on bit 6 of register B
	
	// initialize to 1024 Hz
	rate = RTC_OPEN_RATE; // rate is 15 now
	rate &= 0x0F;	// rate must be 15 or less
	outb(0x8A, RTC_ADDR_PORT);	// set index to register A, disable NMI
	prev = inb(RTC_DATA_PORT);	// get initial value of register A
	outb(0x8A, RTC_ADDR_PORT);	// reset index to A
	outb((prev & 0xF0) | rate, RTC_DATA_PORT);	// write only our rate to A. Note, rate is the bottom 4 bits.
	
	restore_flags(flags);
	enable_irq(irq); //Init the RTC
}

/* rtc_handler
 * description: handles rtc interrupts
 * inputs: none
 * outputs: none
 * side effect: handle rtc for each running process with its own frequency
 *  */
void rtc_handler() {
	uint32_t irq; 
	uint32_t i;
	cli();
	irq = RTC_INTR_NUM;
    send_eoi(irq);
	
	outb(0x0C, RTC_ADDR_PORT); // select register C
	inb(RTC_DATA_PORT); //throw it out

	/* Go each process and handle rtc frequency of that process*/
    pcb_t* pcb;
	int process_id;
	for (i = 0; i < terminal_max_number; i++){
		process_id = terminals[i].running_process;
		// get process for each terminal
		// if(process_id < terminal_max_number){
		// 	continue;  
		// }
		pcb = find_pcb_by_process_number(process_id);
		if (pcb->pcb_freq == 0) {
			continue;
		}	
		if (pcb->pcb_freq_wait != 0) { // if it is not 0 than waiting for reading
			pcb->pcb_freq_wait--;
		}
	}
	sti();
}

/* rtc_open
 *	Descrption:	open a rtc descriptor for a process (virtualized to start at default 2hz).
 *	Parameter:
 *		filename: (not used) use NULL in this argument.
 * 	RETURN: 0 if success, -1 for failure
		halt the system if too many RTC is opend in this system.
  */
int32_t rtc_open(const uint8_t *filename){
 	uint32_t freq;
	//int freq_pow = 0;
	pcb_t* pcb;

	/* get current pcb, set pcb freq after doing sanity check  */
	pcb = get_current_pcb();
	freq = 2; //2hz
	pcb->pcb_freq = freq;
	pcb->pcb_freq_wait = 0;
	return 0;
}

/* rtc_close
 *	Descrption:	close a rtc descriptor for a process.
 *	Parameter: 	file descriptor for rtc
 * 	RETURN:	0 for  success , -1 for failure
  */
int32_t rtc_close(int32_t fd){
	if ((fd < 2) | (fd > 7)) return -1;
    // do nothing
	return 0;
}

/* rtc_read
 *	Descrption:	a user blocking function intended to wait for the next RTC interrupt.
 *	Parameter:
 *		buf: (not used) Use NULL in this argument
 *		nbytes: number of bytes to read (not used here)
 *		fd: RTC file descriptor
 * 	RETURN: 0 always
*/
 int32_t rtc_read(int32_t fd,  void * buf,  int32_t nbytes){
	
	/* get pcb and set frequency of that pcb */
	cli();
	pcb_t* pcb = get_current_pcb();
	if (pcb->pcb_freq != 0){
		pcb->pcb_freq_wait = RTC_USER_MAX_FREQ/pcb->pcb_freq;
	}
	sti();
    while (pcb->pcb_freq_wait != 0);
    return 0;
	
 }

 /* rtc_write
  *	Descrption:	Set the virtual RTC Hardware periodic interrupt frequency
  *
  *	Parameter: buf: must be a power of 2 and inside the range of [2,8192]
				nbytes: must be 4 
				fd: file descriptor for RTC 
	RETURN: -1 if failed, 0  if success     */
int32_t rtc_write(int32_t fd, const void* buf , int32_t nbytes){
 uint32_t freq;
//int freq_pow = 0;
 pcb_t* pcb;
 if (!buf) return -1;
 if (nbytes != 4)return -1; // freq should be 4 byte int 
// if user program gives kernel address return -1

/* get current pcb, set pcb freq after doing sanity check  */
pcb = get_current_pcb();
freq = *(int32_t*)buf ;
if (freq & (freq-1)) return -1; // if not a power of 2
if ( freq < 2 &&  freq > RTC_USER_MAX_FREQ) return -1; 
pcb->pcb_freq = freq;
pcb->pcb_freq_wait = 0;

   // convert freq to a power of 2
	//   while ( freq > 1){
	// 		freq_pow++;
	// 				freq =  freq >> 1;
	//    }

	//   rate = 15 - freq_pow + 1;  // max freq = 2^15, 
	//   rate &= 0x0F;	// rate must be 15 or less
	//   cli();
	//   outb(0x8A, RTC_ADDR_PORT);	// set index to register A, disable NMI
	//   prev = inb(RTC_DATA_PORT);	// get initial value of register A
	//   outb(0x8A, RTC_ADDR_PORT);	// reset index to A
	//   outb((prev & 0xF0) | rate, RTC_DATA_PORT);
	//   sti();
  return 0;
}

