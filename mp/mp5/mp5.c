/*									tab:8
 *
 * main.c - skeleton source file for ECE220 picture drawing program
 *
 * "Copyright (c) 2018 by Charles H. Zega, and Saransh Sinha."
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
 * Author:	    Charles Zega, Saransh Sinha
 * Version:	    1
 * Creation Date:   12 February 2018
 * Filename:	    mp5.h
 * History:
 *	CZ	1	12 February 2018
 *		First written.
 *      XK      2       1  November 2020 
 *              Modificated
 */
#include "mp5.h"

/*
	You must write all your code only in this file, for all the functions!
*/


/* In this task, I write a selection of functions that act as simple drawing tools to create a PNG image. 
 * More specifically, I will write functions that can draw lines, rectangles, triangles, parallelograms, 
 * and circles, as well as a color gradient in a rectangular shape. Then I write a function that calls the 
 * other functions to draw a picture of what I like.
 */



/* 
 *  near_horizontal
 *	 
 *	 
 *	
 *	
 * INPUTS: x_start,y_start -- the coordinates of the pixel at one end of the line
 * 	   x_end, y_end    -- the coordinates of the pixel at the other end
 * OUTPUTS: draws a pixel to all points in between the two given pixels including
 *          the end points
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
near_horizontal(int32_t x_start, int32_t y_start, int32_t x_end, int32_t y_end){
	/* Your code goes here! */
	int32_t sgn,x,x1,x2,y,y1,y2;
	int32_t return_value_horizontal=1;    //Initilization and decleration

	if (x_start > x_end) {         //Set x2 to larger,x1 to smaller
	x2 = x_start;
	x1 = x_end;
	y2 = y_start;
	y1 = y_end;
	} else {
	x2 = x_end;
	x1 = x_start;
	y2 = y_end;
	y1 = y_start;
	}


	if (y2-y1 > 0) {             //judge the sign of (y2-y1) here
	sgn = 1;
	} else if (y2-y1 == 0) {
	sgn = 0;
	} else if (y2-y1 < 0) {
	sgn = -1;
	}

	for ( x=x1;x<=x2;x++) {             //loop each point and draw them
		y=(2*(y2-y1)*(x-x1)+sgn*(x2-x1))/(2*(x2-x1))+y1;
		return_value_horizontal = return_value_horizontal & draw_dot(x,y);   // only when all 1 can return 1
	}

	return return_value_horizontal;
}


/* 
 *  near_vertical
 *	 
 *	 
 *	
 *	
 * INPUTS: x_start,y_start -- the coordinates of the pixel at one end of the line
 * 	   x_end, y_end    -- the coordinates of the pixel at the other end
 * OUTPUTS: draws a pixel to all points in between the two given pixels including
 *          the end points
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
near_vertical(int32_t x_start, int32_t y_start, int32_t x_end, int32_t y_end){
	/* Your code goes here! */
	int32_t sgn,x,x1,x2,y,y1,y2;       //Initilization and decleration
	int32_t return_value_vertical=1;

	if (y_start > y_end) {             //Set y2 to larger,y1 to smaller
	y2 = y_start;
	y1 = y_end;
	x2 = x_start;
	x1 = x_end;
	} else {
	y2 = y_end;
	y1 = y_start;
	x2 = x_end;
	x1 = x_start;
	}


	if (x2-x1 > 0) {                              //judge the sign of (x2-x1) here
	sgn = 1;
	} else if (x2-x1 == 0) {
	sgn = 0;
	} else if (x2-x1 < 0) {
	sgn = -1;
	}

	for ( y=y1;y<=y2;y++) {                      //loop each point and draw them
		x=(2*(x2-x1)*(y-y1)+sgn*(y2-y1))/(2*(y2-y1))+x1;
		return_value_vertical = return_value_vertical & draw_dot(x,y);   // only when all 1 can return 1
	}

	return return_value_vertical;
}

