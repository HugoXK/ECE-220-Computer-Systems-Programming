;This is the MP1 written by Xu Ke for ECE220 class.
;This function contains two subroutines to print
;part of a student's daily schedule, especially 
;PRINT_SLOT and PRINT_CENTERED function.
;All rights reserved.

;Code starts here.

.ORIG x3000

; Add this test code to the start of your file (just after .ORIG).
; I'd put it in another file, but we can't use the PRINT_SLOT and 
; PRINT_CENTERED labels outside of the mp1.asm file (at least, not 
; easily).

; Read the comments in this file to understand what it's doing and
; for ways that you can use this test code.  You can also just run
; it and diff the output with the output produced by our 'gold'
; (bug-free!) version.
;
; After assembling mp1 with lc3as, execute the test script by typing
;    lc3sim -s script1 > your_output
; (look at the script--it just loads mp1 with a file command, then
; continues execution; when the LC-3 halts, the script is finished,
; so the simulator halts).
;
; You can then type
;    diff your_output out1
; to compare your code's output with ours.
;

	; feeling lazy, so I'm going to set all of the bits to the same value
	LD	R0,BITS
	ADD	R2,R0,#0
	ADD	R3,R0,#0
	ADD	R4,R0,#0
	ADD	R5,R0,#0
	ADD	R6,R0,#0

	; let's try PRINT_SLOT ... "   1100  "
	AND	R1,R1,#0
	ADD	R1,R1,#5

	; set a breakpoint here in the debugger, then use 'next' to
	; execute your subroutine and see what happens to the registers;
	; they're not supposed to change (except for R7)...
	JSR	PRINT_SLOT
    LEA R1, STRING
    JSR PRINT_CENTERED
	; we're short on human time to test your code, so we'll do 
	; something like the following instead (feel free to replicate)...
	LD	R7,BITS
	NOT	R7,R7
	ADD	R7,R7,#1
	ADD	R0,R0,R7
	BRz	R0_OK
	LEA	R0,R0_BAD
	PUTS
R0_OK	

	; this trap changes register values, so it's not sufficient
	; to check that all of the registers are unchanged; HALT may
	; also lead to confusion because the register values differ
	; for other reasons (R7 differences, for example).
	HALT

BITS	.FILL	xABCD	; something unusual
R0_BAD	.STRINGZ "PRINT_SLOT changes R0!\n"
STRING   .STRINGZ "teststr"

; your code should go here ... don't forget .ORIG and .END

;The first subroutine.

;Register table for PRINT_SLOT
;R0|store the ASCII value for printing (OUT).
;R1|store the input value (from 0 to 14).
;R2|used to make judgement whether the first bit is 0 or 1 or 2, and partly used to compute the second bit
;R7|no used, store and reload it just because some operations like OUT may change it.

;PRINT_SLOT begins here. This is a callee save. It is used to compute the time part in the schedule, when the input is n (between 0 and 14), the output should be n+6 (between 6 and 20) respectively. And 3 space in the left as well as 2 space in the right.


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

;PRINT_CENTERED starts here. This is a callee save. This function is used to load the spesific arrangement part of the schedule and add space to make it centered accrodingly. If no space, print the first nine character and if odd number of space, the left has 1 more than the right, if even, both sides have the same number of space.

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
