/* tests.c - tests for every component in this mp
 * vim:ts=4 noexpandtab
 */

#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "idt.h"
#include "rtc.h"
#include "filesystem.h"
#include "terminal.h"
#include "syscall.h"

#define PASS 1
#define FAIL 0

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result==PASS) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15");
}


/* Checkpoint 1 tests */

/* IDT Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test(){
	TEST_HEADER;

	int i;
	int result = PASS;
	int interrupt_test[4]={PIT_INTERRUPT_VEC,KEYBOARD_INTERRUPT_VEC
							,RTC_INTERRUPT_VEC,SYSCALL_INTERRUPT_VEC}; // index of interrupt
	for (i = 0; i < 20; ++i){ // from 0x0 to 0x13
		if ((idt[i].offset_15_00 == NULL) && 
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}
	for (i = 0; i < 4; i++){ 
		int index = interrupt_test[i];
		if ((idt[index].offset_15_00 == NULL) && 
			(idt[index].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}

	// printf("past adddress\n");
	// test zero divide
	// int a,b;
	// a=0;
	// b = 4/a;
	// printf("pass zero divide\n");
	//
	return result;
}


/* Paging initialization test
 *
 * test NULL pointer, kernel 
 * inputs: None
 * outputs: PASS/FAIL
 * Side effects: None
 * Coverage: 
 * File: page.h, page,c
 */
unsigned int test_kernel()
{
	TEST_HEADER;
	// asign an array and it should be in kernel memory.
	unsigned int i,b;
	int* a;
	int result[2];
	for(i=0;i<2;i++) // define an array
	{
		result[i]=10;  // 10 is just random data.
	}
	int kernels = 0x400000; // start address of kernel
	int kernele = 0x800000; // end address of kernel
	int address =  (unsigned int)(&result[0]); // address of first element
	printf("Testing paging: result at %x value: %d\n",&result[0],result[0]);
	if ((address&kernels)&&(address<kernele)){ //start of kernel memory and the end of kernel memory
	}
	else
	{
		return FAIL; // if not in kernel memory, fail
	}
	
	address = (unsigned int)&result[1];
	printf("Testing paging: result at %x value: %d\n",&result[1],result[1]);
	if ((address&kernels)&&(address<kernele)){//start of kernel memory and the end of kernel memory
	}
	else
	{
		return FAIL;
	}
	// loop over kernel memory but some bytes are reserved.
	for(i=kernels;i<kernele-3;i++) // iterate over kernel page, some bytes are reserved maybe for stack use.
	{
		a = (int*) i;
		b = *(a);
		// printf("stop at %x\n",i);
	}
	b = PASS;
	return b;
	// test -100 pointer

}

/* test_negative_pointer()
 *
 * test negative pointer
 * inputs: None
 * outputs: PASS/FAIL
 * Side effects: None
 * Coverage: 
 * File: page.h, page,c
 */
int test_negative_pointer()
{
	TEST_HEADER;
	int* a = (signed int*)-100; // assign a negtive pointer
	int b = *(a);
	b = FAIL;
	return b;
}
/* test_null
 *
 * test NULL pointer 
 * inputs: None
 * outputs: PASS/FAIL
 * Side effects: None
 * Coverage: File: page.h, page,c
 */
int test_null()
{
	TEST_HEADER;
	int* a = NULL;
	int b = *(a);
	b = FAIL;
	return b;
}

/* test_zero_divide()
 *
 * test zero divided
 * inputs: None
 * outputs: PASS/FAIL
 * Side effects: None
 * Coverage: File: idt.c
 */
int test_zero_divide()
{
	TEST_HEADER;
	int a =0;
	int b;
	b = 4/a;
	b = FAIL;
	return b;
}

/* test_outside_kernel
 *
 * test not present page
 * inputs: None
 * outputs: PASS/FAIL
 * Side effects: None
 * Coverage: File: page.c,page.h
 */
int test_not_present()
{
	TEST_HEADER;
	int edge = 0x399999; // the edge of kernel page
	int* a=(int*)edge; // not present 
	int b = *(a);
	b = FAIL;
	return b; // if no exception, fail
}


/* test_video_memory()
 *
 * test video memory
 * inputs: None
 * outputs: PASS/FAIL
 * Side effects: None
 * Coverage: File: page.c,page.h
 */
