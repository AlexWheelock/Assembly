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
	BSF	    PORTA,3
	BTFSC	    BYTE9_RECEIVED,0
	GOTO	    TEST_CONTINUE
	BTFSC	    BYTE8_RECEIVED,0
	GOTO	    TEST_BYTE9
	BTFSC	    BYTE7_RECEIVED,0
	GOTO	    TEST_BYTE8
	BTFSC	    BYTE6_RECEIVED,0
	GOTO	    TEST_BYTE7
	BTFSC	    BYTE5_RECEIVED,0
	GOTO	    TEST_BYTE6
	BTFSC	    BYTE4_RECEIVED,0
	GOTO	    TEST_BYTE5
	BTFSC	    BYTE3_RECEIVED,0
	GOTO	    TEST_BYTE4
	BTFSC	    BYTE2_RECEIVED,0
	GOTO	    TEST_BYTE3
	BTFSC	    BYTE1_RECEIVED,0
	GOTO	    TEST_BYTE2
	GOTO	    TEST_BYTE1
	
ABORT
	CLRF	    BYTE1_RECEIVED
	CLRF	    BYTE2_RECEIVED
	CLRF	    BYTE3_RECEIVED
	CLRF	    BYTE4_RECEIVED
	CLRF	    BYTE5_RECEIVED
	CLRF	    BYTE6_RECEIVED
	CLRF	    BYTE7_RECEIVED
	CLRF	    BYTE8_RECEIVED
	CLRF	    BYTE9_RECEIVED
	CLRF	    BYTE10_RECEIVED
	CLRF	    BYTE11_RECEIVED
	CLRF	    BYTE12_RECEIVED
	BSF	    PORTA,4
	RETURN
	
DRIVE_FORWARD
	BANKSEL	    PORTA
	BSF	    PORTA,0
	BCF	    PORTA,1
	BCF	    PORTA,2
	BCF	    PORTA,3
	RETURN	
	
DRIVE_BACK
	BANKSEL	    PORTA
	BCF	    PORTA,0
	BSF	    PORTA,1
	BCF	    PORTA,2
	BCF	    PORTA,3
	RETURN	
	
TURN_LEFT
	BANKSEL	    PORTA
	BCF	    PORTA,0
	BCF	    PORTA,1
	BSF	    PORTA,2
	BCF	    PORTA,3
	RETURN	
	
TURN_RIGHT
	BANKSEL	    PORTA
	BCF	    PORTA,0
	BCF	    PORTA,1
	BCF	    PORTA,2
	BSF	    PORTA,3
	RETURN
	
TOGGLE_ARMS
	BANKSEL	    PORTA
	BTFSS	    PORTA,4
	GOTO	    SET_ARMS
	BCF	    PORTA,4
	RETURN
    SET_ARMS
	BSF	    PORTA,4
	RETURN
	
TEST_CONTINUE
	GOTO	    ABORT
	
	
TEST_BYTE1
	BANKSEL	    PORTA
	MOVWF	    RECEIVED_BYTE
	BTFSS	    RECEIVED_BYTE,7
	GOTO	    ABORT
	BTFSC	    RECEIVED_BYTE,6
	GOTO	    ABORT
	BTFSS	    RECEIVED_BYTE,5
	GOTO	    ABORT
	BTFSC	    RECEIVED_BYTE,4
	GOTO	    ABORT
	BTFSS	    RECEIVED_BYTE,3
	GOTO	    ABORT
	BTFSC	    RECEIVED_BYTE,2
	GOTO	    ABORT
	BTFSS	    RECEIVED_BYTE,1
	GOTO	    ABORT
	BTFSC	    RECEIVED_BYTE,0
	GOTO	    ABORT
	BSF	    BYTE1_RECEIVED,0
	;BSF	    PORTA,3
	RETURN
TEST_BYTE2
	BANKSEL	    PORTA
	;MOVWF	    RECEIVED_BYTE
	;BTFSC	    RECEIVED_BYTE,7
	;GOTO	    ABORT
	;BTFSS	    RECEIVED_BYTE,6
	;GOTO	    ABORT
	;BTFSC	    RECEIVED_BYTE,5
	;GOTO	    ABORT
	;BTFSS	    RECEIVED_BYTE,4
	;GOTO	    ABORT
	;BTFSC	    RECEIVED_BYTE,3
	;GOTO	    ABORT
	;BTFSS	    RECEIVED_BYTE,2
	;GOTO	    ABORT
	;BTFSC	    RECEIVED_BYTE,1
	;GOTO	    ABORT
	;BTFSS	    RECEIVED_BYTE,0
	;GOTO	    ABORT
	BSF	    BYTE2_RECEIVED,0
	RETURN
TEST_BYTE3
	BANKSEL	    PORTA
	BSF	    BYTE3_RECEIVED,0
	RETURN
TEST_BYTE4
 	BANKSEL	    PORTA
	BSF	    BYTE4_RECEIVED,0
	BTFSC	    RECEIVED_BYTE,2
	GOTO	    DRIVE_FORWARD
	GOTO	    DRIVE_BACK
	RETURN
TEST_BYTE5
 	BANKSEL	    PORTA
	BSF	    BYTE5_RECEIVED,0
	RETURN
TEST_BYTE6
 	BANKSEL	    PORTA
	BSF	    BYTE6_RECEIVED,0
	RETURN
TEST_BYTE7
	BANKSEL	    PORTA
	BSF	    BYTE7_RECEIVED,0
	RETURN
TEST_BYTE8
	BANKSEL	    PORTA
	BSF	    BYTE8_RECEIVED,0
	RETURN
TEST_BYTE9
	BANKSEL	    PORTA
	BSF	    BYTE9_RECEIVED,0
	RETURN
