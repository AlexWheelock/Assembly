;************************************************************************************
;										    *
;   Filename:	    SUBROUTINES.asm						    *
;   Date:	    September 30, 2024						    *
;   File Version:   3								    *
;   Author:	    Alex Wheelock						    *
;   Company:	    Idaho State University					    *
;   Description:    Contains all subroutines needed for the traffic light in Lab 7. *
;										    *
;************************************************************************************

;************************************************************************************
;										    *
;   Revision History:								    *
;										    *
;   1:	Initialize file, and set up the file along with includes		    *
;										    *
;   2:	Program it for Lab 7's traffic light, adding the subroutine for when the    *
;	traffic light has an event.						    *
;										    *
;   3:	Added the code to support the buttons added to RB0 & RB1 that signal a car  *
;	approaching the intersection from one of the two directions.		    *
;										    *
;************************************************************************************		
	    
SUBROUTINES_CODE CODE
 
		BANKSEL	PORTC 				;NO BUTTONS WERE PRESSED DURING THIS LOOP
		MOVLW	H'20'				;
		MOVWF	PORTC				;SET PORTC OUTPUT TO 20h TO DISPLAY NOTHING ONTO THE DOT MATRIX DISPLAY
		GOTO	MAIN				;RESTART LOOP
    SET_F				
		BANKSEL	PORTC				;
		MOVLW	H'46'				;
		MOVWF	PORTC				;SET PORTC OUTPUT TO 46h TO DISPLAY "F" ONTO THE DOT MATRIX DISPLAY
		GOTO	MAIN				;RESTART LOOP
    SET_E
		BANKSEL	PORTC				;
		MOVLW	H'45'				;
		MOVWF	PORTC				;SET PORTC OUTPUT TO 45h TO DISPLAY "E" ONTO THE DOT MATRIX DISPLAY
		GOTO	MAIN				;RESTART LOOP
    SET_D
		BANKSEL	PORTC				;
		MOVLW	H'44'				;
		MOVWF	PORTC				;SET PORTC OUTPUT TO 44h TO DISPLAY "D" ONTO THE DOT MATRIX DISPLAY
		GOTO	MAIN				;RESTART LOOP
    SET_C
	    	BANKSEL	PORTC				;
		MOVLW	H'43'				;
		MOVWF	PORTC				;SET PORTC OUTPUT TO 43h TO DISPLAY "C" ONTO THE DOT MATRIX DISPLAY
		GOTO	MAIN				;RESTART LOOP
    SET_B
		BANKSEL	PORTC				;
		MOVLW	H'42'				;
		MOVWF	PORTC				;SET PORTC OUTPUT TO 42h TO DISPLAY "B" ONTO THE DOT MATRIX DISPLAY
		GOTO	MAIN				;RESTART LOOP
    SET_A
		BANKSEL	PORTC				;
		MOVLW	H'41'				;
		MOVWF	PORTC				;SET PORTC OUTPUT TO 41h TO DISPLAY "A" ONTO THE DOT MATRIX DISPLAY
		GOTO	MAIN				;RESTART LOOP
    SET_9
		BANKSEL	PORTC				;
		MOVLW	H'39'				;
		MOVWF	PORTC				;SET PORTC OUTPUT TO 39h TO DISPLAY "9" ONTO THE DOT MATRIX DISPLAY
		GOTO	MAIN				;RESTART LOOP
    SET_8
		BANKSEL	PORTC				;
		MOVLW	H'38'				;
		MOVWF	PORTC				;SET PORTC OUTPUT TO 38h TO DISPLAY "8" ONTO THE DOT MATRIX DISPLAY
		GOTO	MAIN				;RESTART LOOP
    SET_7
		BANKSEL	PORTC				;
		MOVLW	H'37'				;
		MOVWF	PORTC				;SET PORTC OUTPUT TO 37h TO DISPLAY "7" ONTO THE DOT MATRIX DISPLAY
		GOTO	MAIN				;RESTART LOOP
    SET_6
		BANKSEL	PORTC				;
		MOVLW	H'36'				;
		MOVWF	PORTC				;SET PORTC OUTPUT TO 36h TO DISPLAY "6" ONTO THE DOT MATRIX DISPLAY
		GOTO	MAIN				;RESTART LOOP
    SET_5
		BANKSEL	PORTC				;
		MOVLW	H'35'				;
		MOVWF	PORTC				;SET PORTC OUTPUT TO 35h TO DISPLAY "5" ONTO THE DOT MATRIX DISPLAY
		GOTO	MAIN				;RESTART LOOP
    SET_4
		BANKSEL	PORTC				;
		MOVLW	H'34'				;
		MOVWF	PORTC				;SET PORTC OUTPUT TO 34h TO DISPLAY "4" ONTO THE DOT MATRIX DISPLAY
		GOTO	MAIN				;RESTART LOOP
    SET_3
		BANKSEL	PORTC				;
		MOVLW	H'33'				;
		MOVWF	PORTC				;SET PORTC OUTPUT TO 33h TO DISPLAY "3" ONTO THE DOT MATRIX DISPLAY
		GOTO	MAIN				;RESTART LOOP	
    SET_2
		BANKSEL	PORTC				;
		MOVLW	H'32'				;
		MOVWF	PORTC				;SET PORTC OUTPUT TO 32h TO DISPLAY "2" ONTO THE DOT MATRIX DISPLAY
		GOTO	MAIN				;RESTART LOOP
    SET_1
		BANKSEL	PORTC				;
		MOVLW	H'31'				;
		MOVWF	PORTC				;SET PORTC OUTPUT TO 31h TO DISPLAY "1" ONTO THE DOT MATRIX DISPLAY
		GOTO	MAIN				;RESTART LOOP
    SET_0
		BANKSEL	PORTC				;
		MOVLW	H'30'				;
		MOVWF	PORTC				;SET PORTC OUTPUT TO 30h TO DISPLAY "0" ONTO THE DOT MATRIX DISPLAY
		GOTO	MAIN				;RESTART LOOP