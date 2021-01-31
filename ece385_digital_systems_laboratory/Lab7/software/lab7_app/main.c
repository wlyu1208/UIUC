// Main.c - makes LEDG0 on DE2-115 board blink if NIOS II is set up correctly
// for ECE 385 - University of Illinois - Electrical and Computer Engineering
// Author: Zuofu Cheng

//int main()
//{
//	int i = 0;
//	volatile unsigned int *LED_PIO = (unsigned int*)0x00000090; //make a pointer to access the PIO block
//
//	*LED_PIO = 0; //clear all LEDs
//	while ( (1+1) != 3) //infinite loop
//	{
//		for (i = 0; i < 100000; i++); //software delay
//		*LED_PIO |= 0x1; //set LSB
//		for (i = 0; i < 100000; i++); //software delay
//		*LED_PIO &= ~0x1; //clear LSB
//	}
//	return 1; //never gets here
//}



int main(){
	int s=0;
	volatile unsigned int *LED_PIO=(unsigned int*)0x00001090;
	volatile unsigned int *SWITCH_PIO=(unsigned int*)0x00000080;
	volatile unsigned int *KEY3_PIO=(unsigned int*)0x00000060;
	volatile unsigned int *KEY2_PIO=(unsigned int*)0x00000070; //pointers to access PIOs
	int i=0;
	*LED_PIO=0; //initialize LED and i for loop

	while(1==1){ //when always true
		if(!(*KEY2_PIO)){
			s=0;
		} //reset sum if key2 is pressed
		else if(!(*KEY3_PIO)){ //when key3 is pressed
			s=s+*SWITCH_PIO; //add s to switch inputs
			if(s>=256){
				s=s-256;
			} //if s exceeds 256 then it subtracts 256
		}

		for (i = 0; i < 100000; i++); //software delay
		*LED_PIO = s; //set LED to s
	}
	return 1; //never gets here
}


