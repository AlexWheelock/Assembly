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
 
    SET_F				
		MOVLW	H'46'				;
		GOTO	SAVE_DATA				;RESTART LOOP
    SET_E
		MOVLW	H'45'				;
		GOTO	SAVE_DATA				;RESTART LOOP
    SET_D
		MOVLW	H'44'				;
		GOTO	SAVE_DATA				;RESTART LOOP
    SET_C
		MOVLW	H'43'				;
		GOTO	SAVE_DATA				;RESTART LOOP
    SET_B
		MOVLW	H'42'				;
		GOTO	SAVE_DATA				;RESTART LOOP
    SET_A
		MOVLW	H'41'				;
		GOTO	SAVE_DATA				;RESTART LOOP
    SET_9
		MOVLW	H'39'				;
		GOTO	SAVE_DATA				;RESTART LOOP
    SET_8
		MOVLW	H'38'				;
		GOTO	SAVE_DATA				;RESTART LOOP
    SET_7
		MOVLW	H'37'				;
		GOTO	SAVE_DATA				;RESTART LOOP
    SET_6
		MOVLW	H'36'				;
		GOTO	SAVE_DATA				;RESTART LOOP
    SET_5
		MOVLW	H'35'				;
		GOTO	SAVE_DATA				;RESTART LOOP
    SET_4
		MOVLW	H'34'				;
		GOTO	SAVE_DATA				;RESTART LOOP
    SET_3
		MOVLW	H'33'				;
		GOTO	SAVE_DATA				;RESTART LOOP	
    SET_2
		MOVLW	H'32'				;
		GOTO	SAVE_DATA				;RESTART LOOP
    SET_1
		MOVLW	H'31'				;
		GOTO	SAVE_DATA				;RESTART LOOP
    SET_0
		MOVLW	H'30'				;
	SAVE_DATA
		BANKSEL	PORTC
		MOVWF	PORTC
		MOVWF	CURRENT_VALUE
		BCF 	TEST_INPUTS,0
		BSF	INPUT_FLAG,0
		GOTO	MAIN
		
		
    TEST_MODE_CHANGE
		BANKSEL	PORTA
		BTFSC	PORTA,0
		GOTO	START_RECORD
		BTFSC	PORTA,1
		GOTO	START_PLAY
		DECFSZ	IDLE_COUNT
		GOTO	GOBACK
    CONTINUE_IDLE
		BTFSC	PORTC,0
		GOTO	CLEAR_OUTPUT
		MOVLW	0x53
		MOVWF	PORTC
		GOTO	RESET_IDLE
	CLEAR_OUTPUT
		MOVLW	0x20
		MOVWF	PORTC
	RESET_IDLE
		MOVLW	0x64
		MOVWF	IDLE_COUNT
		GOTO	GOBACK
		
		
    START_RECORD
    
    
    
    CONTINUE_RECORD
		
    
    
    START_PLAY
		MOVLW	0x32
		MOVWF	PLAY_COUNT
		BSF	PLAY_MODE,0
		CLRF	CURRENT_REGISTER
		MOVFW	CURRENT_VALUE
		BANKSEL	EEDATA
		MOVWF	EEDATA
		BANKSEL	PORTA
		MOVFW	CURRENT_REGISTER
		BANKSEL	EEADR
		MOVWF	EEADR
		BANKSEL	EECON1
		BSF	EECON1,2
		BSF	EECON1,1
		BANKSEL	PORTA
		GOTO	GOBACK
    
    
    CONTINUE_PLAY
    
    
    
    STOP_BUTTON_PRESSED
		BSF	IDLE,0
		CLRF	CURRENT_REGISTER
		MOVLW	0x64
		MOVWF	IDLE_COUNT
		BCF	PLAY_MODE,0
		BCF	RECORD_MODE,0
		BCF	INPUT_FLAG,0