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
 
TRANSMIT_HANDSHAKE
	BANKSEL	    TX1REG
	MOVLW	    0x21
	MOVWF	    TX1REG
	RETURN
	
TRANSMIT_FORWARD
	CALL	    TRANSMIT_HANDSHAKE
	BANKSEL	    TX1STA
	BTFSS	    TX1STA,1
	GOTO	    $-1
	BANKSEL	    TX1REG
	MOVLW	    0x46
	MOVWF	    TX1REG
	BANKSEL	    PORTA
	BSF	    VALID_INPUT,0
	GOTO	    GOBACK

	
TRANSMIT_BACKWARD
	CALL	    TRANSMIT_HANDSHAKE
	BANKSEL	    TX1STA
	BTFSS	    TX1STA,1
	GOTO	    $-1
	BANKSEL	    TX1REG
	MOVLW	    0x42
	MOVWF	    TX1REG
	BANKSEL	    PORTA
	BSF	    VALID_INPUT,0
	GOTO	    GOBACK	
	
	
TRANSMIT_LEFT
	CALL	    TRANSMIT_HANDSHAKE
	BANKSEL	    TX1STA
	BTFSS	    TX1STA,1
	GOTO	    $-1
	BANKSEL	    TX1REG
	MOVLW	    0x4C
	MOVWF	    TX1REG
	BANKSEL	    PORTA
	BSF	    VALID_INPUT,0
	GOTO	    GOBACK
	
	
TRANSMIT_RIGHT
	CALL	    TRANSMIT_HANDSHAKE
	BANKSEL	    TX1STA
	BTFSS	    TX1STA,1
	GOTO	    $-1
	BANKSEL	    TX1REG
	MOVLW	    0x52
	MOVWF	    TX1REG
	BANKSEL	    PORTA
	BSF	    VALID_INPUT,0
	GOTO	    GOBACK
	

TOGGLE_ARMS
	CALL	    TRANSMIT_HANDSHAKE
	BANKSEL	    TX1STA
	BTFSS	    TX1STA,1
	GOTO	    $-1
	BANKSEL	    TX1REG
	MOVLW	    0x3E
	MOVWF	    TX1REG
	BANKSEL	    PORTA
	BSF	    VALID_INPUT,0
	GOTO	    GOBACK
	
TEST_CONTINUE
	BTFSC	    PORTB,0
	GOTO	    STILL_VALID_INPUT
	BTFSC	    PORTB,1
	GOTO	    STILL_VALID_INPUT
	BTFSC	    PORTB,2
	GOTO	    STILL_VALID_INPUT
	BTFSC	    PORTB,3
	GOTO	    STILL_VALID_INPUT
	GOTO	    GOBACK
    STILL_VALID_INPUT
	CALL	    TRANSMIT_HANDSHAKE
	BANKSEL	    TX1STA
	BTFSS	    TX1STA,1
	GOTO	    $-1
	BANKSEL	    TX1REG
	MOVLW	    0x58
	MOVWF	    TX1REG
	BANKSEL	    PORTA
	BCF	    VALID_INPUT,0
	GOTO	    GOBACK
	
	
	