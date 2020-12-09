/*									tab:8
 *
 * photo.c - photo display functions
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
 * Creation Date:   Fri Sep  9 21:44:10 2011
 * Filename:	    photo.c
 * History:
 *	SL	1	Fri Sep  9 21:44:10 2011
 *		First written (based on mazegame code).
 *	SL	2	Sun Sep 11 14:57:59 2011
 *		Completed initial implementation of functions.
 *	SL	3	Wed Sep 14 21:49:44 2011
 *		Cleaned up code for distribution.
 */


#include <string.h>

#include "assert.h"
#include "modex.h"
#include "photo.h"
#include "photo_headers.h"
#include "world.h"


/* types local to this file (declared in types.h) */

/* 
 * A room photo.  Note that you must write the code that selects the
 * optimized palette colors and fills in the pixel data using them as 
 * well as the code that sets up the VGA to make use of these colors.
 * Pixel data are stored as one-byte values starting from the upper
 * left and traversing the top row before returning to the left of
 * the second row, and so forth.  No padding should be used.
 */
struct photo_t {
    photo_header_t hdr;			/* defines height and width */
    uint8_t        palette[192][3];     /* optimized palette colors */
    uint8_t*       img;                 /* pixel data               */
};

/* 
 * An object image.  The code for managing these images has been given
 * to you.  The data are simply loaded from a file, where they have 
 * been stored as 2:2:2-bit RGB values (one byte each), including 
 * transparent pixels (value OBJ_CLR_TRANSP).  As with the room photos, 
 * pixel data are stored as one-byte values starting from the upper 
 * left and traversing the top row before returning to the left of the 
 * second row, and so forth.  No padding is used.
 */
struct image_t {
    photo_header_t hdr;			/* defines height and width */
    uint8_t*       img;                 /* pixel data               */
};

/*
* An object of node. The code is to create a node for octree algorithm
* which make number of nodes based on given parameter with size of total of
* seven infomration: red, green, blue, pixel number, palette index, level four index,
* and level two index. By looping around fill the information and sort by algorithm and fill
* palette at the end
*/
struct octree_node_t{
	uint32_t sum_red, sum_green, sum_blue, pixel_count;
	uint16_t palette_index, index_2nd, index_4th;
};

/* file-scope variables */

/* 
 * The room currently shown on the screen.  This value is not known to 
 * the mode X code, but is needed when filling buffers in callbacks from 
 * that code (fill_horiz_buffer/fill_vert_buffer).  The value is set 
 * by calling prep_room.
 */
static const room_t* cur_room = NULL; 


/* 
 * fill_horiz_buffer
 *   DESCRIPTION: Given the (x,y) map pixel coordinate of the leftmost 
 *                pixel of a line to be drawn on the screen, this routine 
 *                produces an image of the line.  Each pixel on the line
 *                is represented as a single byte in the image.
 *
 *                Note that this routine draws both the room photo and
 *                the objects in the room.
 *
 *   INPUTS: (x,y) -- leftmost pixel of line to be drawn 
 *   OUTPUTS: buf -- buffer holding image data for the line
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
void
fill_horiz_buffer (int x, int y, unsigned char buf[SCROLL_X_DIM])
{
    int            idx;   /* loop index over pixels in the line          */ 
    object_t*      obj;   /* loop index over objects in the current room */
    int            imgx;  /* loop index over pixels in object image      */ 
    int            yoff;  /* y offset into object image                  */ 
    uint8_t        pixel; /* pixel from object image                     */
    const photo_t* view;  /* room photo                                  */
    int32_t        obj_x; /* object x position                           */
    int32_t        obj_y; /* object y position                           */
    const image_t* img;   /* object image                                */

    /* Get pointer to current photo of current room. */
    view = room_photo (cur_room);

    /* Loop over pixels in line. */
    for (idx = 0; idx < SCROLL_X_DIM; idx++) {
        buf[idx] = (0 <= x + idx && view->hdr.width > x + idx ?
		    view->img[view->hdr.width * y + x + idx] : 0);
    }

    /* Loop over objects in the current room. */
    for (obj = room_contents_iterate (cur_room); NULL != obj;
    	 obj = obj_next (obj)) {
	obj_x = obj_get_x (obj);
	obj_y = obj_get_y (obj);
	img = obj_image (obj);

        /* Is object outside of the line we're drawing? */
	if (y < obj_y || y >= obj_y + img->hdr.height ||
	    x + SCROLL_X_DIM <= obj_x || x >= obj_x + img->hdr.width) {
	    continue;
	}

	/* The y offset of drawing is fixed. */
	yoff = (y - obj_y) * img->hdr.width;

	/* 
	 * The x offsets depend on whether the object starts to the left
	 * or to the right of the starting point for the line being drawn.
	 */
	if (x <= obj_x) {
	    idx = obj_x - x;
	    imgx = 0;
	} else {
	    idx = 0;
	    imgx = x - obj_x;
	}

	/* Copy the object's pixel data. */
	for (; SCROLL_X_DIM > idx && img->hdr.width > imgx; idx++, imgx++) {
	    pixel = img->img[yoff + imgx];

	    /* Don't copy transparent pixels. */
	    if (OBJ_CLR_TRANSP != pixel) {
		buf[idx] = pixel;
	    }
	}
    }
}


