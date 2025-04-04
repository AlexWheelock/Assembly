;************************************************************************************
;										    *
;   Filename:	    ADC.asm							    *
;   Date:	    October 4, 2024						    *
;   File Version:   2								    *
;   Author:	    Alex Wheelock						    *
;   Company:	    Idaho State University					    *
;   Description:    This program will be used to convert the analog signal coming   *
;		    from a temperature sensor into a binary value, and output the   *
;		    temperature onto PORTC, illuminating the count on a set of 8    *
;		    LEDs.							    *
;										    *
;************************************************************************************

;************************************************************************************
;										    *
;   Revision History:								    *
;										    *
;   1:	Initialize file, and set up the file along with includes		    *
;										    *
;   2:	Setup the file to take the input from the temperature sensor and output the *
;	binary temperature value on LEDs on PORTC.				    *
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

W_TEMP		 EQU  0X20				;Used to store the working register upon an interrupt
STATUS_TEMP	 EQU  0x21				;Used to store the STATUS register upon an interrupt
							
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
		BANKSEL	PORTB				;/
		BTFSC	PORTB,0				;\Determine if ADIF is set, if not then leave ISR
		GOTO	SETAA
		BTFSC	PORTB,1
		GOTO	SET55
		GOTO	TEST_RCREG
	SETAA
		BANKSEL	TXREG
		MOVLW	0xAA
		MOVWF	TXREG
		GOTO	POLL_TRMT
	SET55
		BANKSEL	TXREG
		MOVLW	0x55
		MOVWF	TXREG
		GOTO	POLL_TRMT
	POLL_TRMT
		BANKSEL	TXSTA
		BTFSS	TXSTA,1
		GOTO	POLL_TRMT
	TEST_RCREG
		BANKSEL	PIR1
		BTFSS	PIR1,5
		GOTO	GOBACK
		BTFSC	RCREG,7
		GOTO	SETOUTPUT
		BCF	PORTC,0
		GOTO	GOBACK
	SETOUTPUT
		BSF	PORTC,0
	GOBACK
		BANKSEL	INTCON
		BCF	INTCON,0			;Clear ADIF, allowing interrupts to occur again
		BANKSEL	PIR1
		BCF	PIR1,5
		SWAPF	STATUS_TEMP,W			;/
		MOVWF	STATUS				;Move the previous W & STATUS registers back into the W & STATUS registers
		SWAPF	W_TEMP,F			;
		SWAPF	W_TEMP,W			;\
		RETFIE					;Return to MAIN, Re-enable global interrupt
		
;******************************************
;Main Code
;******************************************
MAIN	
		NOP					;Do nothing in MAIN
		GOTO	MAIN				;Restart back to MAIN
END
		
;******************************************		
;END PROGRAM DIRECTIVE
;******************************************