int test_video_memory()
{
	TEST_HEADER;
	int video_mem = 0xB8000;
	int i,b;
	for(i=0;i<0x1000-3;i++) // 0x1000 = 4kb page, 3 byte reserved.	
	{
		int* a = (int*)(video_mem+i);
		b = *(a);
		// printf("video memory is: %x\n",b);
	}
	b = PASS;
	return b; // no exception, succeed.
}

/* test_values_contained_in_page()
 *
 * test values contained in page
 * inputs: None
 * outputs: PASS/FAIL
 * Side effects: None
 * Coverage: File: page.c,page.h
 */
// test_values_contained_in_page()
// {
// 	TEST_HEADER;
// 	int i;
// 	int result = PASS;
// 	int* a;
// 	int b;
// 	for (i = 0; i < 0x800000; ++i)
// 	{
// 		a = (int*)i;
// 		b = *(a);
// 	}
// }

// add more tests here

/* Checkpoint 2 tests */

/* checks if rtc open works
parameter: none;
return: always 1
* should print TICK 2 times a second
*/
int test_rtc_open(){
	if (0 == rtc_open(NULL)) return 1;  // 3 doesn't matter, frq = 2 HZ
	return 0;
}

/* checks if rtc write works
parameter: none;
return: always 1
* should change the freq to 32 HZ
*/
int test_rtc_write(){
	int32_t freq = 256;
	void * buffer = &freq;
	if (0 == rtc_write(3, buffer, 4)) return 1; // 1st arg: always 3, 3rd arg: always 4, 2nd arg: buffer containing freq
	return 0;
}

/* checks if rtc read works
parameter: none;
return: always 1
* should print "RTC IS READING! " continuously
*/
int  test_rtc_read(){
	if ( 0 == rtc_read(3, NULL, 4)) return 1; // 3 doesn't matter, 2nd arg always NULL, 3rd arg always 4
	return 0;
}

/* checks if rtc close works
parameter: none;
return: always 1
* 
*/
int test_rtc_close(){
	if (0 == rtc_close(5)) return 1;  //parameter of rtc_close: less than  8 and more than 1
	return 0;
}

/* test_dir_list
 * description: prints out file names of directory
 * input: none
 * output: result of test (PASS/FAIL)
 * side effect: prints directory content to screen
 */
int test_dir_list() {
	int32_t fd, cnt;
    uint8_t buf[33];
	const uint8_t curr_dir[] = ".";
	/* reset screen */
	clear_screen();
	place_cursor(0, 0);
	printf("Listing files:\n");
	/* open directory file */
    if (-1 == (fd = dir_open(curr_dir))) {
        printf("Directory open failed\n");
        return FAIL;
    }
	/* read each filename and put each character to screen until read all filenames */
    while (0 != (cnt = dir_read(fd, buf, 32))) {
        if (-1 == cnt) {
	        printf("Directory entry read failed\n");
	        return FAIL;
	    }
	    buf[cnt] = '\n';
		if (-1 == terminal_write(1, buf, cnt + 1)) return FAIL;
    }
    return PASS;
}

/* test_file_by_name
 * description: prints out file of a certain name
 * input: fname - char array representing file name to print out
 * output: result of test (PASS/FAIL)
 * side effect: prints file content to screen
 */
int test_file_by_name(const int8_t* fname) {
	int32_t fd, cnt;
    uint8_t buf[1024];
	/* reset screen */
	clear_screen();
	place_cursor(0, 0);
	/* open file */
    if (-1 == (fd = file_open((uint8_t*)fname))) {
        printf("File not found\n");
		return FAIL;
    }
	/* read and put each character to screen until end of file */
    while (0 != (cnt = file_read(fd, buf, 1024))) {
        if (-1 == cnt) {
			printf("File read failed\n");
			return FAIL;
		}
		if (-1 == terminal_write(1, buf, cnt)) return FAIL;
	}
	/* close file */
	file_close(fd);
    return PASS;
}

/* test_file_by_index
 * description: prints out file located in a specific dentry index
 * input: index - dentry index of file to print out
 * output: result of test (PASS/FAIL)
 * side effect: prints file content to screen
 */