/* 
 * fill_vert_buffer
 *   DESCRIPTION: Given the (x,y) map pixel coordinate of the top pixel of 
 *                a vertical line to be drawn on the screen, this routine 
 *                produces an image of the line.  Each pixel on the line
 *                is represented as a single byte in the image.
 *
 *                Note that this routine draws both the room photo and
 *                the objects in the room.
 *
 *   INPUTS: (x,y) -- top pixel of line to be drawn 
 *   OUTPUTS: buf -- buffer holding image data for the line
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
void
fill_vert_buffer (int x, int y, unsigned char buf[SCROLL_Y_DIM])
{
    int            idx;   /* loop index over pixels in the line          */ 
    object_t*      obj;   /* loop index over objects in the current room */
    int            imgy;  /* loop index over pixels in object image      */ 
    int            xoff;  /* x offset into object image                  */ 
    uint8_t        pixel; /* pixel from object image                     */
    const photo_t* view;  /* room photo                                  */
    int32_t        obj_x; /* object x position                           */
    int32_t        obj_y; /* object y position                           */
    const image_t* img;   /* object image                                */

    /* Get pointer to current photo of current room. */
    view = room_photo (cur_room);

    /* Loop over pixels in line. */
    for (idx = 0; idx < SCROLL_Y_DIM; idx++) {
        buf[idx] = (0 <= y + idx && view->hdr.height > y + idx ?
		    view->img[view->hdr.width * (y + idx) + x] : 0);
    }

    /* Loop over objects in the current room. */
    for (obj = room_contents_iterate (cur_room); NULL != obj;
    	 obj = obj_next (obj)) {
	obj_x = obj_get_x (obj);
	obj_y = obj_get_y (obj);
	img = obj_image (obj);

        /* Is object outside of the line we're drawing? */
	if (x < obj_x || x >= obj_x + img->hdr.width ||
	    y + SCROLL_Y_DIM <= obj_y || y >= obj_y + img->hdr.height) {
	    continue;
	}

	/* The x offset of drawing is fixed. */
	xoff = x - obj_x;

	/* 
	 * The y offsets depend on whether the object starts below or 
	 * above the starting point for the line being drawn.
	 */
	if (y <= obj_y) {
	    idx = obj_y - y;
	    imgy = 0;
	} else {
	    idx = 0;
	    imgy = y - obj_y;
	}

	/* Copy the object's pixel data. */
	for (; SCROLL_Y_DIM > idx && img->hdr.height > imgy; idx++, imgy++) {
	    pixel = img->img[xoff + img->hdr.width * imgy];

	    /* Don't copy transparent pixels. */
	    if (OBJ_CLR_TRANSP != pixel) {
		buf[idx] = pixel;
	    }
	}
    }
}


