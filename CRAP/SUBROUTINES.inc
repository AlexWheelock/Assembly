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
 
TRANSMIT_STUFF
	BANKSEL	    TX1REG
	MOVLW	    0xAA
	MOVWF	    TX1REG
	GOTO	    GOBACK
 
RECEIVE_BYTE
	BANKSEL	    RC1REG
	MOVFW	    RC1REG
	BANKSEL	    PORTA
	MOVWF	    RECEIVED_BYTE
	BTFSC	    BYTE2_RECEIVED,0
	GOTO	    CONTINUE
	BTFSC	    BYTE1_RECEIVED,0
	GOTO	    TEST_BYTE2
	GOTO	    TEST_BYTE1
	
ABORT
	CLRF	    BYTE1_RECEIVED
	CLRF	    BYTE2_RECEIVED
	CLRF	    BYTE3_RECEIVED
	CLRF	    BYTE4_RECEIVED
	BANKSEL	    CCPR1L
	MOVLW	    0x0B			;PW FOR INITIALIZING 1.5ms PW AT 50Hz
	MOVWF	    CCPR1L
	BANKSEL	    CCPR2L
	MOVLW	    0x0B			;PW FOR INITIALIZING 1.5ms PW AT 50Hz
	MOVWF	    CCPR2L
	GOTO	    GOBACK
	
DRIVE_FORWARD
	BANKSEL	    PORTA
	BSF	    PORTA,0
	BCF	    PORTA,1
	BCF	    PORTA,2
	BCF	    PORTA,3
	BANKSEL	    CCPR1L
	MOVLW	    0x0F			;PW FOR INITIALIZING 1.5ms PW AT 50Hz
	MOVWF	    CCPR1L
	BANKSEL	    CCPR2L
	MOVLW	    0x07			;PW FOR INITIALIZING 1.5ms PW AT 50Hz
	MOVWF	    CCPR2L
	GOTO	    GOBACK	
	
DRIVE_BACK
	BANKSEL	    PORTA
	BCF	    PORTA,0
	BSF	    PORTA,1
	BCF	    PORTA,2
	BCF	    PORTA,3
	BANKSEL	    CCPR1L
	MOVLW	    0x07			;PW FOR INITIALIZING 1.5ms PW AT 50Hz
	MOVWF	    CCPR1L
	BANKSEL	    CCPR2L
	MOVLW	    0x0F			;PW FOR INITIALIZING 1.5ms PW AT 50Hz
	MOVWF	    CCPR2L
	GOTO	    GOBACK	
	
TURN_LEFT
	BANKSEL	    PORTA
	BCF	    PORTA,0
	BCF	    PORTA,1
	BSF	    PORTA,2
	BCF	    PORTA,3
	BANKSEL	    CCPR1L
	MOVLW	    0x07			;PW FOR INITIALIZING 1.5ms PW AT 50Hz
	MOVWF	    CCPR1L
	BANKSEL	    CCPR2L
	MOVLW	    0x07			;PW FOR INITIALIZING 1.5ms PW AT 50Hz
	MOVWF	    CCPR2L
	GOTO	    GOBACK	
	
TURN_RIGHT
	BANKSEL	    PORTA
	BCF	    PORTA,0
	BCF	    PORTA,1
	BCF	    PORTA,2
	BSF	    PORTA,3
	BANKSEL	    CCPR1L
	MOVLW	    0x0F			;PW FOR INITIALIZING 1.5ms PW AT 50Hz
	MOVWF	    CCPR1L
	BANKSEL	    CCPR2L
	MOVLW	    0x0F			;PW FOR INITIALIZING 1.5ms PW AT 50Hz
	MOVWF	    CCPR2L
	GOTO	    GOBACK
	
TOGGLE_ARMS
	BANKSEL	    PORTA
	BTFSS	    PORTA,4
	GOTO	    SET_ARMS
	BCF	    PORTA,4
	GOTO	    ABORT
    SET_ARMS
	BSF	    PORTA,4
	GOTO	    ABORT
	
	
TEST_BYTE1
	BANKSEL	    PORTA
	BTFSC	    RECEIVED_BYTE,7
	GOTO	    ABORT
	BTFSC	    RECEIVED_BYTE,6
	GOTO	    ABORT
	BTFSS	    RECEIVED_BYTE,5
	GOTO	    ABORT
	BTFSC	    RECEIVED_BYTE,4
	GOTO	    ABORT
	BTFSC	    RECEIVED_BYTE,3
	GOTO	    ABORT
	BTFSC	    RECEIVED_BYTE,2
	GOTO	    ABORT
	BTFSC	    RECEIVED_BYTE,1
	GOTO	    ABORT
	BTFSS	    RECEIVED_BYTE,0
	GOTO	    ABORT
	BSF	    BYTE1_RECEIVED,0
	GOTO	    GOBACK
	
	
TEST_BYTE2
	BANKSEL	    PORTA
	BSF	    BYTE2_RECEIVED,0
	BTFSC	    RECEIVED_BYTE,6
	GOTO	    BIT_6_SET
	GOTO	    TOGGLE_ARMS
    BIT_6_SET
	BTFSC	    RECEIVED_BYTE,4
	GOTO	    TURN_RIGHT
	BTFSC	    RECEIVED_BYTE,3
	GOTO	    TURN_LEFT
	BTFSC	    RECEIVED_BYTE,2
	GOTO	    DRIVE_FORWARD
	GOTO	    DRIVE_BACK

	
CONTINUE
	BANKSEL	    PORTA
	BSF	    RECEIVED_CONTINUE,0
	GOTO	    GOBACK
	
TEST_CONTINUE
	BANKSEL	    PORTA
	BTFSS	    RECEIVED_CONTINUE,0
	GOTO	    ABORT
	BCF	    RECEIVED_CONTINUE,0
	GOTO	    GOBACK