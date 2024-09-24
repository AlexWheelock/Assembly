;************************************************************************************
;										    *
;   Filename:	    Interrupt.asm						    *
;   Date:	    September 24, 2024						    *
;   File Version:   1								    *
;   Author:	    Alex Wheelock						    *
;   Company:	    Idaho State University					    *
;   Description:    Program that sets a dot-matrix display to display a "1",	    *
;		    but pressing a button on RB0 causes an interrupt to display	    *
;		    a "7" for two seconds before resuming the normal operation	    *
;		    of displaying a "1".					    *
;										    *
;************************************************************************************

;************************************************************************************
;										    *
;   Revision History:								    *
;										    *
;   1:	Initialize file, and set up the file along with includes		    *
;										    *
;										    *
;										    *
;										    *		
;										    *
;										    *		
;										    *		
;										    *		
;************************************************************************************		
		
		#INCLUDE <p16f883.inc>        		; processor specific variable definitions
		#INCLUDE <16F883_SETUP.inc>		; Custom setup file for the PIC16F883 micro-controller
		LIST      p=16f883		  	; list directive to define processor
		errorlevel -302,-207,-305,-206,-203	;suppress "not in bank 0" message,  Found label after column 1,
							;Using default destination of 1 (file),  Found call to macro in column 1

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

 	COUNT1	EQU	0x20
	COUNT2	EQU	0x21
	COUNT3	EQU	0x22
	COUNT4	EQU	0x23
	COUNT5	EQU	0x24
	STEP	EQU	0x25
	LEG_1	EQU	0x26
		
;******************************************		
;Interrupt Vectors
;******************************************
		ORG	H'000'
		GOTO	SETUP				;RESET CONDITION GOTO SETUP
		ORG	H'004'
		GOTO	INTERRUPT

;******************************************
;SETUP ROUTINE
;******************************************
SETUP
		CALL	INITIALIZE
		GOTO	MAIN				;END OF SETUP ROUTINE
		
;******************************************
;INTERRUPT SERVICE ROUTINE
;******************************************
INTERRUPT
		
		
;******************************************
;Main Code
;******************************************
MAIN	
		BANKSEL	PORTC

END
		
;******************************************		
;END PROGRAM DIRECTIVE
;******************************************