/* 
 * image_height
 *   DESCRIPTION: Get height of object image in pixels.
 *   INPUTS: im -- object image pointer
 *   OUTPUTS: none
 *   RETURN VALUE: height of object image im in pixels
 *   SIDE EFFECTS: none
 */
uint32_t 
image_height (const image_t* im)
{
    return im->hdr.height;
}


/* 
 * image_width
 *   DESCRIPTION: Get width of object image in pixels.
 *   INPUTS: im -- object image pointer
 *   OUTPUTS: none
 *   RETURN VALUE: width of object image im in pixels
 *   SIDE EFFECTS: none
 */
uint32_t 
image_width (const image_t* im)
{
    return im->hdr.width;
}

/* 
 * photo_height
 *   DESCRIPTION: Get height of room photo in pixels.
 *   INPUTS: p -- room photo pointer
 *   OUTPUTS: none
 *   RETURN VALUE: height of room photo p in pixels
 *   SIDE EFFECTS: none
 */
uint32_t 
photo_height (const photo_t* p)
{
    return p->hdr.height;
}


/* 
 * photo_width
 *   DESCRIPTION: Get width of room photo in pixels.
 *   INPUTS: p -- room photo pointer
 *   OUTPUTS: none
 *   RETURN VALUE: width of room photo p in pixels
 *   SIDE EFFECTS: none
 */
uint32_t 
photo_width (const photo_t* p)
{
    return p->hdr.width;
}


/* 
 * prep_room
 *   DESCRIPTION: Prepare a new room for display.  Since photo struct has
 * 	 information of palette, pass room of photo struct of palette to color
 *   INPUTS: r -- pointer to the new room
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: changes recorded cur_room for this file
 */
void
prep_room (const room_t* r)
{
    /* Record the current room. */
    cur_room = r;
	photo_t* p=room_photo(r);
	set_palette(p->palette);
}


/* 
 * read_obj_image
 *   DESCRIPTION: Read size and pixel data in 2:2:2 RGB format from a
 *                photo file and create an image structure from it.
 *   INPUTS: fname -- file name for input
 *   OUTPUTS: none
 *   RETURN VALUE: pointer to newly allocated photo on success, or NULL
 *                 on failure
 *   SIDE EFFECTS: dynamically allocates memory for the image
 */
image_t*
read_obj_image (const char* fname)
{
    FILE*    in;		/* input file               */
    image_t* img = NULL;	/* image structure          */
    uint16_t x;			/* index over image columns */
    uint16_t y;			/* index over image rows    */
    uint8_t  pixel;		/* one pixel from the file  */

    /* 
     * Open the file, allocate the structure, read the header, do some
     * sanity checks on it, and allocate space to hold the image pixels.
     * If anything fails, clean up as necessary and return NULL.
     */
    if (NULL == (in = fopen (fname, "r+b")) ||
	NULL == (img = malloc (sizeof (*img))) ||
	NULL != (img->img = NULL) || /* false clause for initialization */
	1 != fread (&img->hdr, sizeof (img->hdr), 1, in) ||
	MAX_OBJECT_WIDTH < img->hdr.width ||
	MAX_OBJECT_HEIGHT < img->hdr.height ||
	NULL == (img->img = malloc 
		 (img->hdr.width * img->hdr.height * sizeof (img->img[0])))) {
	if (NULL != img) {
	    if (NULL != img->img) {
	        free (img->img);
	    }
	    free (img);
	}
	if (NULL != in) {
	    (void)fclose (in);
	}
	return NULL;
    }

    /* 
     * Loop over rows from bottom to top.  Note that the file is stored
     * in this order, whereas in memory we store the data in the reverse
     * order (top to bottom).
     */
    for (y = img->hdr.height; y-- > 0; ) {

	/* Loop over columns from left to right. */
	for (x = 0; img->hdr.width > x; x++) {

	    /* 
	     * Try to read one 8-bit pixel.  On failure, clean up and 
	     * return NULL.
	     */
	    if (1 != fread (&pixel, sizeof (pixel), 1, in)) {
		free (img->img);
		free (img);
	        (void)fclose (in);
		return NULL;
	    }

	    /* Store the pixel in the image data. */
	    img->img[img->hdr.width * y + x] = pixel;
	}
    }

    /* All done.  Return success. */
    (void)fclose (in);
    return img;
}