int test_file_by_index(uint32_t index) {
	int32_t fd, cnt;
    uint8_t buf[1024];
	/* reset screen */
	clear_screen();
	place_cursor(0, 0);
	/* open file */
    if (-1 == (fd = file_open_index(index))) {
        printf("File not found\n");
		return FAIL;
    }
	/* read and put each character to screen until end of file */
    while (0 != (cnt = file_read(fd, buf, 1024))) {
        if (-1 == cnt) {
			printf("File read failed\n");
			return FAIL;
		}
		if (-1 == terminal_write(1, buf, cnt)) return FAIL;
	}
	/* close file */
	file_close(fd);
    return PASS;
}

/* test_file_bad_inputs
 * description: tests various inputs that are incorrect to check behavior
 * input: none
 * output: result of test (PASS/FAIL)
 * side effect: runs bad inputs that may or may not work (and break the kernel)
 */
int test_file_bad_inputs() {
	int32_t fd, cnt;
    uint8_t buf[1024];
	/* initialize some random file names */
	const uint8_t test_file[] = "frame0.txt";
	const uint8_t test_file2[] = "frame1.txt";
	const uint8_t test_file_not_exist[] = "oogabooga.txt";
	/* reset screen */
	clear_screen();
	place_cursor(0, 0);
	/* check if can read file without opening */
	if (-1 != (cnt = file_read(fd, buf, 1024))) {
		printf("Can read files without opening\n");
		return FAIL;
	}
	/* check if can open a null file */	
	if (-1 != (fd = file_open(NULL))) {
		printf("Can open null files\n");
		return FAIL;
	}
	/* check if can open a nonexistant file */
	if (-1 != (fd = file_open(test_file_not_exist))) {
		printf("Can open nonexistant files\n");
		return FAIL;
	}
	/* check if can open a nonexistant indexed file */
	if (-1 != (fd = file_open_index(999))) {
		printf("Can open nonexistant indexed files\n");
		return FAIL;
	}
	/* check if can close an unopened file */
	if (-1 != (file_close(fd))) {
		printf("Can close an unopened file\n");
		return FAIL;
	}
	/* check if can open a valid file */
	if (-1 == (fd = file_open(test_file))) {
        printf("File not found\n");
		return FAIL;
    }
	/* check if can read file into a nonexistant buffer */
	if (-1 != (cnt = file_read(fd, NULL, 1024))) {
		printf("Can read files into nonexistant buffer\n");
		return FAIL;
	}
	/* read from file an absurd amount of bytes, should be fine and terminate without any problems */
	while (0 != (cnt = file_read(fd, buf, 99999999))) {
		/* should either finish or escape */
        if (-1 == cnt) break;
	}
	/* check if can open file while current file open */
	if (-1 != (fd = file_open(test_file2))) {
		printf("Can open another file while one is currently open\n");
		return FAIL;
	}
	/* check if can close file */
	if (-1 == (file_close(fd))) {
		printf("Failed to close file\n");
		return FAIL;
	}
	return PASS;
}

/* test_terminal_open()
 * description: test the open default terminal function
 * input: none
 * output:none
 * side effect: set terminal variable to 0
 */
int test_terminal_open(){
	terminal_open((uint8_t*)1);
	return PASS;
}

/* test_terminal_close()
 * description: test the close default terminal function
 * input: none
 * output:none
 */
int test_terminal_close(){
	int result;
	result = terminal_close(2);
	if(result == 0){
		return PASS;
	}
	else
	{
		return FAIL;
	}
}

/* test_terminal_read()
 * description: test the read default terminal function
 * input: none
 * output:none
 * side effect: set terminal buffer to 111111111,size to 11
 */
int test_terminal_read()
{
	int i,result;
	unsigned char buffer[128];
	for(i=0;i<127;i++)
	{
		buffer[i]='1';
	}
	buffer[127]=0;
	puts((int8_t*)buffer);
	// puts('');
	// terminal_write();
	result = terminal_read(6,buffer,128);
	puts((int8_t*)buffer);
	if(result == 128){
		return PASS;
	}
	else{
		return FAIL;
	}
	// terminal_write();
}

/* test_terminal_write()
 * description: test the write default terminal function
 * input: none
 * output:none
 * side effect: output buffer
 */
