#include <stdio.h>
#include <stdlib.h>

//    mp4.c work written for the Machine Problem Four of the ECE220 Fall directed by Professor Lumetta
// Author:     Xu Ke (3190110360)
// Version:    1
// Creation Date: 23 October 2020
// Filename:   mp4.c
// History:
//         XK   1  23 October 2020
//              First written
// Description: In this machine problem, I solve a logic problem and to print the solution. The logic problem forms a small
// part of solving a type of puzzle often called a Nonogram, in which a player tries to fill in a pixelated, black-
// and-white image based on lists of the lengths of the contiguous black regions in each row and column. In
// particular, given the number of pixels in a row and the number of pixels in up to five regions in that row,
// I identify those pixels known to be part of one of the regions.

int32_t print_row (int32_t width, int32_t r1, int32_t r2, int32_t r3, int32_t r4, int32_t r5)
{       int32_t leastlength = r1+r2+r3+r4+r5+(r2!=0)+(r3!=0)+(r4!=0)+(r5!=0);     //Calculate the least-length according to the inputs.
	if (leastlength > width){
            return 0;      // If the least length is larger than the width input, fail and return 0 to denote.
        } else {
	    int32_t array[55];     //Set an array to store and transform the 1/0 bits
	    for ( int32_t t=0; t<=50; t++){
		array[t]=0;       //Initialize all valid-region elements into 0
	    }          
            for ( int32_t x = 1; x <= r1; x++){
		array[x]=1;       //Since r1 is nonzero, no need to judge, set the number of r1 bits into 1 directly
	    }
            if ( r2!=0 ) {       //If r2=0, need to increment the pointer
   	    array[r1+1]=0;       //A blank is needed between black regions
	    }
            for ( int32_t y = r1+(r2!=0)+1; y <= r1+(r2!=0)+r2; y++){
		array[y]=1;      //Deal with region r2, choose the place of black block
	    }
	    if ( r3!=0 ) {       //If r3=0, need to increment the pointer
   	    array[r1+r2+2]=0;    //A blank is needed between black regions
	    }
            for ( int32_t z = r1+(r2!=0)+r2+(r3!=0)+1; z <= r1+(r2!=0)+r2+(r3!=0)+r3; z++){
		array[z]=1;      //Deal with region r3, choose the place of black block
	    }
	    if ( r4!=0 ) {       //If r4=0, need to increment the pointer
   	    array[r1+r2+r3+3]=0; //A blank is needed between black regions
	    }
            for ( int32_t a = r1+(r2!=0)+r2+(r3!=0)+r3+(r4!=0)+1; a <= r1+(r2!=0)+r2+(r3!=0)+r3+(r4!=0)+r4; a++){
		array[a]=1;      //Deal with region r4, choose the place of black block
	    }
            if ( r5!=0 ) {       //If r5=0, need to increment the pointer
   	    array[r1+r2+r3+r4+4]=0;     //A blank is needed between black regions
	    }
            for ( int32_t b = r1+(r2!=0)+r2+(r3!=0)+r3+(r4!=0)+r4+(r5!=0)+1; b <= r1+(r2!=0)+r2+(r3!=0)+r3+(r4!=0)+r4+(r5!=0)+r5; b++){
		array[b]=1;                 //Deal with region r5, choose the place of black block
	    }
            int32_t andtimes = width - leastlength;    //The time transformation happens equals to their difference
	    for (int32_t i = andtimes; i > 0; i--) {
		for (int32_t j=50; j >= 1; j--) {      //And each bit with its closet before bit, this is the rule behind transformation
                        array[j]=array[j] && array[j-1];
	                }
				
   		 }
	    for ( int32_t k=1; k<=width; k++){  //Search all the bits within the valid width.
		if (array[k]==1){       //For the "1" bits, print "*" as requested (representing black block)
	             printf("*");
		} else {                //For the "-" bits, print "-" as asked to do so (representing write block)
		     printf("-");
		}
	    }
	    printf("\n");      //Newline after each row
	    return 1;          //Return 1 representing successfully finding the solution.
	    }
}
      
	
