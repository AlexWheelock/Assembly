;************************************************************************************
;										    *
;   Filename:	    Traffic_Light.asm						    *
;   Date:	    September 30, 2024						    *
;   File Version:   2								    *
;   Author:	    Alex Wheelock						    *
;   Company:	    Idaho State University					    *
;   Description:    Program that simulates an intersection using LEDs on PORTC.	    *
;		    One is N/S, the other is E/W, and each push on a button on PORTB*
;		    indicates a car approaching the intersection from one of the two*
;		    directions that corresponds with that button.		    *
;										    *		
;		    Normal operation is 5 seconds green, 1 second yellow before	    *
;		    turning red and swapping. If there are cars only in one directon*
;		    then that light will be green until all cars are through, or    *
;		    until a car approaches from the other direction, where it will  *
;		    resume normal operation.					    *
;										    *
;************************************************************************************

;************************************************************************************
;										    *
;   Revision History:								    *
;										    *
;   1:	Initialize file, and set up the file along with includes		    *
;										    *
;   2:	Setup the interrupts for the light, and the timers necessary.		    *
;										    *	
;************************************************************************************		
		
		#INCLUDE <p16f883.inc>        		; processor specific variable definitions
		#INCLUDE <16F883_SETUP.inc>		; Custom setup file for the PIC16F883 micro-controller
		#INCLUDE <SUBROUTINES.inc>		; File containing all used subroutines
		LIST      p=16f883		  	; list directive to define processor
		errorlevel -302,-207,-305,-206,-203	; suppress "not in bank 0" message,  Found label after column 1,
							; Using default destination of 1 (file),  Found call to macro in column 1

		; CONFIG1
; __config 0xE0E2
 __CONFIG _CONFIG1, _FOSC_HS & _WDTE_OFF & _PWRTE_ON & _MCLRE_OFF & _CP_OFF & _CPD_OFF & _BOREN_OFF & _IESO_OFF & _FCMEN_OFF & _LVP_OFF
; CONFIG2
; __config 0xFFFF
 __CONFIG _CONFIG2, _BOR4V_BOR40V & _WRT_OFF


;******************************************		
;Define Constants
;******************************************

#Define		FOSC		D'4000'			;Oscillator Clock in KHz This must be filled

;******************************************		
;Define Variable Registers
;******************************************

    W_TEMP	 EQU  H'0020'
    STATUS_TEMP	 EQU  H'0021'
    END_GREEN  	 EQU  H'0022'
    END_YELLOW	 EQU  H'0023'
    NS_TRUE	 EQU  H'0024'
    GREEN	 EQU  H'0025'
    YELLOW	 EQU  H'0026'
							
;******************************************		
;Interrupt Vectors
;******************************************
		ORG	H'000'				;ORGs beginning of code
		GOTO	SETUP				;RESET CONDITION GOTO SETUP
		ORG	H'004'				;ORGs interrupt location
		GOTO	INTERRUPT			;Interrupt occurred, carry out ISR

;******************************************
;SETUP ROUTINE
;******************************************
SETUP
		CALL	INITIALIZE			;Call setup include file to initialize the PIC
		GOTO	MAIN				;END OF SETUP ROUTINE
		
;******************************************
;INTERRUPT SERVICE ROUTINE
;******************************************
INTERRUPT
		MOVWF	W_TEMP				;/
		SWAPF	STATUS,W			;Saves the W & STATUS registers into a temporary location to not interfere with the MAIN code that was interrupted, when resumed
		MOVWF	STATUS_TEMP			;\
		BANKSEL	PIR1
		BTFSC	PORTB,0
		INCF	NS_CAR_COUNT
		BTFSC	PORTB,1
		INCF	EW_CAR_COUNT
		BTFSC	PIR1,1
		CALL	TRAFFIC_LIGHT
		GOTO	GOBACK
	GOBACK
		BCF	PIR1,1				;Clear TMR2IF, allowing interrupts to occur again
		SWAPF	STATUS_TEMP,W			;/
		MOVWF	STATUS				;Move the previous W & STATUS registers back into the W & STATUS registers
		SWAPF	W_TEMP,F			;
		SWAPF	W_TEMP,W			;\
		RETFIE
		
;******************************************
;Main Code
;******************************************
MAIN	
		NOP
		GOTO	MAIN				;Restart back to MAIN
END
		
;******************************************		
;END PROGRAM DIRECTIVE
;******************************************