int test_terminal_write()
{
	uint8_t buffer[10];
	buffer[0]='2';
	buffer[1]='5';
	buffer[2]='8';
	buffer[3]=0;
	int result;
	result = terminal_write(6, buffer, 4);
	if(result == 4){
		return PASS;
	}
	else{
		return FAIL;
	}
}
int test_execute()
{
	printf("into execute\n");
	execute((uint8_t*)"shell");
	return 0;
}
/* Checkpoint 3 tests */

/* test_systemcall_read()
 * description: test the read default systemcall function
 * input: none
 * output:none
 * side effect: output buffer
 */
int test_read()
{
	return PASS;
}

/* test_systemcall_write()
 * description: test the write default systemcall function
 * input: none
 * output:none
 * side effect: output buffer
 */
int test_systemcall_write()
{
	return PASS;
}

/* checks if system call open works
parameter: none;
return: always 1
*/
int  test_open(){
	if ( -1 != open(NULL)) return 1; 
	return 0;
}

/* checks if  system call close works
parameter: none;
return: always 1
*/
int  test_close(){
	if ( -1 != close(5)) return 1; 
	return 0;
}

int test_syscall_counter()
{
	// execute((uint8_t*)"testprint");
	printf("%d",execute((uint8_t*)"counter"));
	return PASS;
	// execute((uint8_t*)"shell");
}

int test_syscall_testprint()
{
	// execute((uint8_t*)"testprint");
	printf("%d",execute((uint8_t*)"testprint"));
	return PASS;
	// execute((uint8_t*)"shell");
}

int test_all_syscall()
{
	char new_exe[9][20] = {"testprint","hello","counter","syserr","ls""cat hello","grep /\\","pingpong","fish"};
	int i;
	for(i=0;i<9;i++)
	{
		execute((uint8_t*)new_exe[i]);
	}
	return PASS;
}



/* Checkpoint 4 tests */
/* Checkpoint 5 tests */

/* launch_tests 
 * description: Test suite entry point 
 * input: none
 * output: none
 * side effect: runs various tests that may or may not work (and break the kernel)
 */
void launch_tests() {
	// TEST_OUTPUT("idt_test", idt_test());	
	// TEST_OUTPUT("test zero divided",test_zero_divide());
	// TEST_OUTPUT("test kernel page",test_kernel());
	// TEST_OUTPUT("test video memory",test_video_memory());
	// TEST_OUTPUT("test null pointer dereference",test_null());
	// TEST_OUTPUT("test negative pointer dereference",test_negative_pointer());
	// TEST_OUTPUT("test not present",test_not_present());
	// TEST_OUTPUT("RTC OPEN test",test_rtc_open());
	// TEST_OUTPUT("RTC CLOSE test",test_rtc_close());
	// TEST_OUTPUT("RTC READ test",test_rtc_read());
	// TEST_OUTPUT("RTC WRITE test",test_rtc_write());
	// TEST_OUTPUT("DIR LIST TEST", test_dir_list());
	// TEST_OUTPUT("FILE READ TEST", test_file_by_name("frame0.txt"));
	// TEST_OUTPUT("FILE READ TEST", test_file_by_name("verylargetextwithverylongname.txt"));
	// TEST_OUTPUT("FILE READ TEST", test_file_by_name("cat"));
	// TEST_OUTPUT("FILE READ TEST", test_file_by_name("ls"));
	// TEST_OUTPUT("FILE READ BY INDEX TEST", test_file_by_index(10));
	// TEST_OUTPUT("BAD INPUTS", test_file_bad_inputs());
	// TEST_OUTPUT("Termianl Open Test", test_terminal_open());
	// TEST_OUTPUT("Terminal Close Test", test_terminal_close());
	// TEST_OUTPUT("terminal read test",test_terminal_read());
	// TEST_OUTPUT("terminal write test",test_terminal_write());
	// TEST_OUTPUT("syscall read test",test_read());
	//TEST_OUTPUT("syscall write test",test_write());
	//TEST_OUTPUT("syscall open test",test_open());
	//TEST_OUTPUT("syscall close test",test_close());
	// TEST_OUTPUT("SYSCALL TEST",test_syscall_counter());
	// TEST_OUTPUT("SYSCALL TEST",test_all_syscall());
	// TEST_OUTPUT("execute test",test_execute());
	// launch your tests here
}
