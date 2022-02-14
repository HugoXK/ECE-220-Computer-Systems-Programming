;Acknowledgemnt: This is the MP2 written by Xu Ke for ECE220 class.
;This function contains a main function to print a student's daily 
;schedule. And it includes three subroutines PRINT_SCHEDULE, PRINT_SLOT 
;and PRINT_CENTERED.And both later two functions are copied from my MP1
;All rights reserved.


;Explanation: This program totally has three parts, namely, Initilization, Translation and Printing. 
;I firstly initialize the schedule and then translate events with variable-length into a schedule with fixed-length fields. 
;Finally, I print it with day names on the top and hour names on the left.


;Code starts here.
;Main function.

	.ORIG x3000                 ;Starts the program at x3000.

;Initialization starts here. In this part, I initialize all the memory location from x3800 to the 75th after it to x0000.
;Register table for initialization part.
;R1|store the value x0000
;R2|store the address (from beginning address x3800 to the 75th after it) or address pointer.
;R3|store the time still need to be looped (begin with 75)

INITIALIZATION
	LD R1,OZERO	            ;Load the x0000 into R1
	LD R2,THREIGHT              ;Initialize the beginning address x3800
	LD R3,SEVFIVE               ;Initialize the total times the function should execute
CONTINI
	STR R1,R2,#0                ;Store the value in R1 into the memory whose address is in R2.
	ADD R2,R2,#1		    ;Increment R2, pointing to the next address
	ADD R3,R3,#-1               ;Decrement R3, showing the times still need be executed
	BRp CONTINI                 ;Continue the initialization

;Translation starts here. In this part, all the message will be dealt with for printing. 
;Register table for translation part.
;R0|store the value of 16, since doing AND operation, 16 cannot be represented by offsets.
;R1|store the address in memory
;R2|used as an address pointer	
;R3|used to store the value in the address of value in R2/ also used to judge whether function finishes.
;R4|store the bits while making judgement on which day.
;R5|store the slot
;R6|store the day-representing part in memory.
;R7|temporarily used as a counter while doing 5 times slot operating.
	
	LD R2,BEGINNING		     ;Store the beginning address x4000.

OUTERLOOP
	LDR R3,R2,#0                 ;Load the value in memory address value of R2 into R3
	ST  R3,DAY		     ;Store it into memory for further use (since register is limited)
	ADD R3,R3,#1	             ;R3 plus one to make judgement whether it should finish (since -1 means finishing).
	BRz PRINT_SCHEDULE           ;If zero, means value in R3 is -1, the translation finishes.
	ADD R2,R2,#1                 ;Increment R2, pointing to the next.
	ST  R2,ADDRESS               ;Store it into memory for further use (since register is limited)

INNERLOOP
	LDR R3,R2,#0                 ;Load the value in memory address value of R2 into R3.
	BRz FINDNULL                 ;If Null, finding process finishes.
	ADD R2,R2,#1                 ;Increment R2, pointing to the next.
	BRnzp INNERLOOP              ;GO back to innerloop unconditionally.

