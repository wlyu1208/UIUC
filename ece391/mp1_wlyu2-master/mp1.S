
# mp1.S - life-or-death version
# Good luck, and have fun!

.data
	
    # Constants for accessing the fields of a struct keystroke_args
    # struct keystroke_args is defined in rtc.h and mp1.h

	KA_SIZE	  = 14   # 14 bytes when copying
	KA_STACK  = 16   # use 16 bytes on stack for alignment
	DIRECTION = 0
	SELECTOR  = 2
	GUESS     = 4
	HINT      = 9

    # Constants for accessing the fields of a struct game_status
    # struct game_status is defined in rtc.h and mp1.h

	GS_SIZE   = 8	# 8 bytes
	POP 	  = 0
	INF	  = 4

    # Constants for screen size

    	SCR_WIDTH  = 80
	SCR_HEIGHT = 20
	SCR_SIZE   = ((SCR_WIDTH) * (SCR_HEIGHT))

    # ASCII cell images

        CELL_LIVE = 0x78
	CELL_DEAD = 0x20

# Data shared between this file and rtc.c helper functions

# This '.globl' directive makes these symbols visible externally
.globl rand_seed, aggression

			.align 8
rand_seed:		.quad 0x0	# Pseudo-random number seed
aggression:		.long 0x0	# Aggression level of virus


# Data private to this file

current_board:		.long 0x0	# Address of current board
next_board:		.long 0x0	# Address of next board
population:		.long 0x0	# Remaining human population
infection:		.long 0x0	# Count of live virus cells
select_image:		.string "==>"	# Picture of the selector
select_blank:		.string "   "	# Picture to overwrite selector
base_array:             .string "ACGT"  # Array of characters for bases

.text
jumptable: 
.long mp1_ioctl_startgame, mp1_ioctl_endgame, mp1_ioctl_keystroke, mp1_ioctl_getstatus, mp1_ioctl_vaccinate

# void mp1_poke(void);
# You can use this function to write to video memory.
#
# Interface: Register-based arguments (not C-style)
#    Inputs: %cl  - The byte you wish to write
#            %eax - Offset from the start of video memory that you wish
#                   to write to
#   Outputs: Text-mode video screen is written to at location %eax with
#            the byte in %cl
# Registers: Clobbers EDX

mp1_poke:
		movl    vmem_base_addr(,1),%edx
		movb    %cl,(%edx,%eax,1)
		ret

#   ----------------- Exported functions ---------------------

# void mp1_rtc_tasklet(unsigned long ignore);
# Performs three tasks:
#	(1) updates the life game board and calculates infection count
#	(2) swaps the two boards
#	(3) applies population reduction based on infection count
#	(4) redraws the screen
#	(5) notifies the user code by calling mp1_notify_user
# Inputs   : none
# Outputs  : none
# Registers: Standard C calling convention

.globl mp1_rtc_tasklet
mp1_rtc_tasklet:

	pushl %ebp
	movl %esp, %ebp # new stack base with save old ebp

	pushl %ebx # callee saved registers
	pushl %esi
	pushl %edi 

	# first check if current is 0
	movl current_board, %edi
	cmpl $0, %edi
	je end_of_tasklet # if current 0, than go done section

	# initialize variables
	xorl %ebx, %ebx # count live cell
	movl $1, %ecx # i
	movl $1, %edx # j

	tasklet_outloop:
        cmpl $19, %ecx 
        je taskout_done # until 19 keep rotate
		movl $1, %edx  # reset j

 	tasklet_inner:
        imull $SCR_WIDTH, %ecx, %esi   # index=i*80 
		addl %edx, %esi   # idx+j
		movl current_board, %eax
		addl %esi, %eax	# add idx with current pointer
			
		pushl %ecx
		pushl %edx # c call reg save	

		pushl %eax # push arg
		call neighbor_count
		addl $4, %esp  # pop up arg

		popl %edx			
		popl %ecx # put back c call reg

		movl current_board, %edi

		pushl %ecx
		pushl %edx # c call reg save
		pushl %eax   # result of count
		movzbl (%edi, %esi), %eax  # get data of index
		pushl %eax # put two arg for tick result
		call tick_result
		addl $8, %esp # pop two arg
		popl %edx
		popl %ecx #  save back c call regs

		movl next_board, %edi # get next board pointer
		addl %esi, %edi
		movb %al, (%edi)  # get data of next board of index
		cmpb $1, %al         
		jne deadoralive # if dead skip next 
		incl %ebx	# total_count++

	deadoralive:
        cmpl $78, %edx # if went to all 78 col
        je taskin_done
        incl %edx # inc row and go back
        jmp tasklet_inner

	taskin_done:
        inc %ecx # inc col
        jmp tasklet_outloop # go back to huge loop

	taskout_done:
		imull $10, %ebx, %eax  # eax = total_count * 10
		movl %eax, infection   # infection = total_count * 10

		# swap two pointers
		movl current_board, %ebx
		movl next_board, %eax
		movl %eax, current_board
		movl %ebx, next_board 

		movl population, %edi
		subl infection, %edi  # check population to not go below 0
		cmpl $0, %edi
		jl popul_zero # population < infection
		movl %edi, population
		jmp end_of_tasklet # if pop still positive or zero tasklet done

	popul_zero:
		movl $0, population # population is 0
						

	end_of_tasklet:
		call mp1_notify_user
		call draw_screen # notify first and draw screen

		popl %edi
		popl %esi
		popl %ebx # return saved registers
		leave # set back stack pointers
		ret

