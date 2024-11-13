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
		GOTO	    GOBACK
		BANKSEL	    PORTB
		BTFSS	    PORTB,0
		GOTO	    STOP_FIRING	
		BANKSEL	    TRISC
		BCF	    TRISC,2
		BANKSEL	    PORTC
		BSF	    PORTC,3
		BANKSEL	    TMR1L
		CLRF	    TMR1L
		CLRF	    TMR1H
		
		BANKSEL	    PIR1
		BCF	    PIR1,0
	POLL_TMR1_SEMI	
		BTFSS	    PIR1,0
		GOTO	    POLL_TMR1_SEMI	
		BANKSEL	    PORTC
		BCF	    PORTC,3
		GOTO	    STOP_FIRING		
				
    FIRE_BURST
		BANKSEL	    IOCBF
		BTFSS	    IOCBF,0
		GOTO	    GOBACK
		BANKSEL	    PORTB
		BTFSS	    PORTB,0
		GOTO	    STOP_FIRING	
		BANKSEL	    TRISC
		BCF	    TRISC,2
		BANKSEL	    TMR1L
		CLRF	    TMR1L
		CLRF	    TMR1H
		BANKSEL	    PIR1
		BCF	    PIR1,0
	POLL_TMR1_BURST1
		BTFSS	    PIR1,0
		GOTO	    POLL_TMR1_BURST1
		BANKSEL	    TRISC
		BSF	    TRISC,2
		BANKSEL	    PIR1
		BCF	    PIR1,0
	POLL_TMR1_BURST2
		BTFSS	    PIR1,0
		GOTO	    POLL_TMR1_BURST2
		BANKSEL	    TRISC
		BCF	    TRISC,2
		BANKSEL	    PIR1
		BCF	    PIR1,0
	POLL_TMR1_BURST3
		BTFSS	    PIR1,0
		GOTO	    POLL_TMR1_BURST3
		BANKSEL	    TRISC
		BSF	    TRISC,2
		BANKSEL	    PIR1
		BCF	    PIR1,0
	POLL_TMR1_BURST4
		BTFSS	    PIR1,0
		GOTO	    POLL_TMR1_BURST4
		BANKSEL	    TRISC
		BCF	    TRISC,2
		BANKSEL	    PIR1
		BCF	    PIR1,0
	POLL_TMR1_BURST5
		BTFSS	    PIR1,0
		GOTO	    POLL_TMR1_BURST5		
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
		

    
    
		