/* 
 * read_photo
 *   DESCRIPTION: Read size and pixel data in 5:6:5 RGB format from a photo
 * file and create photo structure from it, which has a palette. Map to 2:2:2
 * and 4:4:4 RGB and implement octree algorithm. Based on the algorithm, fill 
 * most used rgb color from level four and leve two with fixed number for each of
 * them to palette index and print image with palette.
 *  
 *   INPUTS: fname -- file name for input
 *   OUTPUTS: none
 *   RETURN VALUE: pointer to newly allocated photo on success, or NULL
 *                 on failure
 *   SIDE EFFECTS: dynamically allocates memory for the photo
 */
photo_t*
read_photo (const char* fname)
{
    FILE*    in;	/* input file               */
    photo_t* p = NULL;	/* photo structure          */
    uint16_t x;		/* index over image columns */
    uint16_t y;		/* index over image rows    */
    uint16_t pixel;	/* one pixel from the file  */

    /* 
     * Open the file, allocate the structure, read the header, do some
     * sanity checks on it, and allocate space to hold the photo pixels.
     * If anything fails, clean up as necessary and return NULL.
     */
    if (NULL == (in = fopen (fname, "r+b")) ||
	NULL == (p = malloc (sizeof (*p))) ||
	NULL != (p->img = NULL) || /* false clause for initialization */
	1 != fread (&p->hdr, sizeof (p->hdr), 1, in) ||
	MAX_PHOTO_WIDTH < p->hdr.width ||
	MAX_PHOTO_HEIGHT < p->hdr.height ||
	NULL == (p->img = malloc 
		 (p->hdr.width * p->hdr.height * sizeof (p->img[0])))) {
	if (NULL != p) {
	    if (NULL != p->img) {
	        free (p->img);
	    }
	    free (p);
	}
	if (NULL != in) {
	    (void)fclose (in);
	}
	return NULL;
    }

	/*
	*	initialize level two and four octree by fixed numbers
	* 	with data of 0 for everything
	*	two: 64 nodes 	four: 4096 nodes
	*	node parameter:
	*	uint32_t sum_red, sum_green, sum_blue, pixel_count
	*	uint16_t palette_index, index_2nd, index_4th
	*/
	struct octree_node_t level_two[LEVEL_2nd_NODE];
	struct octree_node_t level_four[LEVEL_4th_NODE]; 
	
	int i;
	for(i = reset_zero; i < LEVEL_4th_NODE ; i++){
		if(i < LEVEL_2nd_NODE){
			level_two[i].sum_red=reset_zero;
			level_two[i].sum_green=reset_zero;
			level_two[i].sum_blue=reset_zero;
			level_two[i].pixel_count=reset_zero;
			level_two[i].palette_index=reset_zero;
			level_two[i].index_2nd=reset_zero;
			level_two[i].index_4th=reset_zero;

			level_four[i].sum_red=reset_zero;
			level_four[i].sum_green=reset_zero;
			level_four[i].sum_blue=reset_zero;
			level_four[i].pixel_count=reset_zero;
			level_four[i].palette_index=reset_zero;
			level_four[i].index_2nd=reset_zero;
			level_four[i].index_4th=reset_zero;
		}
		else{ //initialize rest of level four parameter
			level_four[i].sum_red=reset_zero;
			level_four[i].sum_green=reset_zero;
			level_four[i].sum_blue=reset_zero;
			level_four[i].pixel_count=reset_zero;
			level_four[i].palette_index=reset_zero;
			level_four[i].index_2nd=reset_zero;
			level_four[i].index_4th=reset_zero;
		}
	}

	/*
	*	initialize index variable 
	*/
	int idx;

    /* 
     * Loop over rows from bottom to top.  Note that the file is stored
     * in this order, whereas in memory we store the data in the reverse
     * order (top to bottom).
     */
    for (y = p->hdr.height; y-- > 0; ) {

	/* Loop over columns from left to right. */
	for (x = reset_zero; p->hdr.width > x; x++) {

	    /* 
	     * Try to read one 16-bit pixel.  On failure, clean up and 
	     * return NULL.
	     */
	    if (1 != fread (&pixel, sizeof (pixel), 1, in)) {
		free (p->img);
		free (p);
	        (void)fclose (in);
		return NULL;

	    }
	    /* 
	     * 16-bit pixel is coded as 5:6:5 RGB (5 bits red, 6 bits green,
	     * and 6 bits blue).  We change to 2:2:2, which we've set for the
	     * game objects.  You need to use the other 192 palette colors
	     * to specialize the appearance of each photo.
	     *
	     * In this code, you need to calculate the p->palette values,
	     * which encode 6-bit RGB as arrays of three uint8_t's.  When
	     * the game puts up a photo, you should then change the palette 
	     * to match the colors needed for that photo.
	     */
	    // p->img[p->hdr.width * y + x] = (((pixel >> 14) << 4) |
		// 			    (((pixel >> 9) & 0x3) << 2) |
		// 			    ((pixel >> 3) & 0x3));
		
		/*
		*	put pixel into corresponding image_pixel (saving image pixel)
		*	Save each pixel of image into order
		*/
		
		
		/* 
		*	RRRRR GGGGGG BBBBB >> to get R or G or B -> RGB 5:6:5
		*	Put each pixel R G B itself to each R G B parameter and put index of each
		* 	level with count number of pixel
		*	Furthermore, save each levels index to use them later when filling up palette
		*	and image
		* 	Each leve index formula is different since two needs 2:2:2 and four needs
		*	4:4:4
		*/
		idx = LEVEL_TWO_IDX(pixel); // index for level 2
		level_two[idx].sum_blue += pixel & five_full_bit;
		level_two[idx].sum_green += (pixel >> shift_five) & six_full_bit;
		level_two[idx].sum_red += (pixel >> shift_eleven) & five_full_bit;
		level_two[idx].index_2nd = idx;
		level_two[idx].index_4th = LEVEL_FOUR_IDX(pixel);
		level_two[idx].pixel_count++;
		
		/* do same thing for level four */
		idx = LEVEL_FOUR_IDX(pixel); // index for level 4
		level_four[idx].sum_blue += pixel & five_full_bit;
		level_four[idx].sum_green += (pixel >> shift_five) & six_full_bit;
		level_four[idx].sum_red += (pixel >> shift_eleven) & five_full_bit;
		level_four[idx].index_2nd = LEVEL_TWO_IDX(pixel);
		level_four[idx].index_4th = idx;
		level_four[idx].pixel_count++;
	}
    }

	/*
	*	void qsort (void* base, size_t num, size_t size, int (*compar)(const void*,const void*));
	* 	sort array of level four based on pixel count
	* 	http://www.cplusplus.com/reference/cstdlib/qsort/
	*	get size of node for third parameter
	*/
	int node_size = sizeof(struct octree_node_t);
	qsort(level_four, LEVEL_4th_NODE, node_size, compareMyType);

	/* 
	*	Initialize variables to put average rgb
	*	and variable to change int to uint8_t
	*	to fill palette of R G B
	*/
	int average_red, average_blue, average_green;
	uint8_t final_rgb;

	/* For all 4th and 2nd level, get average of RGB and fill palette if pixel is counted
	 * Furthermore, get most 128 first from level four and fill palette from the start
	*/
	for(i = reset_zero; i < LEVEL_4th_NODE; i++){
		idx = level_four[i].index_4th; //get level 4 index
		if(i < FINAL_USE_NODE){ // get most used 128 RGB of level 4 on palette
			if(level_four[i].pixel_count != 0){ 
				average_red = level_four[i].sum_red / level_four[i].pixel_count;
				average_green = level_four[i].sum_green / level_four[i].pixel_count;
				average_blue = level_four[i].sum_blue / level_four[i].pixel_count;
			} // get average if level four has data of no zero pixel counter or make 0 for RGB
			else{
				average_red = reset_zero;
				average_green = reset_zero;
				average_blue = reset_zero;
			}
			// change value of int to uint8_t since palette is assigned as uint8_t
			final_rgb = ( (average_red & five_full_bit) + (average_red & five_full_bit) ); 
			p->palette[i][R] = final_rgb;
			final_rgb = (average_green & six_full_bit);
			p->palette[i][G] = final_rgb;
			final_rgb = ( (average_blue & five_full_bit) + (average_blue & five_full_bit) );
			p->palette[i][B] = final_rgb; 
			/* conver 5:6:5 to 6:6:6  green is already 6 so no need of shifting left one which equals to
			* multiply with two */

			/* Since palette of 64 is already set, add palette index from 64 + i */
			level_four[i].palette_index = LEVEL_2nd_NODE + i; // level_2nd_node = 64 = palette offset
		} 
		else{ /* after getting most 128 color, get next color of 64 from level two*/
			idx = level_four[i].index_2nd; // get level two index from level four
			if(idx < LEVEL_2nd_NODE){ // get only level two node number
				level_two[idx].sum_red += level_four[i].sum_red;
				level_two[idx].sum_green += level_four[i].sum_green;
				level_two[idx].sum_blue += level_four[i].sum_blue;
				level_two[idx].pixel_count += level_four[i].pixel_count;	
			}
			else{ // if index is greater than 64 than just put 0
				level_two[idx].sum_red += reset_zero;
				level_two[idx].sum_green += reset_zero;
				level_two[idx].sum_blue += reset_zero;
				level_two[idx].pixel_count += reset_zero;
			}
		} 
	}
	/* Get average color of level two node and put on next palette 
	*	add 128 on index since already palette 128 added from level four
	*/
	for(i = reset_zero; i < LEVEL_2nd_NODE; i++){
		if(level_two[i].pixel_count != 0){ // do same thing one level to get average RGB like level four, but only size of level 2
			average_red = level_two[i].sum_red / level_two[i].pixel_count;
			average_green = level_two[i].sum_green / level_two[i].pixel_count;
			average_blue = level_two[i].sum_blue / level_two[i].pixel_count;
		}
		else{ // if count is 0 than put average 0 for all RGB
			average_red=reset_zero;
			average_green=reset_zero;
			average_blue=reset_zero;
		}
		/*Do same for palette like Level 4 except add next to most used 128*/
		final_rgb = ( (average_red & five_full_bit) + (average_red & five_full_bit) );
		p->palette[i+FINAL_USE_NODE][R] = final_rgb;
		final_rgb = (average_green & six_full_bit);
		p->palette[i+FINAL_USE_NODE][G] = final_rgb;
		final_rgb = ( (average_blue & five_full_bit) + (average_blue & five_full_bit) );
		p->palette[i+FINAL_USE_NODE][B] = final_rgb;
		// conver 5:6:5 to 6:6:6  green is already 6

		/* Keep track of index and add 192 since 64 and 128 were added */
		level_two[i].palette_index = LEVEL_TWO_PALETTE_OFFSET + i; // level_2nd_node = 64 = # of palette
	}	

	/*  Change palette index after filled 128 most used on level four with level
	this is neccessary to make level four as the base to find the color using palette index */
	for(i = reset_zero; i < LEVEL_4th_NODE; i++){ 
		idx = level_four[i].index_2nd;
		if( (i >= FINAL_USE_NODE) && (idx < LEVEL_2nd_NODE) ){ //only get 64 of level 2
			level_four[i].palette_index = level_two[idx].palette_index;
		}
	}


	/* Create table for level four that has level four index based on i
	This acts like a map to find color on level four octree to fill color of iamge*/
	uint32_t level_four_table[LEVEL_4th_NODE];
	for(i = reset_zero; i < LEVEL_4th_NODE; i++){
		level_four_table[level_four[i].index_4th] = i; 
	}

	/* https://www.geeksforgeeks.org/fseek-in-c-with-example/
	*  move file pointer associated with a given file to a specific position
	*  FILE *pointer, long int offset, int position
	*  Use seek_set to start from the beginning
	*  Basically use to just go over the data
	*/
	fseek(in, sizeof(p->hdr), SEEK_SET);

	/* Like did above on getting pixel, do same to find pixel of image */
	for (y = p->hdr.height; y-- > reset_zero; ) {
		
		/* Loop over columns from left to right. */
		for (x = 0; p->hdr.width > x; x++) {
	
			/* 
	    	 * Try to read one 8-bit pixel.  On failure, clean up and 
	    	 * return NULL.
	    	 */
	    	if (1 != fread (&pixel, sizeof (pixel), 1, in)) {
			free (p->img);
			free (p);
	        	(void)fclose (in);
			return NULL;
		    }
			idx = LEVEL_FOUR_IDX(pixel); //using pixel get 4:4:4 index

	    	/* Store the pixel in the image data by using level_four map array */
	    	p->img[(y * p->hdr.width) + x] = level_four[ level_four_table[idx] ].palette_index;
		}
    }

	/* All done.  Return success. */
    (void)fclose (in);

    return p;
}