/* 
 *  draw_line
 *	 
 *	 
 *	
 *	
 * INPUTS: x_start,y_start -- the coordinates of the pixel at one end of the line
 * 	   x_end, y_end    -- the coordinates of the pixel at the other end
 * OUTPUTS: draws a pixel to all points in between the two given pixels including
 *          the end points
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
draw_line(int32_t x_start, int32_t y_start, int32_t x_end, int32_t y_end){
	/* Your code goes here! */
	int32_t return_value_line = 1;       //Initilization and decleration
	float slope;
	if (x_start == x_end) {            //If two ends have same x
		if (y_start==y_end){       // If the same point
			return_value_line = return_value_line & near_vertical(x_start, y_start, x_end, y_end);
		}else{
			return_value_line = return_value_line & near_vertical(x_start, y_start, x_end, y_end);
		}
	} else {
		slope = (y_end - y_start)/(x_end - x_start);  //calculate the slope here
		if ((slope > 1.0) || (slope < -1.0)){         //If |slope|>1 vertical
		return_value_line = return_value_line & near_vertical(x_start, y_start, x_end, y_end);
		} else {                                     //Otherwise, horizontal
		return_value_line = return_value_line & near_horizontal(x_start, y_start, x_end, y_end);
		}                                      // only when all 1 can return 1
	}
	return return_value_line;
}


/* 
 *  draw_rect
 *	 
 *	 
 *	
 *	
 * INPUTS: x,y -- the coordinates of the of the top-left pixel of the rectangle
 *         w,h -- the width and height, respectively, of the rectangle
 * OUTPUTS: draws a pixel to every point of the edges of the rectangle
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
draw_rect(int32_t x, int32_t y, int32_t w, int32_t h){
	/* Your code goes here! */
	int32_t return_value_rect=1;                           //Initilization and decleration
	return_value_rect=return_value_rect & draw_line(x,y,x+w,y);
	return_value_rect=return_value_rect & draw_line(x,y+h,x+w,y+h);       //use 4 lines to draw the rectangle
	return_value_rect=return_value_rect & draw_line(x+w,y,x+w,y+h);
	return_value_rect=return_value_rect & draw_line(x,y,x,y+h);      // only when all 1 can return 1
	return return_value_rect;
}


/* 
 *  draw_triangle
 *	 
 *	 
 *	
 *	
 * INPUTS: x_A,y_A -- the coordinates of one of the vertices of the triangle
 *         x_B,y_B -- the coordinates of another of the vertices of the triangle
 *         x_C,y_C -- the coordinates of the final of the vertices of the triangle
 * OUTPUTS: draws a pixel to every point of the edges of the triangle
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
draw_triangle(int32_t x_A, int32_t y_A, int32_t x_B, int32_t y_B, int32_t x_C, int32_t y_C){
	/* Your code goes here! */
	int32_t return_value_triangle=1;                             //Initilization and decleration
	return_value_triangle=return_value_triangle & draw_line(x_A,y_A,x_B,y_B);
	return_value_triangle=return_value_triangle & draw_line(x_B,y_B,x_C,y_C);   //use 3 lines to draw triangle
	return_value_triangle=return_value_triangle & draw_line(x_C,y_C,x_A,y_A);
	return return_value_triangle;                   // only when all 1 can return 1
}

/* 
 *  draw_parallelogram
 *	 
 *	 
 *	
 *	
 * INPUTS: x_A,y_A -- the coordinates of one of the vertices of the parallelogram
 *         x_B,y_B -- the coordinates of another of the vertices of the parallelogram
 *         x_C,y_C -- the coordinates of another of the vertices of the parallelogram
 * OUTPUTS: draws a pixel to every point of the edges of the parallelogram
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
draw_parallelogram(int32_t x_A, int32_t y_A, int32_t x_B, int32_t y_B, int32_t x_C, int32_t y_C){
	/* Your code goes here! */
	int32_t x_D,y_D;
	int32_t return_value_parallelogram=1;                              //Initilization and decleration
	x_D=x_A-(x_B-x_C);                                      //use the property of parallelogram
	y_D=y_A-(y_B-y_C);
	return_value_parallelogram=return_value_parallelogram&draw_line(x_A,y_A,x_B,y_B);
	return_value_parallelogram=return_value_parallelogram&draw_line(x_B,y_B,x_C,y_C);    //use 4 lines to draw parallelogram
	return_value_parallelogram=return_value_parallelogram&draw_line(x_C,y_C,x_D,y_D);
	return_value_parallelogram=return_value_parallelogram&draw_line(x_D,y_D,x_A,y_A);
	return return_value_parallelogram;                                   // only when all 1 can return 1
}


