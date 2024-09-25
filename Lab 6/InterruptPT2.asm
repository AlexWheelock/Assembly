;************************************************************************************
;										    *
;   Filename:	    InterruptPT2.asm						    *
;   Date:	    September 24, 2024						    *
;   File Version:   1								    *
;   Author:	    Alex Wheelock						    *
;   Company:	    Idaho State University					    *
;   Description:    Program that sets a dot-matrix display to display a "1",	    *
;		    but pressing a button on RB0 causes an interrupt to display	    *
;		    a "7" for two seconds before resuming the normal operation	    *
;		    of displaying a "1". RB1 pressed will change cause an interrupt *
;		    displaying a "6", being able to be interrupted by the RB0 button*
;		    but cannot override the RB1 button.				    *
;										    *
;************************************************************************************

;************************************************************************************
;										    *
;   Revision History:								    *
;										    *
;   1:	Initialize file, and set up the file along with includes		    *
;										    *
;   2:	Setup interrupt when RB0 push button is pressed to cause a 2 second change  * 
;	on the output, making the dot-matrix display a 7, then reverting back to a  *
;	"1".									    *
;										    *	
;   3:	Adding a push button that displays a "6" on RB1, that can be interrupted by *
;	the button on RB0, but cannot override the RB0 ISR.			    *
;										    *	
;************************************************************************************		
		
		#INCLUDE <p16f883.inc>        		; processor specific variable definitions
		#INCLUDE <16F883_SETUP_PT2.inc>		; Custom setup file for the PIC16F883 micro-controller
		#INCLUDE <SUBROUTINES.inc>		; File containing all of the used subroutines
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

    W_TEMP	EQU	0X20				;Used to store W upon an interrupt from RB0
    STATUS_TEMP	EQU	0X21				;Used to store STATUS on an interrupt from RB0
    COUNT1	EQU	0X22				;/
    COUNT2	EQU	0X23				;
    COUNT3	EQU	0X24				;Registers used to set the 2 second delay when the RB0 interrupt occurs
    COUNT4	EQU	0x25		 		;
    COUNT5	EQU	0x26				;\
    COUNT6	EQU	0X27		 		;/
    COUNT7	EQU	0X28		 		;
    COUNT8	EQU	0X29		 		;Registers used to set the 2 second delay when the RB0 interrupt occurs
    COUNT9	EQU	0X2A		 		;
    COUNT10	EQU	0X2B		 		;\
    W_TEMP2	EQU	0X2C				;Used to store W upon an interrupt from RB1
    STATUS_TEMP2 EQU	0X2D				;Used to store STATUS on an interrupt from RB1
							;Separate delays because the RB1 interrupt will ruin the delay that the RB0 delay had remaining if interrupted in the middle of the ISR
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
		BTFSS	PORTB,RB0			;Test which interrupt is occurring
		GOTO	SET6				;RB1 is true, skip over if RB0 is true
		MOVWF	W_TEMP				;/
		SWAPF	STATUS,W			;Saves the W & STATUS registers into a temporary location to not interfere with the MAIN code that was interrupted, when resumed
		MOVWF	STATUS_TEMP			;\
		BANKSEL	PORTC			
		MOVLW	0X37				;/Set the output to display a "7"
		MOVWF	PORTC				;\
		CALL	DELAY				;Cause a 2 second delay
		SWAPF	STATUS_TEMP,W			;/
		MOVWF	STATUS				;Move the previous W & STATUS registers back into the W & STATUS registers
		SWAPF	W_TEMP,F			;
		SWAPF	W_TEMP,W			;\
		BCF	INTCON,RBIF			;Clear RBIF to allow another interrupt to occur, placed at the end to give it priority
		RETFIE					;Return to last address in the stack, enable interrupts
	SET6	
		MOVWF	W_TEMP2				;/
		SWAPF	STATUS,W			;Saves the W & STATUS registers into a temporary location to not interfere with the MAIN code that was interrupted
		MOVWF	STATUS_TEMP2			;\
		BANKSEL	PORTC
		BCF	INTCON,RBIF			;Clear RBIF to allow another interrupt to occur, placed at the beginning to allow it to be interrupted
		MOVLW	0X36				;/Set the output to display a "6"
		MOVWF	PORTC				;\
		CALL	DELAY				;Cause a 2 second delay
		SWAPF	STATUS_TEMP2,W			;/
		MOVWF	STATUS				;Move the previous stored W & STATUS registers back into the W & STATUS registers before resuming MAIN code
		SWAPF	W_TEMP2,F			;
		SWAPF	W_TEMP2,W			;\
		RETFIE					;Return to last address in the stack, enable interrupts
		
;******************************************
;Main Code
;******************************************
MAIN	
		BANKSEL	PORTC				;
		MOVLW	0X31				;/Set output to display a "1"
		MOVWF	PORTC				;\
		GOTO	MAIN				;Restart back to MAIN
END
		
;******************************************		
;END PROGRAM DIRECTIVE
;******************************************