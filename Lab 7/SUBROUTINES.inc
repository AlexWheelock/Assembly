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
 
TRAFFIC_LIGHT
		BANKSEL	PORTC
		DECFSZ	END_GREEN		    ;Decrement from green light time tracking register
		GOTO	TEST_YELLOW		    ;Green time is not up, decrement Yellow time
		DECF	END_YELLOW		    ;Green time is up, decrement yellow to keep timing accurate
		GOTO	SET_YELLOW		    ;Determine if the light needs to be set yellow
	TEST_YELLOW				    ;
		DECFSZ	END_YELLOW		    ;Green time is not up, determine if yellow time is up
		GOTO	RETURN_TO_INTERRUPT	    ;Yellow time is not up, end interrupt
		GOTO	SET_RED			    ;Yellow time is up, set light to red
	SET_YELLOW				    ;Yellow time is up
		BTFSC	NS_TRUE,0		    ;Determine if NS is currently green
		GOTO	SET_NS_YELLOW		    ;NS is currently green
		BTFSS	NS_CAR,0		    ;Determine if a car is approaching red light
		GOTO	TEST_RESET_EW		    ;Car is not approaching red light, determine if car is approaching green
	MAKE_EW_YELLOW				    ;
		MOVLW	0x14			    ;/Set NS to Red
		MOVWF	PORTC			    ;\Set EW TO Yellow
		GOTO	RETURN_TO_INTERRUPT	    ;	
	TEST_RESET_EW				    ;
		BTFSS	EW_CAR,0		    ;Test if a car is approaching EW green light
		GOTO	MAKE_EW_YELLOW		    ;Car is not approaching green light, continue normal operation
		BCF	EW_CAR,0		    ;Car is approaching green light, clear EW_CAR flag
		GOTO	RESET_TRACKERS		    ;Reset time trackers to extend the green cycle 
	SET_NS_YELLOW				    ;
		BTFSS	EW_CAR,0		    ;Determine if there is an EW car approaching red
		GOTO	TEST_RESET_NS		    ;There is not a car approaching the red light
	MAKE_NS_YELLOW				    ;Car is approaching red light, continue normal operation
		MOVLW	0x22			    ;/Set NS to Red 
		MOVWF	PORTC	    		    ;\Set EW to Yellow
		GOTO	RETURN_TO_INTERRUPT	    ;
	TEST_RESET_NS		    		    ;
		BTFSS	NS_CAR,0		    ;Test if cars are approaching green light NS
		GOTO	MAKE_NS_YELLOW		    ;No NS cars approaching green, continue normal operation
		BCF	NS_CAR,0		    ;No EW cars approaching, but NS cars approaching, clear NS car approaching light, reset green cycle
		GOTO	RESET_TRACKERS		    ;
	SET_RED					    ;
		BTFSC	NS_TRUE,0		    ;Test if NS light is active, if not then EW is active
		GOTO	SET_NS_RED		    ;NS is active, set it to red
		MOVLW	0x21			    ;/Set NS green
		MOVWF	PORTC			    ;\Set EW red
		BCF	NS_CAR,0		    ;NS is now green, clear NS car approaching light
		BSF	NS_TRUE,0		    ;Set NS directional indicator
		GOTO	RESET_TRACKERS		    ;
	SET_NS_RED				    ;
		MOVLW	0x0C			    ;/Set NS red
		MOVWF	PORTC	    		    ;\Set EW green
		BCF	EW_CAR,0		    ;EW is now green, clear EW car approaching light
		BCF	NS_TRUE,0		    ;Clear NS directional indicator
		GOTO	RESET_TRACKERS		    ;
	RESET_TRACKERS				    ;
		MOVLW	0xC8			    ;/
		MOVWF	END_GREEN		    ;Reset the counts that track the time until the lights
		MOVLW	0xF0			    ;change to yellow/red to default for the next cycle.
		MOVWF	END_YELLOW		    ;\
		GOTO	RETURN_TO_INTERRUPT	    ;
	RETURN_TO_INTERRUPT			    ;
		RETURN		    		    ;RETURN from subroutine