# int mp1_ioctl(unsigned long arg, unsigned int cmd)
# The dispatch function for the MP1 ioctls - should use the cmd argument
# and a jumptable to execute one of the specific ioctls implemented below.
# Inputs   : unsigned long arg - parameter to the mp1_ioctl_....
#          : unsigned int cmd  - specifies which mp1_ioctl_... function 
#          :                     to execute
# Outputs  : Returns an integer - depends on which ioctl() function is called
# Registers: Standard C calling convention

.globl mp1_ioctl
mp1_ioctl:
	movl 8(%esp), %edx # edx<-m[esp+8], put cmd on reg
	
	# cmd: 0~4 unsigned long
	cmpl $0, %edx
	jb jump_invalid # if cmd below 0, invalid

	cmpl $4, %edx
	ja jump_invalid # if cmd above 4, invalid

	jmp *jumptable(, %edx, 4) # jump to m[jtable+cmd*4byte]

	jump_invalid:
		movl $-1, %eax 
		ret # return value(eax) is -1 and return


# ----------------- Functions private to this file -------------------


draw_screen:	
	pushl %ebp
	movl %esp, %ebp # new stack base with save old ebp

	pushl %ebx # callee saved registers
	pushl %esi
	pushl %edi

	movl $1, %ecx # i 
	movl $1, %ebx # j

 	draw_loop:
    imull $SCR_WIDTH, %ecx, %esi   
	addl %ebx, %esi # idx: i+col+j
			
	movl %esi, %eax
	addl %eax, %eax # twice the eax
	movl current_board, %edi  # edi=current board
	cmpb $1, (%edi, %esi) # check data of board
	
	je live # if the cell is alive
	pushl %edx
	pushl %ecx 
	pushl %eax # save c caller registers
	movb $CELL_DEAD, %cl # put data to write down on cl
	call mp1_poke
	popl %eax
	popl %ecx 
	popl %edx # after poke pop caller register
	jmp liveanddead # go to live and dead

	live:	
		pushl %edx
		pushl %ecx 
		pushl %eax
		movb $CELL_LIVE, %cl # if life save call reg and draw by poke
		call mp1_poke
		popl %eax
		popl %ecx 
		popl %edx

	liveanddead:			
    	cmpl $78, %ebx
    	je draw_innerdone
    	incl %ebx
    	jmp draw_loop # check that it went all 78 width and increase if didnt

	draw_innerdone:
    	incl %ecx
    	cmpl $19, %ecx
    	je finish_draw
		movl $1, %ebx
		jmp draw_loop # check that it went all 19 loops

	finish_draw:
		popl %edi
		popl %esi
		popl %ebx # return saved registers
		leave # set back stack pointers
		ret

