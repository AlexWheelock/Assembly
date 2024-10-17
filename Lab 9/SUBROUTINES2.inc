;************************************************************************************
;										    *
;   Filename:	    SUBROUTINES.asm						    *
;   Date:	    October 4, 2024						    *
;   File Version:   2								    *
;   Author:	    Alex Wheelock						    *
;   Company:	    Idaho State University					    *
;   Description:    Contains all subroutines needed for the Servo section of Lab 8. *
;										    *
;************************************************************************************

;************************************************************************************
;										    *
;   Revision History:								    *
;										    *
;   1:	Initialize file, set up for ADC conversion and output to PORTC.		    *
;										    *	    
;   2:	Setup file to use the ADC to control a servo motor on PORTC		    *
;										    *
;************************************************************************************		
	    
SUBROUTINES_CODE CODE

LOAD_RCREG
		BANKSEL	RCREG
		MOVFW	RCREG
		MOVWF	STORED_RCREG
		BTFSC	HANDSHAKE,0
		BSF	NEW_BYTE,0
		RETURN
 
TEST_HANDSHAKE
		BTFSC	STORED_RCREG,7
		GOTO	LEAVE_SUB
		BTFSC	STORED_RCREG,6
		GOTO	LEAVE_SUB
		BTFSS	STORED_RCREG,5
		GOTO	LEAVE_SUB
		BTFSC	STORED_RCREG,4
		GOTO	LEAVE_SUB
		BTFSC	STORED_RCREG,3
		GOTO	LEAVE_SUB
		BTFSS	STORED_RCREG,2
		GOTO	LEAVE_SUB
		BTFSC	STORED_RCREG,1
		GOTO	LEAVE_SUB
		BTFSC	STORED_RCREG,0
		GOTO	LEAVE_SUB
		BSF	HANDSHAKE,0
		MOVLW	0x24
		MOVWF	TXREG
		GOTO	LEAVE_SUB
LEAVE_SUB
		RETURN
		
SAVE_RCREG
		BANKSEL	RCREG
		MOVFW	RCREG
		MOVWF	CURRENT_RCREG
		BCF	HANDSHAKE,0	
		BCF	NEW_BYTE,0
		RETURN
		
NEW_CYCLE		
		BANKSEL	PORTA
		BSF	PW_TRUE,0
		BSF	PORTA,1
		MOVLW	0xC8
		MOVWF	PERIOD
		MOVFW	CURRENT_RCREG
		MOVWF	PW_COUNT
		GOTO	GOBACK