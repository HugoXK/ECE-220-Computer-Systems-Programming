#include <stdlib.h>
#include <string.h>

#include "mp8.h"

/* Introduction Paragraph:
 * My task this week is to implement several varieties of flood fill operations using recursion. These 
 * operations can be applied to sample images. This Part is the recursive function used to determine 
 * whether the flood operation continues recursively or stops with stopping conditions.
 */

/*
 * basicRecurse -- flood fill recursively from a point until reaching
 *                 white or the image border
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array)
 *         x -- current x position of the flood
 *         y -- current y position of the flood
 *         marking -- array used to mark seen positions (indexed
 *                    in the same way as the RGB input arrays)
 * OUTPUTS: marking -- marked with flooded pixels
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void 
basicRecurse (int32_t width, int32_t height,
	      const uint8_t* inRed, const uint8_t* inGreen, 
	      const uint8_t* inBlue, 
	      int32_t x, int32_t y, 
	      uint8_t* marking)
{	
	if (marking[x+width*y]==1){                                             // if 1 meaning it has been marked, stop!
		return;
	}else{
		if ( (inRed[x+width*y]==255) & (inGreen[x+width*y]==255) & (inBlue[x+width*y]==255)){
			return;                                                 // if RGB=xFFFFFF meaning white color, stop!
		}
		marking[x+width*y]=1;                                           // otherwise, mark it and recurse
		if (y-1 >= 0){							// Check adjacent pixels in the following order: up (negative y) 
										// right (positive x), down (positive y), and left (negative x)
		basicRecurse (width,height,inRed,inGreen,inBlue,x,y-1,marking); // to make sure whether it beyond the valid region.
		 								// recurse the upper one
		}
		if (x+1 < width){
		basicRecurse (width,height,inRed,inGreen,inBlue,x+1,y,marking); // recurse the right one
		}
		if (y+1 < height){
		basicRecurse (width,height,inRed,inGreen,inBlue,x,y+1,marking); // recurse the bottom one
		}
		if (x-1 >= 0){
		basicRecurse (width,height,inRed,inGreen,inBlue,x-1,y,marking); // recurse the left one
		}
	}
	return;
}


/*
 * greyRecurse -- flood fill recursively from a point until reaching
 *                near-white pixels or the image border
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array)
 *         x -- current x position of the flood
 *         y -- current y position of the flood
 *         distSq -- maximum distance squared between white and boundary
 *                   pixel color
 *         marking -- array used to mark seen positions (indexed
 *                    in the same way as the RGB input arrays)
 * OUTPUTS: marking -- marked with flooded pixels
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void 
greyRecurse (int32_t width, int32_t height,
	     const uint8_t* inRed, const uint8_t* inGreen, 
	     const uint8_t* inBlue, 
	     int32_t x, int32_t y, uint32_t distSq, 
	     uint8_t* marking)
{
	if (marking[x+width*y]==1){                                             // if 1 meaning it has been marked, stop!
		return;
	}else{
		int32_t r1=255;							// initialize parameters for judgement
		int32_t g1=255;
		int32_t b1=255;
		int32_t r2=inRed[x+width*y];
		int32_t g2=inGreen[x+width*y];
		int32_t b2=inBlue[x+width*y];
		if (colorsWithinDistSq (r1,g1,b1,r2,g2,b2,distSq)==1){		// to judge whether near-white pixels
			return;
		}
		marking[x+width*y]=1; 						// otherwise, mark it and recurse
		if (y-1 >= 0){							// Check adjacent pixels in the following order: up (negative y) 
										// right (positive x), down (positive y), and left (negative x)
		greyRecurse (width,height,inRed,inGreen,inBlue,x,y-1,distSq,marking);
										// to make sure whether it beyond the valid region.
										// recurse the upper one
		}
		if (x+1 < width){
		greyRecurse (width,height,inRed,inGreen,inBlue,x+1,y,distSq,marking); // recurse the right one
		}
		if (y+1 < height){
		greyRecurse (width,height,inRed,inGreen,inBlue,x,y+1,distSq,marking); // recurse the bottom one
		}
		if (x-1 >= 0){
		greyRecurse (width,height,inRed,inGreen,inBlue,x-1,y,distSq,marking); // recurse the left one
		}
	}
	return;
}


/*
 * limitedRecurse -- flood fill recursively from a point until reaching
 *                   pixels too different (in RGB color) from the color at
 *                   the flood start point, too far away (> 35 pixels), or
 *                   beyond the image border
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array)
 *         origX -- starting x position of the flood
 *         origY -- starting y position of the flood
 *         x -- current x position of the flood
 *         y -- current y position of the flood
 *         distSq -- maximum distance squared between pixel at origin 
 *                   and boundary pixel color
 *         marking -- array used to mark seen positions (indexed
 *                    in the same way as the RGB input arrays)
 * OUTPUTS: marking -- marked with flooded pixels
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void 
limitedRecurse (int32_t width, int32_t height,
	        const uint8_t* inRed, const uint8_t* inGreen, 
	        const uint8_t* inBlue, 
	        int32_t origX, int32_t origY, int32_t x, int32_t y, 
		uint32_t distSq, uint8_t* marking)
{
	if (marking[x+width*y]==1){                                             	// if 1 meaning it has been marked, stop!
		return;
	}else{
		int32_t r1=inRed[origX+width*origY];					// initialize parameters for judgement
		int32_t g1=inGreen[origX+width*origY];
		int32_t b1=inBlue[origX+width*origY];
		int32_t r2=inRed[x+width*y];
		int32_t g2=inGreen[x+width*y];
		int32_t b2=inBlue[x+width*y];
		
		if ( ((origX-x)*(origX-x)+(origY-y)*(origY-y) > 35*35) ){		// to judge whether too far away (> 35 pixels)
			return;
		}
		if (colorsWithinDistSq (r1,g1,b1,r2,g2,b2,distSq)==0){			// to judge whether pixels too different (in RGB color) 
			return;								//from the color atthe flood start point
		}
		marking[x+width*y]=1;							// otherwise, mark it and recurse
		if (y-1 >= 0){								//Check adjacent pixels in the following order: up (negative y) 											// right (positive x), down (positive y), and left (negative x)
											// to make sure whether it beyond the valid region.
		limitedRecurse (width,height,inRed,inGreen,inBlue,origX,origY,x,y-1,distSq,marking); // recurse the upper one
		}
		if (x+1 < width){
		limitedRecurse (width,height,inRed,inGreen,inBlue,origX,origY,x+1,y,distSq,marking); // recurse the right one
		}
		if (y+1 < height){
		limitedRecurse (width,height,inRed,inGreen,inBlue,origX,origY,x,y+1,distSq,marking); // recurse the bottom one
		}
		if (x-1 >= 0){
		limitedRecurse (width,height,inRed,inGreen,inBlue,origX,origY,x-1,y,distSq,marking); // recurse the left one
		}
	}
	return;
}