FINDNULL
	ADD R2,R2,#1                 ;Increment R2
	LDR R5,R2,#0                 ;Store slot in R5
	BRn ERRORONE		     ;If negative, warning error message
	ADD R0,R5,#-14               ;Add -14 for judgement
	BRp ERRORONE	 	     ;If positive(means slot is larger than 14), warning error message

	LD R6,DAY                    ;Reload the day-representing bits (#16 means Monday, etc.)
	LD R0,SIXTEEN		     ;Load 16 into R0,since while doing AND, offsets cannot represent 16.
	LD R1,THREIGHT               ;Load the beginning address x3800
	AND R7,R7,#0		     ;Clear R7 for further use.
	ADD R7,R7,#5                 ;Set R7 to five, used as a counter.
	LD R3,ADDRESS                ;Reload the beginning value should be stored.

TIMELOOP			     ;This loop is used to calculate 5 times slot.
	ADD R1,R5,R1                 ;Add 1 times slot to R1     
	ADD R7,R7,#-1                ;Decrement R7, which means the times still need to be executed.
	BRnp TIMELOOP                ;If zero, jume out of the loop.
	AND R4,R0,R6                 ;Since the offset should less than 16, we have to store 16 in memory and load it.
	BRnz NEXTONE                 ;If positive, means should have 1 in the 5th(from right side) bits
	LDR R7,R1,#0                 ;Read for avioding repeating.
	BRnp ERRORTWO                ;If zero, means this address have not been filled yet.
	STR R3,R1,#0		     ;Store the value in R1 into the memory of the address value in R3.

NEXTONE	ADD R1,R1,#1		     ;Increment R1, pointing to the next
	AND R4,R6,#8		     ;AND with x0008
	BRnz NEXTTWO                 ;If positive, means should have 1 in the 4th(from right side) bits
	LDR R7,R1,#0                 ;Read for avioding repeating.
	BRnp ERRORTWO                ;If zero, means this address have not been filled yet.
	STR R3,R1,#0		     ;Store the value in R1 into the memory of the address value in R3.

NEXTTWO	ADD R1,R1,#1		     ;Increment R1, pointing to the next
	AND R4,R6,#4                 ;AND with x0004
	BRnz NEXTHREE                ;If positive, means should have 1 in the 3th(from right side) bits
	LDR R7,R1,#0                 ;Read for avioding repeating.
	BRnp ERRORTWO                ;If zero, means this address have not been filled yet.
	STR R3,R1,#0		     ;Store the value in R1 into the memory of the address value in R3.

NEXTHREE	
	ADD R1,R1,#1		     ;Increment R1, pointing to the next
	AND R4,R6,#2                 ;AND with x0002
	BRnz NEXTFOUR                ;If positive, means should have 1 in the 2th(from right side) bits
	LDR R7,R1,#0                 ;Read for avioding repeating.
	BRnp ERRORTWO                ;If zero, means this address have not been filled yet.
	STR R3,R1,#0		     ;Store the value in R1 into the memory of the address value in R3.

NEXTFOUR	
	ADD R1,R1,#1		     ;Increment R1, pointing to the next
	AND R4,R6,#1                 ;AND with x0001
	BRnz NEXTFIVE                ;If positive, means should have 1 in the 1th(from right side) bits
	LDR R7,R1,#0                 ;Read for avioding repeating.
	BRnp ERRORTWO                ;If zero, means this address have not been filled yet.
	STR R3,R1,#0		     ;Store the value in R1 into the memory of the address value in R3.

NEXTFIVE	
	ADD R2,R2,#1                 ;Increment R2, pointing to the next
	BRnzp OUTERLOOP              ;Go back to outerloop unconditionally

	
ERRORONE 
	 LD  R2,ADDRESS              ;Load the beginning address of string
	 ADD R0,R2,#0                ;Copy the beginning address into R0 for printing.
	 PUTS                        ;Printing the string
	 LEA R0,ERRORMESSAGEONE      ;Load the beginning address into R0 for printing.
	 PUTS                        ;Printing the string
	 BRnzp FUNCTIONSTOP          ;Jump to halt unconditionally
ERRORTWO 
	 LD  R2,ADDRESS              ;Load the beginning address of string
	 ADD R0,R2,#0                ;Copy the beginning address into R0 for printing.
	 PUTS                        ;Printing the string
	 LEA R0,ERRORMESSAGETWO      ;Load the beginning address into R0 for printing.
	 PUTS                        ;Printing the string
	 BRnzp FUNCTIONSTOP          ;Jump to halt unconditionally

ERRORMESSAGEONE .STRINGZ " has an invalid slot number.\n"            ;Error message one
ERRORMESSAGETWO .STRINGZ " conflicts with an earlier event.\n"       ;Error message two
BEGINNING .FILL x4000                ;store the value of x4000 for convenience.
SIXTEEN   .FILL #16                  ;store the value of #16 for convenience, since offsets cannot represent 16.
DAY       .BLKW #1                   ;Save a address for storing day-representing bits
ADDRESS   .BLKW #1                   ;Save a address for storing address.
THREIGHT  .FILL x3800                ;store the value of x3800 (beginnig address) for convenience.
QUICK     .BLKW #1                   ;store the value of x3800+5Xslot for convenience.
SEVFIVE   .FILL #75                  ;store the times should be executed for convenience.
OZERO     .FILL x0000                ;store the value everytime should be filled while initializing.

;Printing starts here. In this part, I use the subroutine PRINT_SCHEDULE for printing.
;Register table for printing part (PRINT_SCHEDULE)
;R0|store the ASCII value for printing (OUT).
;R1|load the beginning address for every day.
;R2|store the value of -5 to make judgement about whether the function finishes printing 5 columns. 
;R3|store the value of -15 to make judgement about whether the function finishes printing 15 rows.
;R4|store the address, used as a pointer in memory.
;R5|used as a time pointer
;R6|no used, store and reload it just to avoid accidence.
;R7|no used, store and reload it just because some operations like OUT may change it.
;PRINT_SCHEDULE begins here. This is a callee save.

PRINT_SCHEDULE

	ST R0,STORZERO        ;Since it is a subroutine, store original value to avoid register value in main function.
        ST R1,STORONE         ;Since it is a subroutine, store original value to avoid register value in main function.
	ST R2,STORTWO         ;Since it is a subroutine, store original value to avoid register value in main function.
	ST R3,STORTHREE       ;Since it is a subroutine, store original value to avoid register value in main function.
	ST R4,STORFOUR        ;Since it is a subroutine, store original value to avoid register value in main function.
	ST R5,STORFIVE        ;Since it is a subroutine, store original value to avoid register value in main function.
	ST R6,STORSIX         ;Since it is a subroutine, store original value to avoid register value in main function.
	ST R7,STORSEVEN       ;Since it is a subroutine, store original value to avoid register value in main function.

	LEA R1,NINESPACE      ;Print the first nine space in first line
	JSR PRINT_CENTERED    ;Use PRINT_CENTERED subroutine to choose and center what should be printed.
	LD R0,LINE            ;Store the ASCII value of "|"
	OUT                   ;Print "|"
	LEA R1,MONDAY         ;Print Monday
	JSR PRINT_CENTERED    ;Use PRINT_CENTERED subroutine to choose and center what should be printed.
	LD R0,LINE            ;Store the ASCII value of "|"
	OUT                   ;Print "|"
	LEA R1,TUESDAY        ;Print Tuesday
	JSR PRINT_CENTERED    ;Use PRINT_CENTERED subroutine to choose and center what should be printed.
	LD R0,LINE            ;Store the ASCII value of "|"
	OUT                   ;Print "|"
	LEA R1,WEDNESDAY      ;Print Wednesday
	JSR PRINT_CENTERED    ;Use PRINT_CENTERED subroutine to choose and center what should be printed.
	LD R0,LINE            ;Store the ASCII value of "|"
	OUT                   ;Print "|"
	LEA R1,THURSDAY       ;Print Thursday
	JSR PRINT_CENTERED    ;Use PRINT_CENTERED subroutine to choose and center what should be printed.
	LD R0,LINE            ;Store the ASCII value of "|"
	OUT                   ;Print "|"
	LEA R1,FRIDAY         ;Print Friday
	JSR PRINT_CENTERED    ;Use PRINT_CENTERED subroutine to choose and center what should be printed.
	LD R0,ENTER           ;Store the ASCII value of enter.
	OUT                   ;Print the enter


	LD R4,BEGINPLACE      ;Store the address of beginning place into R4
	LD R3,MINUSSIXTEEN    ;Store the value of -16 into R3 for making judgement.
	AND R5,R5,#0          ;Clear the value in R5
	ADD R5,R5,#-1         ;Set R5 to -1.

ROWLOOP
	ADD R3,R3,#1          ;Increment R3
	BRz RELOAD            ;If zero, finishes.
	LD R2,MINUSFIVE       ;Load -5 into R2
	ADD R5,R5,#1          ;Increment R5
	ADD R1,R5,#0          ;Copy R5 into R1
	JSR PRINT_SLOT        ;Execute subroutine PRINT_SLOT for time

COLUMNLOOP
	ADD R2,R2,#0          ;Add noting just for judging
	BRz NEWROW            ;If zero, new row.
	LD R0,LINE            ;Load ASCII value of "|" into R0
	OUT                   ;Printing it.
	LDR R1,R4,#0          ;Load the bits in address value of R4 into R1
	BRnp CONTI            ;If negative/positive, jump towards CONTI and continue
	LEA R1,NINESPACE      ;Load ninespace into R1 for subroutine PRINT_CENTERED.
CONTI	JSR PRINT_CENTERED    ;Use subroutine PRINT_CENTERED.
	ADD R4,R4,#1          ;Increment pointer R4.
	ADD R2,R2,#1          ;Increment R2 to make judgement whether element in one line finishes.
	BRnzp COLUMNLOOP      ;Go back to column loop unconditionally.

NEWROW
	LD R0,ENTER           ;Load the ASCII value of enter into R0
	OUT                   ;Printing it.
	BRnzp ROWLOOP         ;Go back to rowloop unconditionally.
	
	
RELOAD
 	LD R0,STORZERO   ;restore register R0
	LD R1,STORONE    ;restore register R1
	LD R2,STORTWO    ;restore register R2
	LD R3,STORTHREE  ;restore register R3
	LD R4,STORFOUR   ;restore register R4
	LD R5,STORFIVE   ;restore register R5
	LD R6,STORSIX    ;restore register R6
	LD R7,STORSEVEN  ;restore register R7

FUNCTIONSTOP HALT          ;Halt the function.


STORZERO    .BLKW #1       ;this place is reserved for storing the value of R0.
STORONE     .BLKW #1       ;this place is reserved for storing the value of R1.
STORTWO     .BLKW #1       ;this place is reserved for storing the value of R2.
STORTHREE   .BLKW #1       ;this place is reserved for storing the value of R3.
STORFOUR    .BLKW #1       ;this place is reserved for storing the value of R4.
STORFIVE    .BLKW #1       ;this place is reserved for storing the value of R5.
STORSIX     .BLKW #1       ;this place is reserved for storing the value of R6.
STORSEVEN   .BLKW #1       ;this place is reserved for storing the value of R7.
BEGINPLACE   .FILL x3800     ;Store beginning address x3800 for convenience
MINUSSIXTEEN .FILL #-16      ;Store -16 for convenience while making judgement
MINUSFIVE    .FILL #-5       ;Store -5 for convenience while making judgement
ENTER        .FILL x000A     ;FIll with the ASCII value of Enter
LINE         .FILL x007C     ;Fill with the ASCII value of "|"

NINESPACE    .STRINGZ "         "   ;Store the string for printing.
MONDAY	     .STRINGZ "MONDAY"      ;Store the string for printing.
TUESDAY      .STRINGZ "TUESDAY"     ;Store the string for printing.
WEDNESDAY    .STRINGZ "WEDNESDAY"   ;Store the string for printing.
THURSDAY     .STRINGZ "THURSDAY"    ;Store the string for printing.
FRIDAY       .STRINGZ "FRIDAY"      ;Store the string for printing.


;From this part is the MP1 code written by me (Xu Ke) for ECE220 class.
;This function contains two subroutines to print part of a student's daily 
;schedule, especially PRINT_SLOT and PRINT_CENTERED function.

;The first subroutine.

;Register table for PRINT_SLOT
;R0|store the ASCII value for printing (OUT).
;R1|store the input value (from 0 to 14).
;R2|used to make judgement whether the first bit is 0 or 1 or 2, and partly used to compute the second bit
;R7|no used, store and reload it just because some operations like OUT may change it.

;PRINT_SLOT begins here. It is used to compute the time part in the schedule, when the input is n (between 0 and 14), the output should be n+6 (between 6 and 20) respectively. And 3 space in the left as well as 2 space in the right.


PRINT_SLOT

    ST R0,STORAGEZERO   ;store the value of R0 into memory.
    ST R1,STORAGEONE    ;store the value of R1 into memory.
    ST R2,STORAGETWO    ;store the value of R2 into memory.
    ST R7,STORAGESEVEN  ;store the value of R7 into memory.

    LD R0,ASCIISPACE    ;load the ASCII value of space into R0 for printing.
    OUT                 ;print the first space.
    OUT                 ;print the second space.
    OUT                 ;print the third space.

    ADD R2,R1,#-4       ;to judge whether the time is before 10 or not.
    BRn BEFORE          ;if negative, means before 10 (or the first character should be zero)
    ADD R2,R1,#-14      ;to judge whether the time is before 20 or not.
    BRn MIDDLE	        ;if negative, means between 10(included) and 20(not inluded).
    ADD R2,R1,#-14      ;compute the next character for the condition after 20(included).
    LD R0,ASCIITWO      ;load the ASCII value of two into R0 for printing.
    BRnzp PRINTIT       ;jump to print operation.
BEFORE
    ADD R2,R1,#6        ;compute the next character for the condition before 10(not included).
    LD R0,ASCIIZERO     ;load the ASCII value of zero into R0 for printing.
    BRnzp PRINTIT       ;jump to print operation.
MIDDLE
    ADD R2,R1,#-4       ;compute the next character for the condition between 10(included) and 20(not included).
    LD R0,ASCIIONE      ;load the ASCII value of one into R0 for printing.
    BRnzp PRINTIT       ;jump to print operation.
PRINTIT
    OUT                 ;print it.

    ADD R0,R2,#0        ;copy the value from R2 to R0 for print.
    LD R2,ASCIIZERO     ;load the difference between value and its ASCII.
    ADD R0,R2,R0        ;turn value into ASCII.
    OUT                 ;print the next character.

    LD R0,ASCIIZERO     ;load the ASCII value of zero into R0 for printing.
    OUT                 ;print the zero before the last one.
    OUT                 ;print the last zero.

    LD R0,ASCIISPACE    ;load the ASCII value of space into R0 for printing.
    OUT                 ;print the space before the last one.
    OUT                 ;print the last space.

    LD R0,STORAGEZERO   ;restore register R0
    LD R1,STORAGEONE    ;restore register R1
    LD R2,STORAGETWO    ;restore register R2
    LD R7,STORAGESEVEN  ;restore register R7
    RET

STORAGEZERO  .BLKW #1   ;this place is reserved for storing the value of R0.
STORAGEONE   .BLKW #1   ;this place is reserved for storing the value of R1.
STORAGETWO   .BLKW #1   ;this place is reserved for storing the value of R2.
STORAGESEVEN .BLKW #1   ;this place is reserved for storing the value of R7.
ASCIISPACE .FILL x0020  ;store the ASCII value of space.
ASCIIZERO  .FILL x0030  ;store the ASCII value of 0.
ASCIIONE   .FILL x0031  ;store the ASCII value of 1.
ASCIITWO   .FILL x0032  ;store the ASCII value of 2.

;The second subroutine

;Register table for PRINT_CENTERED
;R0|store the ASCII value for printing(OUT).
;R1|store the adress of the first character in the string.Then used as a pointer.
;R2|store the value -9 for length comparasion.Then store the value of the space in the left of the string.Then the number of the space to be printed.
;R3|store the length of the spesific string.
;R4|as a place to make judgement whether the left and right space have been incremented completely.store the times of the cycle had been processed.
;R5|used to store the minus value of the total space in the calculating process.
;R6|store the value of the space in the right of the string.Then the number of the space to be printed.
;R7|no used, store and reload it just because some operations like OUT may change it.

;PRINT_CENTERED starts here. This function is used to load the spesific arrangement part of the schedule and add space to make it centered accrodingly. If no space, print the first nine character and if odd number of space, the left has 1 more than the right, if even, both sides have the same number of space.

PRINT_CENTERED

    ST R0,STOREZERO   ;store the value of R0 into memory.
    ST R1,STOREONE    ;store the value of R1 into memory.
    ST R2,STORETWO    ;store the value of R2 into memory.
    ST R3,STORETHREE  ;store the value of R3 into memory.
    ST R4,STOREFOUR   ;store the value of R4 into memory.
    ST R5,STOREFIVE   ;store the value of R5 into memory.
    ST R6,STORESIX    ;store the value of R6 into memory.
    ST R7,STORESEVEN  ;store the value of R7 into memory


    LD R2,MINUSNINE   ;load the total space number -9 into register R2.
    AND R3,R3,#0      ;clear the original value in R3.

LOAD
    LDR R0,R1,#0      ;load the character in the location stored in R1 into R0.
    BRz JUMPOUT       ;jump out of the loop if string ends (NULL found).
    ADD R3,R3,#1      ;increment when the input is not null.
    ADD R1,R1,#1      ;increment R1 ( the pointer points at the next character).
    BRnzp LOAD        ;go back to judge the next character.

JUMPOUT
    ADD R0,R2,R3      ;add the number of string and -9.
    BRzp FULL         ;if zero or positive means the string length is more than 9.
 
    ADD R5,R3,R2      ;store the value of the number of string -9. 
    AND R2,R2,#0      ;clear R2.
    AND R6,R6,#0      ;clear R6.

COUNT
    ADD R2,R2,#1      ;increment R2 (the number of space before string).
    ADD R4,R5,R6      ;add the string length -9 with the number of space after string.
    ADD R4,R4,R2      ;continue add the space number before string.
    BRz PRINTBEGIN    ;judge whether the string length plus space number equals to 9.
    ADD R6,R6,#1      ;increment R6 (the number of space after string).
    ADD R4,R5,R2      ;add the string length -9 with the number of space before string.
    ADD R4,R4,R6      ;continue add the space number after string.
    BRz PRINTBEGIN    ;judge whether the string length plus space number equals to 9.
    BRnzp COUNT       ;go back to continue unconditionally.


PRINTBEGIN
    LD R0,SPACE       ;load the ASCII value of space into R0.
    OUT               ;print one space.
    ADD R2,R2,#-1     ;decrement the number of space before string to de printed.
    BRp PRINTBEGIN    ;if positive, go back and continue. 
     
    ADD R3,R3,#0      ;add nothing, just for judging whether R3 is zero or not.
    BRz PRINTREMAIN   ;if the length of string is 0, skip the middle part.

PRINTCHAR
    LD R1,STOREONE    ;reload R1 for the address it contains.
LOOP
    LDR R0,R1,#0      ;load the character in address stored in R1.
    OUT               ;print it.
    ADD R1,R1,#1      ;increment R1( address pointer).
    ADD R3,R3,#-1     ;decrement R3 which means the number of character to be printed.
    BRp LOOP          ;if positive, go back and continue.

PRINTREMAIN
    ADD R6,R6,#0      ;add nothing, just to judge whether R6 is zero or not.
    BRz RESET         ;if zero,finish and go to reset.
    LD R0,SPACE       ;load the ASCII of space into R0.
    OUT               ;print it.
    ADD R6,R6,#-1     ;decrement R6 (the number of space remains to be printed) 
    BRnzp PRINTREMAIN ;go back and continue unconditionally.

FULL 
    LD R1,STOREONE    ;reload R1 for the address it contains.
    AND R4,R4,#0      ;clear R4.
CONTINUE
    ADD R0,R1,R4      ;calculate the current address.
    LDR R0,R0,#0      ;load the ASCII stored in relative memory.
    OUT
    ADD R4,R4,#1      ;increment R4 which means the character has been printed.
    ADD R3,R4,R2      ;R4-9 to judge whether finished.
    BRn CONTINUE      ;if negative, go back and continue.

RESET
    LD R0,STOREZERO   ;restore register R0
    LD R1,STOREONE    ;restore register R1
    LD R2,STORETWO    ;restore register R2
    LD R3,STORETHREE  ;restore register R3
    LD R4,STOREFOUR   ;restore register R4
    LD R5,STOREFIVE   ;restore register R5
    LD R6,STORESIX    ;restore register R6
    LD R7,STORESEVEN  ;restore register R7

    RET



STOREZERO  .BLKW #1   ;this place is reserved for storing the value of R0.
STOREONE   .BLKW #1   ;this place is reserved for storing the value of R1.
STORETWO   .BLKW #1   ;this place is reserved for storing the value of R2.
STORETHREE .BLKW #1   ;this place is reserved for storing the value of R3.
STOREFOUR  .BLKW #1   ;this place is reserved for storing the value of R4.
STOREFIVE  .BLKW #1   ;this place is reserved for storing the value of R5.
STORESIX   .BLKW #1   ;this place is reserved for storing the value of R6.
STORESEVEN .BLKW #1   ;this place is reserved for storing the value of R7.
SPACE      .FILL x0020  ;store the ASCII value of space.
TOTALSPACE .FILL #9   ;store the number 9.
MINUSNINE  .FILL #-9  ;store the value -9.




.END