/* 
 *  draw_circle
 *	 
 *	 
 *	
 *	
 * INPUTS: x,y -- the center of the circle
 *         inner_r,outer_r -- the inner and outer radius of the circle
 * OUTPUTS: draws a pixel to every point whose distance from the center is
 * 	    greater than or equal to inner_r and less than or equal to outer_r
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
draw_circle(int32_t x, int32_t y, int32_t inner_r, int32_t outer_r){
	/* Your code goes here!*/
	int32_t i,j;
	int32_t return_value_circle=1;                               //Initilization and decleration
	for (i=x-outer_r;i<=x+outer_r;i++){
		for (j=y-outer_r;j<=y+outer_r;j++){                 //use distance from center to make judgement
			if (((i-x)*(i-x)+(j-y)*(j-y)>=inner_r*inner_r) && ((i-x)*(i-x)+(j-y)*(j-y)<=outer_r*outer_r)){
				return_value_circle=return_value_circle & draw_dot(i,j);   //In the outer, out of the inner
			}
		}
	}
	return return_value_circle;                 // only when all 1 can return 1
}


/* 
 *  rect_gradient
 *	 
 *	 
 *	
 *	
 * INPUTS: x,y -- the coordinates of the of the top-left pixel of the rectangle
 *         w,h -- the width and height, respectively, of the rectangle
 *         start_color -- the color of the far left side of the rectangle
 *         end_color -- the color of the far right side of the rectangle
 * OUTPUTS: fills every pixel within the bounds of the rectangle with a color
 *	    based on its position within the rectangle and the difference in
 *          color between start_color and end_color
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
rect_gradient(int32_t x, int32_t y, int32_t w, int32_t h, int32_t start_color, int32_t end_color){
	/* Your code goes here! */
	int32_t return_value_gradient=1;
	int32_t i,j,R,G,B;                                        //Initilization and decleration
	int32_t sgn_R,sgn_G,sgn_B;
	int32_t total_level=0;
	int32_t R_and_bit=0x00FF0000;                              //Bits used for mask operation
	int32_t G_and_bit=0x0000FF00;
	int32_t B_and_bit=0x000000FF;
	int32_t R_start=((start_color & R_and_bit)>>16);
	int32_t G_start=((start_color & G_and_bit)>>8);       //shift them to git exact bits
	int32_t B_start=start_color & B_and_bit;
	int32_t R_end=((end_color & R_and_bit)>>16);
	int32_t G_end=((end_color & G_and_bit)>>8);
	int32_t B_end=end_color & B_and_bit;
	
	if (R_start < R_end) {                            //judge the sign of R
		sgn_R=1;
	}else if (R_start == R_end){
		sgn_R=0;
	}else if (R_start > R_end){
		sgn_R=-1;
	}

	if (G_start < G_end) {                               //judge the sign of G
		sgn_G=1;
	}else if (G_start == G_end){
		sgn_G=0;
	}else if (G_start > G_end){
		sgn_G=-1;
	}

	if (B_start < B_end) {                                              //judge the sign of B
		sgn_B=1;
	}else if (B_start == B_end){
		sgn_B=0;
	}else if (B_start > B_end){
		sgn_B=-1;
	}

	for (i=x;i<=x+w;i++){
		R=((2*(i-x)*(R_end-R_start)+sgn_R*w)/(2*w))+R_start;
		G=((2*(i-x)*(G_end-G_start)+sgn_G*w)/(2*w))+G_start;   //calculate them separately
		B=((2*(i-x)*(B_end-B_start)+sgn_B*w)/(2*w))+B_start;
		total_level=(R<<16)+(G<<8)+B;                      //shift and add to get RGB presentation
		set_color(total_level);
		for (j=y;j<=y+h;j++){	
			return_value_gradient=return_value_gradient & draw_dot(i,j);   //Use loop to draw dots
		}
	}
	
                                                  // only when all 1 can return 1
	return return_value_gradient;
}