/* 
* 	LEVEL_TWO_IDX
* 	DESCRIPTION: 16-bit pixel is coded as 5:6:5 RGB (5 bits red, 6 bits green,
* 	and 6 bits blue). We change to 2:2:2, which we've set for the
* 	game objects by getting two bits of most significant bits of each RGB
*   INPUTS: pixel -- pixel of image
*   OUTPUTS: none
*   RETURN VALUE: Six bit of RGB by 2:2:2
*   SIDE EFFECTS: return RGB used for pixel on level two octree
*/
int 
LEVEL_TWO_IDX(uint16_t pixel){
	return ( ((pixel >> fourteen) << four) 
			| ( ((pixel >> nine) & two_full_bit) << ( SHIFT_ONE+ SHIFT_ONE ) ) 
			| ( (pixel >> (four-SHIFT_ONE) ) & two_full_bit) );
}

/* 
* 	LEVEL_FOUR_IDX
* 	DESCRIPTION: 16-bit pixel is coded as 5:6:5 RGB (5 bits red, 6 bits green,
* 	and 6 bits blue). We change to 4:4:4, which we've set for the
* 	game objects by getting two bits of most significant bits of each RGB
*   INPUTS: pixel -- pixel of image
*   OUTPUTS: none
*   RETURN VALUE: twelve bit of RGB by 4:4:4
*   SIDE EFFECTS: return RGB used for pixel on level four octree
*/
int
LEVEL_FOUR_IDX(uint16_t pixel){
	return ( ((pixel >> (shift_eleven+SHIFT_ONE)) << (four + four)) 
		| ( ((pixel >> seven) & four_full_bit) << four) 
		| ( (pixel >> SHIFT_ONE) & four_full_bit) );
}


/* 
* 	compareMyType
* 	DESCRIPTION: 16-bit pixel is coded as 5:6:5 RGB (5 bits red, 6 bits green,
* 	and 6 bits blue). We change to 2:2:2, which we've set for the
* 	game objects by getting two bits of most significant bits of each RGB
*	http://www.cplusplus.com/reference/cstdlib/qsort/
*   INPUTS: *a and *b -- pointer to function that compare two element, qsort repeatedly
*	call to compare two elements
*   OUTPUTS: none
*   RETURN VALUE: -1 if a < b 	0 if a == b		1 if a > b
*   SIDE EFFECTS: Based on the return value, element pointed to by a goes before,
*	equivalent or after to by b
*	subtract b to a because we want high to low, not low to high
*/
int 
compareMyType (const void* a, const void* b){
	return ((struct octree_node_t*) b)->pixel_count - ((struct octree_node_t*)a)->pixel_count;
}
