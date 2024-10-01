;************************************************************************************
;										    *
;   Filename:	    SUBROUTINES.asm						    *
;   Date:	    September 30, 2024						    *
;   File Version:   1								    *
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
;************************************************************************************		
	    
SUBROUTINES_CODE CODE
 
TRAFFIC_LIGHT
		BANKSEL	PORTC
		DECFSZ	END_GREEN
		GOTO	TEST_YELLOW
		DECF	END_YELLOW
		GOTO	SET_YELLOW
	TEST_YELLOW
		DECFSZ	END_YELLOW
		GOTO	RETURN_TO_INTERRUPT
		GOTO	SET_RED
	SET_YELLOW
		BTFSC	NS_TRUE,0
		GOTO	SET_NS_YELLOW
		MOVLW	0x14
		MOVWF	PORTC
		GOTO	RETURN_TO_INTERRUPT
	SET_NS_YELLOW
		MOVLW	0x22
		MOVWF	PORTC
		GOTO	RETURN_TO_INTERRUPT
	SET_RED
		BTFSC	NS_TRUE,0
		GOTO	SET_NS_RED
		MOVLW	0x21
		MOVWF	PORTC
		BSF	NS_TRUE,0
		GOTO	RESET_TRACKERS
	SET_NS_RED
		MOVLW	0x0C
		MOVWF	PORTC
		BCF	NS_TRUE,0
		GOTO	RESET_TRACKERS
	RESET_TRACKERS
		MOVLW	0xC8
		MOVWF	END_GREEN
		MOVLW	0xF0
		MOVWF	END_YELLOW
		GOTO	RETURN_TO_INTERRUPT
	RETURN_TO_INTERRUPT
		RETURN
