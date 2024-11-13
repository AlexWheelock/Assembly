;************************************************************************************
;										    *
;   Filename:	    SUBROUTINES.asm						    *
;   Date:	    October 24, 2024						    *
;   File Version:   2								    *
;   Author:	    Alex Wheelock						    *
;   Company:	    Idaho State University					    *
;   Description:    Contains all subroutines needed for Lab 10. Being able to use a *
;		    Keypad to write ASCII character codes into the PIC EEPROM, then *
;		    displaying it onto a dot-matrix.				    *
;										    *
;************************************************************************************

;************************************************************************************
;										    *
;   Revision History:								    *
;										    *
;   1:	Initialize file, and set up the file along with includes		    *
;										    *
;   2:	Added Subroutines to get EEPROM write/read and displaying it on a dot-matrix*
;	working.								    *
;										    *
;************************************************************************************		
	    
SUBROUTINES_CODE CODE
 
    SET_38KHZ
		BANKSEL	    T2CON
		BCF	    T2CON,2
		BANKSEL	    TMR2
		CLRF	    TMR2
		BANKSEL	    CCPR1L
		MOVLW	    0x0D
		MOVWF	    CCPR1L		
		BANKSEL	    PR2				
		MOVLW	    0x19			;SET PR2 TO 25 FOR 38kHz PERIOD
		MOVWF	    PR2	
		BANKSEL	    T2CON
		BSF	    T2CON,2
		GOTO	    TEST_MODE
		
    SET_56KHZ
    		BANKSEL	    T2CON
		BCF	    T2CON,2
		BANKSEL	    TMR2
		CLRF	    TMR2
		BANKSEL	    CCPR1L
		MOVLW	    0x09
		MOVWF	    CCPR1L		
		BANKSEL	    PR2				
		MOVLW	    0x11			;SET PR2 TO 17 FOR 56kHz PERIOD
		MOVWF	    PR2	
		BANKSEL	    T2CON
		BSF	    T2CON,2
		GOTO	    TEST_MODE