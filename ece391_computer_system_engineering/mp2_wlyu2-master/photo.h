/*									tab:8
 *
 * photo.h - photo display header file
 *
 * "Copyright (c) 2011 by Steven S. Lumetta."
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 * 
 * IN NO EVENT SHALL THE AUTHOR OR THE UNIVERSITY OF ILLINOIS BE LIABLE TO 
 * ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL 
 * DAMAGES ARISING OUT  OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, 
 * EVEN IF THE AUTHOR AND/OR THE UNIVERSITY OF ILLINOIS HAS BEEN ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE AUTHOR AND THE UNIVERSITY OF ILLINOIS SPECIFICALLY DISCLAIM ANY 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE 
 * PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND NEITHER THE AUTHOR NOR
 * THE UNIVERSITY OF ILLINOIS HAS ANY OBLIGATION TO PROVIDE MAINTENANCE, 
 * SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Author:	    Steve Lumetta
 * Version:	    3
 * Creation Date:   Fri Sep  9 21:45:34 2011
 * Filename:	    photo.h
 * History:
 *	SL	1	Fri Sep  9 21:45:34 2011
 *		First written.
 *	SL	2	Sun Sep 11 09:59:23 2011
 *		Completed initial implementation.
 *	SL	3	Wed Sep 14 21:56:08 2011
 *		Cleaned up code for distribution.
 */
#ifndef PHOTO_H
#define PHOTO_H


#include <stdint.h>

#include "types.h"
#include "modex.h"
#include "photo_headers.h"
#include "world.h"


/* limits on allowed size of room photos and object images */
#define MAX_PHOTO_WIDTH   1024
#define MAX_PHOTO_HEIGHT  1024
#define MAX_OBJECT_WIDTH  160
#define MAX_OBJECT_HEIGHT 100

/*define numbers need for octree*/
#define LEVEL_2nd_NODE           64 // 2^6
#define LEVEL_4th_NODE         4096 // 2^12
#define FINAL_USE_NODE          128 // result to use

#define five_full_bit          0x1F
#define six_full_bit           0x3F
#define shift_five                5
#define shift_eleven             11
#define reset_zero                0
#define R                         0
#define G                         1
#define B                         2  
#define SHIFT_ONE                 1
#define LEVEL_TWO_PALETTE_OFFSET 192 // 64+128
/* variables for getting level two and four index*/
#define four_full_bit           0xF
#define two_full_bit            0x3
#define four                      4
#define seven                     7
#define fourteen                 14
#define nine                      9

/* Fill a buffer with the pixels for a horizontal line of current room. */
extern void fill_horiz_buffer (int x, int y, unsigned char buf[SCROLL_X_DIM]);

/* Fill a buffer with the pixels for a vertical line of current room. */
extern void fill_vert_buffer (int x, int y, unsigned char buf[SCROLL_Y_DIM]);

/* Get height of object image in pixels. */
extern uint32_t image_height (const image_t* im);

/* Get width of object image in pixels. */
extern uint32_t image_width (const image_t* im);

/* Get height of room photo in pixels. */
extern uint32_t photo_height (const photo_t* p);

/* Get width of room photo in pixels. */
extern uint32_t photo_width (const photo_t* p);

/* 
 * Prepare room for display (record pointer for use by callbacks, set up
 * VGA palette, etc.). 
 */
extern void prep_room (const room_t* r);

/* Read object image from a file into a dynamically allocated structure. */
extern image_t* read_obj_image (const char* fname);

/* Read room photo from a file into a dynamically allocated structure. */
extern photo_t* read_photo (const char* fname);

/* Change 5:6:5 RGB pixel to 2:2:2 RGB to act as index for level two octree*/
extern int LEVEL_TWO_IDX(uint16_t pixel);

/* Change 5:6:5 RGB pixel to 4:4:4 RGB to act as index for level four octree*/
extern int LEVEL_FOUR_IDX(uint16_t pixel);

/* Comparator using in q sort, subtract high to low or gets seg fault*/
extern int compareMyType (const void *a, const void *b);

/* 
 * N.B.  I'm aware that Valgrind and similar tools will report the fact that
 * I chose not to bother freeing image data before terminating the program.
 * It's probably a bad habit, but ... maybe in a future release (FIXME).
 * (The data are needed until the program terminates, and all data are freed
 * when a program terminates.)
 */

#endif /* PHOTO_H */
