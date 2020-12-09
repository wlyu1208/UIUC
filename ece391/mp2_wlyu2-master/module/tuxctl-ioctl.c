/* tuxctl-ioctl.c
 *
 * Driver (skeleton) for the mp2 tuxcontrollers for ECE391 at UIUC.
 *
 * Mark Murphy 2006
 * Andrew Ofisher 2007
 * Steve Lumetta 12-13 Sep 2009
 * Puskar Naha 2013
 */

#include <asm/current.h>
#include <asm/uaccess.h>

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/file.h>
#include <linux/miscdevice.h>
#include <linux/kdev_t.h>
#include <linux/tty.h>
#include <linux/spinlock.h>

#include "tuxctl-ld.h"
#include "tuxctl-ioctl.h"
#include "mtcp.h"

#define debug(str, ...) \
	printk(KERN_DEBUG "%s: " str, __FUNCTION__, ## __VA_ARGS__)

/************************ Protocol Implementation *************************/

#define button_length		8
#define F_bitmask		  0xF
#define second_f_bit     0xF0
#define low_to_high			4
#define trigger				1
#define byte 			    1
#define led_package			6

// variable to get each number of bit
#define fourth				0x8
#define third				0x4
#define second				0x2
#define first				0x1

#define full_four_hex		0xFFF
#define fifth_full			0xF0000
#define seventh_full		0xF000000
#define first_bit			0xF000
#define second_bit			0xF00
#define thrid_bit			0xF0




unsigned char on[2];
unsigned char in; //variales to pass opcode 

unsigned int state=0; // tell tux done work
unsigned char R, D, L, U; //variable to get R D L U value separte

/* Data of segments for each data to led*/
const unsigned char data_led[16]= {0xE7, 0x06, 0xCB, 0x8F, 0x2E, 0xAD, 
									0xED, 0x86, 0xEF, 0xAF, 0xEE, 0x6D,
									0xE1, 0x4F, 0xE9, 0xE8};

unsigned char button_index; // button index for RLDUCBAstart
spinlock_t lock; // initial lock for synchronization
unsigned int flag=0; //tell that reset happened
unsigned int num; //use to get number for argument in setting LED


/* array to save LED for keep data for reset*/
unsigned char saved_led[led_package];


/* tuxctl_handle_packet()
 * IMPORTANT : Read the header for tuxctl_ldisc_data_callback() in 
 * tuxctl-ld.c. It calls this function, so all warnings there apply 
 * here as well.
 */

/* 
 * tuxctl_handle_packet
 *   DESCRIPTION: 
 *   INPUTS: tty, packet -- receieve comand from tty or serial port dirver,
 * 	 and packet receivied by computer from Tux controller 
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: need to spin_lock and unlock in order to keep state since 
 * 	 value can get mixed up by not synchronizing
 * 	 sends info tux to kernel
 */
void tuxctl_handle_packet (struct tty_struct* tty, unsigned char* packet)
{
    unsigned a, b, c;

    a = packet[0]; /* Avoid printk() sign extending the 8-bit */
    b = packet[1]; /* values when printing them. */
    c = packet[2];
    
    //printk("packet : %x %x %x\n", a, b, c); 
	
	switch(a){
		case MTCP_ACK:{ // for ack set state to 0 and turn on LED if flag is 1
			spin_lock(&lock);
			state = 0;
			spin_unlock(&lock);
			if(flag==trigger){
				spin_lock(&lock);
				state=trigger;
				flag=0;
				spin_unlock(&lock);
				tuxctl_ldisc_put(tty, saved_led, led_package); 
				// if flag was 1, than print old LED on reset mode
			}
			break;
		}
		case MTCP_RESET: //initial button, LED for user and turn off debug and set flag 1
			spin_lock(&lock);
			state = trigger;
			in = MTCP_BIOC_ON; 
			tuxctl_ldisc_put(tty, &in, byte);
			in = MTCP_LED_USR;
			tuxctl_ldisc_put(tty, &in, byte);
			in = MTCP_DBG_OFF;
			tuxctl_ldisc_put(tty, &in, byte);
			state = 0;
			flag=trigger;
			spin_unlock(&lock);
			break;
		case MTCP_BIOC_EVENT: // get button value
			spin_lock(&lock);
			state = trigger;
			R = (c & fourth);
			D = (c & third) >> trigger;
			L = (c & second) << trigger;
			U = (c & first);
			button_index = ((R | D | L | U) << low_to_high) & second_f_bit;
			button_index =(button_index | (b & F_bitmask)); // set correct value on correct index
			//printk("button : %x \n", button_index);
			state = 0;
			spin_unlock(&lock);
			break;
		default: break;
		
	}
}

/******** IMPORTANT NOTE: READ THIS BEFORE IMPLEMENTING THE IOCTLS ************
 *                                                                            *
 * The ioctls should not spend any time waiting for responses to the commands *
 * they send to the controller. The data is sent over the serial line at      *
 * 9600 BAUD. At this rate, a byte takes approximately 1 millisecond to       *
 * transmit; this means that there will be about 9 milliseconds between       *
 * the time you request that the low-level serial driver send the             *
 * 6-byte SET_LEDS packet and the time the 3-byte ACK packet finishes         *
 * arriving. This is far too long a time for a system call to take. The       *
 * ioctls should return immediately with success if their parameters are      *
 * valid.                                                                     *
 *                                                                            *
 ******************************************************************************/

/* 
 * tuxctl_ioctl
 *   DESCRIPTION: 
 *   INPUTS: tty, file -- receieve comand from tty or serial port dirver,
 * 	 and  file with command and argument
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: send data kernel to tux such as button and led or initialization
 */
int 
tuxctl_ioctl (struct tty_struct* tty, struct file* file, 
	      unsigned cmd, unsigned long arg)
{	
    switch (cmd) {
	case TUX_INIT:
		spin_lock_init(&lock);

		on[0] = MTCP_BIOC_ON; 
		on[1] = MTCP_LED_USR;
		tuxctl_ldisc_put(tty, on, 2); // after lock send package with button and led initialize

		spin_lock(&lock);
		state = 0;
		button_index = 0;
		spin_unlock(&lock); //change state and button_index;
		return 0;
	case TUX_BUTTONS:{
		unsigned long return_val;
		if(!tty || arg==0){
			return -EINVAL;
		} // check parameter are correct and in range
		spin_lock(&lock);
		return_val = copy_to_user((int*) arg, &button_index, 1);
		spin_unlock(&lock); //get data from tux to copy to user
		if(return_val!=0){
			return -EINVAL;
		} //if copy to user result is not 0 than fail
		return 0;
		}
	case TUX_SET_LED: 
		{ 
		unsigned int led_idx, decimal_point, bitmask;
		unsigned int number_value[low_to_high];
		unsigned char package[led_package]; //initialize neccessary vairiables
		int i = 0;
		spin_lock(&lock);
		if(state==1){
			return 0;
		} // check state, if fine keep going
		spin_unlock(&lock);
		num = arg & full_four_hex;
		led_idx = (arg & fifth_full) >> 16;
		decimal_point = (arg & seventh_full) >> 24;

		number_value[3] = (num & first_bit) >> 12;
		number_value[2] = (num & second_bit) >> 8;
		number_value[1] = (num & thrid_bit) >> 4;
		number_value[0] = (num & F_bitmask); // get each one digit and put on array 
		package[0]=MTCP_LED_SET;
		package[1]=F_bitmask;
		bitmask=0x01;
		for(i = 0; i < low_to_high; i++){
			if(bitmask<<i & led_idx ){ // put led only where led idx told to
				number_value[i]=data_led[ number_value[i] ]; 
				// get segment data for each number and store back
				if(decimal_point & bitmask<<i){
					number_value[i] |= 0x10;
				}
				package[i+2]=number_value[i]; //first two package is already full
			}
			else{
				package[i+2]=0; // if nothing than put 0
			}
		}

		for(i = 0; i < led_package; i++){
			saved_led[i]=package[i]; // save led data for in case of Reset
		}
		spin_lock(&lock);
		state=trigger;
		spin_unlock(&lock); //change state that it functions
		tuxctl_ldisc_put(tty, package, 6); // send package to turn on the led
		return 0;
		}
	case TUX_LED_ACK:
		return 0;
	case TUX_LED_REQUEST:
		return 0;
	case TUX_READ_LED:
		return 0; //others do not have to implement
	default:
	    return -EINVAL;
    }
}