mp1_ioctl_startgame:
	pushl %ebp
	movl %esp, %ebp # new stack base with save old ebp

	pushl %ebx # callee saved registers
	pushl %esi
	pushl %edi 

	# seed first
	movl 8(%esp), %eax # get parameter of seed for seed_generator
	pushl %eax # get parameter of seed for seed_generator
	call seed_generator # no c caller saved since void func
	addl $4, %esp # erase arg
	
	# allocate two boards
	pushl $SCR_SIZE # push arg for allocation, size of board
	call mp1_malloc
	addl $4, %esp # pop argument after alloc

	cmpl $0, %eax
	je malloc_fail # if pointer is zero, fail
	movl %eax, current_board # fine than put on current board

	pushl $SCR_SIZE
	call mp1_malloc
	addl $4, %esp # push arg and pop arg to malloc

	cmpl $0, %eax
	je malloc_fail # if pointer is zero, fail
	movl %eax, next_board # fine than put on next board

	# edx: 0~board size to initialize board
	xorl %edx, %edx # set idx to 0

	# esi: ptr current | edi: ptr next
	movl current_board, %ecx
	movl next_board, %ebx

	init_loop:
		movl $0, (%ecx, %edx)  
		movl $0, (%ebx, %edx)  # use displacement to put 0 on idx of boards
		incl %edx  # increment index
		cmpl $SCR_SIZE, %edx
        je init_loopdone            
		jmp init_loop # continue jump until index reach 80
			
	init_loopdone:
		pushl current_board
		call init_virus
		addl $4, %esp # push board ptr to start virus and pop
		movl %eax, infection # store result of init to infec

		movl $80, aggression
		movl $8000000, population # initialize variables for agg and popul

		movl $0, %eax # set return value as 0 for success

		popl %edi
		popl %esi
		popl %ebx # return saved registers
		leave # set back stack pointers
		ret

	malloc_fail:
	pushl current_board
	call mp1_free
	addl $4, %esp # push board ptr to free and pop arg

	pushl next_board
	call mp1_free
	addl $4, %esp # do same as above with next board

	movl $0, current_board
	movl $0, next_board # set both boards to zero on popinters

	movl $-1, %eax # return -1 for fail

	popl %edi
	popl %esi
	popl %ebx # return saved registers
	leave # set back stack pointers
	ret

mp1_ioctl_endgame:
	pushl %ebp	
	movl %esp, %ebp # new stack base with save old ebp

	pushl %ebx # callee saved registers
	pushl %esi
	pushl %edi 
	pushl current_board
	call mp1_free
	addl $4, %esp # push current for arg and call free function and pop arg
	
	pushl next_board
	call mp1_free
	addl $4, %esp # push next for arg and call free function and pop arg
	movl $0, current_board
	movl $0, next_board # set both pointers to 0

	movl $0, %eax # return 0 for success

	popl %edi
	popl %esi
	popl %ebx # return saved registers
	leave # set back stack pointers
	ret	

