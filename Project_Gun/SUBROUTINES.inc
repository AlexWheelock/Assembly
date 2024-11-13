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
 
    TEST_ACTIVE_FIRING
		BANKSEL	    PORTB
		BSF	    PORTC,0
		BTFSC	    SEMI_ACTIVE,0
		GOTO	    FINISH_SEMI
		BTFSC	    BURST_ACTIVE,0
		GOTO	    CONTINUE_BURST
		GOTO	    TEST_MODE
 
 
    FIRE_CONTINUOUS
		BANKSEL	    PORTB
		BTFSS	    PORTB,0
		GOTO	    STOP_FIRING
		BANKSEL	    TRISC
		BTFSC	    TRISC,2
		BCF	    TRISC,2
		GOTO	    GOBACK
		
    STOP_FIRING
		BANKSEL	    TRISC
		BTFSS	    TRISC,2
		BSF	    TRISC,2
		GOTO	    GOBACK
 
    FIRE_SEMI
		BANKSEL	    IOCBF
		BTFSS	    IOCBF,0
		GOTO	    STOP_FIRING
		BANKSEL	    PORTB
		BTFSS	    PORTB,0
		GOTO	    STOP_FIRING	
		BANKSEL	    TRISC
		BCF	    TRISC,2
		BANKSEL	    T1CON
		BCF	    T1CON, 0
		BANKSEL	    TMR1L
		CLRF	    TMR1L
		CLRF	    TMR1H
		BCF	    PIR1,0
		;BSF	    SEMI_ACTIVE,0
		BANKSEL	    T1CON
		BSF	    T1CON, 0
		BANKSEL	    PIR1
		BTFSS	    PIR1,0
		GOTO	    $-1
		BANKSEL	    TRISC
		BSF	    TRISC,2
		GOTO	    GOBACK
		
    FINISH_SEMI
		BCF	    SEMI_ACTIVE,0
		GOTO	    STOP_FIRING
		
    FIRE_BURST
		BANKSEL	    IOCBF
		BTFSS	    IOCBF,0
		GOTO	    GOBACK
		BANKSEL	    PORTB
		BTFSS	    PORTB,0
		GOTO	    STOP_FIRING
		GOTO	    STOP_FIRING
		
		
		
    CONTINUE_BURST	
		GOTO	    STOP_FIRING
    
    
    SET_38KHZ
		BANKSEL	    CCPR1L
		MOVLW	    0x13
		MOVWF	    CCPR1L		
		BANKSEL	    PR2				
		MOVLW	    0x19			;SET PR2 TO 25 FOR 38kHz PERIOD
		MOVWF	    PR2	
		BANKSEL	    TMR2
		CLRF	    TMR2
		GOTO	    TEST_TMR1
    SET_56KHZ
		BANKSEL	    CCPR1L
		MOVLW	    0x0D
		MOVWF	    CCPR1L		
		BANKSEL	    PR2				
		MOVLW	    0x11			;SET PR2 TO 17 FOR 56kHz PERIOD
		MOVWF	    PR2	
		BANKSEL	    TMR2
		CLRF	    TMR2
		GOTO	    TEST_TMR1
		

    
    
		