/* 
 *  draw_picture
 *	 
 *	 
 *	
 *	
 * INPUTS: none
 * OUTPUTS: alters the image by calling any of the other functions in the file
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */


int32_t
draw_picture(){
	/* Your code goes here! */
	int32_t return_value_picture=1;                  //Initilization and decleration
	set_color(0x00000000);            //use line and rectangular to draw "H"
	return_value_picture=return_value_picture&draw_line(50,50,50,270);
	set_color(0x000000FF);
	return_value_picture=return_value_picture&draw_line(51,50,51,270);
	set_color(0x0000FF00);
	return_value_picture=return_value_picture&draw_line(52,50,52,270);
	return_value_picture=return_value_picture&draw_line(53,50,53,270);
	set_color(0x00FF0000);
	return_value_picture=return_value_picture&draw_line(54,50,54,270);
	set_color(0x000F0F00);
	return_value_picture=return_value_picture&draw_line(55,50,55,270);

	return_value_picture=return_value_picture&rect_gradient( 55,158,20,5,0x000F000F,0x00000F00);
	return_value_picture=return_value_picture&rect_gradient( 75,50,5,220,0x0000F000,0x000000F0);
	            
	return_value_picture=return_value_picture&rect_gradient( 85,50,5,220,0x00FF0000,0x000000FF);     //use rectangular to draw "E"
	return_value_picture=return_value_picture&rect_gradient( 85,50,15,5,0x00FF0000,0x000000FF);
	return_value_picture=return_value_picture&rect_gradient( 85,158,15,5,0x00FF0000,0x000000FF); 
	return_value_picture=return_value_picture&rect_gradient( 85,265,15,5,0x00FF0000,0x000000FF);  

	return_value_picture=return_value_picture&rect_gradient( 110,50,5,220,0x0000FF00,0x000000FF);    //use rectangular to draw "L"
	return_value_picture=return_value_picture&rect_gradient( 110,265,15,5,0x0000FF00,0x000000FF);

	return_value_picture=return_value_picture&rect_gradient( 130,50,5,220,0x0000FF00,0x00FF0000);    //use rectangular to draw "L"
	return_value_picture=return_value_picture&rect_gradient( 130,265,15,5,0x0000FF00,0x00FF0000);

	return_value_picture=return_value_picture&rect_gradient(  150,50,20,220,0x00000F00,0x00F0000F); //use two rectangular to draw "O"
	return_value_picture=return_value_picture&rect_gradient(  155,55,10,210,0x00FFFFFF,0x00FFFFFF);    //a white rectangular to mask

	set_color(0x00FF0000);        //a red sun
	return_value_picture=return_value_picture&draw_circle(200,50,10,20);

	
	set_color(0x0000FF00);
	return_value_picture=return_value_picture&draw_parallelogram(230, 50, 240, 50, 245, 270);       //use parallelogram to draw "W"
	set_color(0x000000FF);
	return_value_picture=return_value_picture&draw_parallelogram(245, 270, 255, 270, 260, 50);
	set_color(0x00FF0000);
	return_value_picture=return_value_picture&draw_parallelogram(260, 50, 270, 50, 275, 270);
	set_color(0x000F0F00);
	return_value_picture=return_value_picture&draw_parallelogram(275, 270, 285, 270, 290, 50);
	
	set_color(0x000F0F0F);
	return_value_picture=return_value_picture&draw_triangle(300, 270,360, 270, 330, 50);       //use parallelogram to draw "O"
	set_color(0x000F0FF0);
	return_value_picture=return_value_picture&draw_triangle(310, 260,350, 260, 330, 60);

	set_color(0x0000FF00);
	return_value_picture=return_value_picture&draw_parallelogram(380, 50, 390, 50, 395, 270);       //use parallelogram to draw "W"
	set_color(0x000000FF);
	return_value_picture=return_value_picture&draw_parallelogram(395, 270, 405, 270, 410, 50);
	set_color(0x00FF0000);
	return_value_picture=return_value_picture&draw_parallelogram(410, 50, 420, 50, 425, 270);
	set_color(0x000F0F00);
	return_value_picture=return_value_picture&draw_parallelogram(425, 270, 435, 270, 440, 50);
	return return_value_picture;    // only when all 1 can return 1
}