mp1_ioctl_keystroke:
	pushl %ebp
	movl %esp, %ebp # new stack base with save old ebp

	pushl %ebx # callee saved registers
	pushl %esi
	pushl %edi 

	# poke eax edx ecx
	subl $KA_STACK, %esp  # create stack to put information
	movl %esp, %edi 
	movl 8(%ebp), %esi

	# check pointer to be not 0
    cmpl $0, %esi
    je pointer_zero

	pushl $KA_SIZE
	pushl %esi
	pushl %edi # push neccessary stack

	call mp1_copy_from_user
	addl $12, %esp # pop 3 args
	cmpl $0, %eax # return 0 than good
	jne pointer_zero

	# check the direction
	movw (%edi), %bx
	cmpw $0, %bx
	je L
	cmpw $2, %bx
	je R
	
	L:
	# get index to erase
	movl SELECTOR(%edi), %esi
	imull $10, %esi # (14+10S, 22)
	addl $14, %esi 
	addl $1760, %esi # (14+10S, 22) with i*col+j and *2 for video
	addl %esi, %esi # equal multiply twice

	# erase selector
	movl %esi, %eax     
	movl $select_blank, %ecx  # cx get 0x20
	movb (%ecx), %cl # gives space at first place to remove
	call mp1_poke
				
	addl $2, %esi
	movl %esi, %eax
	movl $select_blank, %ecx 
	movb (%ecx), %cl
	call mp1_poke

	addl $2, %esi
	movl %esi, %eax
	movl $select_blank, %ecx 
	movb (%ecx), %cl
	call mp1_poke # three times then fill "   "

	cmpw $0, SELECTOR(%edi) # after erase, to redraw
	jne regular_left_move # only 0 need special case

	movw $4, SELECTOR(%edi) # rewrite selector data by giving new data
	movl SELECTOR(%edi), %esi
	imull $10, %esi
	addl $14, %esi
	addl $1760, %esi # get index to put string
	addl %esi, %esi
	movl %esi, %eax # get input to put on the poke
	movl $select_image, %ecx # give =
	movb (%ecx), %cl
	call mp1_poke
	addl $2, %esi # move to next place
	movl %esi, %eax 
	movl $select_image, %ecx # give value of =
	movb 1(%ecx), %cl
	call mp1_poke
	addl $2, %esi # move to last place
	movl %esi, %eax 
	movl $select_image, %ecx  # give value of >
	movb 2(%ecx), %cl
	call mp1_poke # print out ==> and go to finish
	jmp keystroke_done
	

	regular_left_move:	
	addw $-1, SELECTOR(%edi) # 1, 2, 3, 4 just need subtraction from 1 on selector value to chage
	movl SELECTOR(%edi), %esi
	imull $10, %esi
	addl $14, %esi
	addl $1760, %esi # find the index of place to put string
	addl %esi, %esi

	movl %esi, %eax # give eax index
	movl $select_image, %ecx # give =
	movb (%ecx), %cl
	call mp1_poke

	addl $2, %esi
	movl %esi, %eax # get new index and pass
	movl $select_image, %ecx  # give =
	movb 1(%ecx), %cl
	call mp1_poke

	addl $2, %esi
	movl %esi, %eax # last place
	movl $select_image, %ecx # give >
	movb 2(%ecx), %cl
	call mp1_poke

	jmp keystroke_done

	R:
	# get index to erase
	movl SELECTOR(%edi), %esi
	imull $10, %esi # (14+10S, 22)
	addl $14, %esi 
	addl $1760, %esi # (14+10S, 22) with i*col+j and *2 for video
	addl %esi, %esi # equal multiply twice

	# erase selector
	movl %esi, %eax     
	movl $select_blank, %ecx  # cx get 0x20
	movb (%ecx), %cl # gives space at first place to remove
	call mp1_poke
				
	addl $2, %esi
	movl %esi, %eax
	movl $select_blank, %ecx 
	movb (%ecx), %cl
	call mp1_poke # same as top

	addl $2, %esi
	movl %esi, %eax
	movl $select_blank, %ecx 
	movb (%ecx), %cl
	call mp1_poke # three times to fill "   "
	
	cmpw $4, SELECTOR(%edi) # after erase, to redraw
	jne regular_right_move # only 4 need special case for right side

	movw $0, SELECTOR(%edi) # rewrite selector data by giving new data
	movl SELECTOR(%edi), %esi
	imull $10, %esi
	addl $14, %esi
	addl $1760, %esi # get index to put string
	addl %esi, %esi
	movl %esi, %eax # get input to put on the poke
	movl $select_image, %ecx # give =
	movb (%ecx), %cl
	call mp1_poke
	addl $2, %esi # move to next place
	movl %esi, %eax 
	movl $select_image, %ecx # give value of =
	movb 1(%ecx), %cl
	call mp1_poke
	addl $2, %esi # move to last place
	movl %esi, %eax 
	movl $select_image, %ecx  # give value of >
	movb 2(%ecx), %cl
	call mp1_poke # print out ==> and go to finish
	jmp keystroke_done

	regular_right_move:
	addw $1, SELECTOR(%edi) # 0, 1, 2, 3 just need addition from 1 on selector value to chage
	movl SELECTOR(%edi), %esi
	imull $10, %esi
	addl $14, %esi
	addl $1760, %esi # find the index of place to put string
	addl %esi, %esi

	movl %esi, %eax # give eax index
	movl $select_image, %ecx # give =
	movb (%ecx), %cl
	call mp1_poke

	addl $2, %esi
	movl %esi, %eax # get new index and pass
	movl $select_image, %ecx  # give =
	movb 1(%ecx), %cl
	call mp1_poke

	addl $2, %esi
	movl %esi, %eax # last place
	movl $select_image, %ecx # give >
	movb 2(%ecx), %cl
	call mp1_poke
		jmp keystroke_done

	pointer_zero:
		movl $-1, %eax # return zero if pointer wrong or n=0 form user

		popl %edi
		popl %esi
		popl %ebx # return saved registers
		leave # set back stack pointers
		ret	

	keystroke_done:
		movl 8(%ebp), %esi
		pushl $KA_SIZE
		pushl %edi
		pushl %esi # pass arg for copy to user to show back to kernel

        cmpl $0, %esi
        je pointer_zero # pointer should not be 0

		call mp1_copy_to_user
		addl $12, %esp # pop args
		cmpl $0, %eax # check return value
		jne	pointer_zero # if fail than go to pointer zero

		movl $0, %eax # return 0 for success
		popl %edi
		popl %esi
		popl %ebx # return saved registers
		leave # set back stack pointers
		ret	 

mp1_ioctl_getstatus:
 	pushl %ebp	
	movl %esp, %ebp # new stack base with save old ebp

	pushl %ebx # callee saved registers
	pushl %esi
	pushl %edi

	subl $GS_SIZE, %esp
	pushl infection
	pushl population # push number of inf and pop for status
	movl %esp, %edx
	movl 8(%ebp), %ecx

	pushl $GS_SIZE
	pushl %edx
	pushl %ecx # push three args

    cmpl $0, %ecx
    je z_ptr # check to pointer if zero
	
	call mp1_copy_to_user # if not zero, call copy to
	addl $12, %esp # pop 3 arg

	cmpl $0, %eax # compare result from to user with 0
 	jne z_ptr
	# zero means no left
	movl $0, %eax # return 0 for success
	
	last_status:
    popl %edi
	popl %esi
	popl %ebx # return saved registers
	leave # set back stack pointers
	ret	

    z_ptr:
    movl $-1, %eax
    jmp last_status
        


mp1_ioctl_vaccinate:
    pushl %ebp	
	movl %esp, %ebp # new stack base with save old ebp

	pushl %ebx # callee saved registers
	pushl %esi
	pushl %edi
        
    pushl %eax
	pushl %ebx
	pushl %ecx # 3 call saver push

	movw aggression, %bx
	movw 10(%esp), %eax # high 16 bit
	shrl $2, %eax
	addw %ax, %bx
	movw %bx, aggression
	# movw aggression, %bx
	# addw %ax, %bx
    # movw %bx, aggression # add high 16bit to aggression

    # movw 10(%ebp), %ax # low 16 bit
    pushw %ax # for argument

    movl $1, %ebx # i: initialize i loop
    # i:ebx j: esi
    vac_loop:
        cmpl $19, %ebx
        je done
        movl $1, %esi # j

    vacinner_loop:
		pushl %eax
		pushl %ebx
		pushl %ecx # save c caller registers
        call generate # eas has random gen
		popl %ecx
    	popl %ebx
    	popl %eax # pop caller registers
        movl $100, %ecx
        movl $0, %edx
        idivl %ecx # remainder in edx
        
        imull $SCR_WIDTH, %ebx, %edi  # edi 80*i  
        addl %esi, %edi # edi+j
        addl current_board, %edi # complete edi

        movw 6(%ebp), %ax
        movzwl %ax, %eax # z-extend low 16bit
        cmpl %ecx, %eax # remaineder < percentage
        ja die 
        movl $0, (%edi)
        jmp continue_vac

    die:
        movl $1, (%edi)
        jmp continue_vac # if died just skip

    continue_vac:
        cmpl $79, %esi
        je vac_innerdone
        incl %esi # check that it went all 79 times
		

    vac_innerdone:
        incl %ebx
        jmp vac_loop # if inner loop done than go out loop

    done:
    popl %ecx
    popl %ebx
    popl %eax # c caller save pop
    movl $0, %eax # always return 0
	popl %edi
	popl %esi
	popl %ebx # return saved registers
	leave # set back stack pointers
    ret

