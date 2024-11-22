;************************************************************************************
;										    *
;   Filename:	    Project_Gun.asm						    *
;   Date:	    November 5, 2024						    *
;   File Version:   1								    *
;   Author:	    Alex Wheelock						    *
;   Company:	    Idaho State University					    *
;   Description:    
;										    *
;************************************************************************************

;************************************************************************************
;										    *
;   Revision History:								    *
;										    *
;   1:	  *
;										    *	
;************************************************************************************		
		
		#INCLUDE "p16f1788.inc"      		; processor specific variable definitions
		#INCLUDE <16F1788_SETUP.inc>		; Custom setup file for the PIC16F883 micro-controller
		#INCLUDE <SUBROUTINES.inc>		; File containing all used subroutines
		LIST      p=16f1788		  	; list directive to define processor
		errorlevel -302,-207,-305,-206,-203	; suppress "not in bank 0" message,  Found label after column 1,
							; Using default destination of 1 (file),  Found call to macro in column 1

		; CONFIG1
; __config 0xC9A4
 __CONFIG _CONFIG1, _FOSC_INTOSC & _WDTE_OFF & _PWRTE_OFF & _MCLRE_OFF & _CP_OFF & _CPD_OFF & _BOREN_OFF & _CLKOUTEN_OFF & _IESO_OFF & _FCMEN_OFF
; CONFIG2
; __config 0xDFFF
 __CONFIG _CONFIG2, _WRT_OFF & _PLLEN_ON & _STVREN_ON & _BORV_LO & _LPBOR_OFF & _LVP_OFF


 
;******************************************		
;Define Variable Registers
;******************************************


							
;******************************************		
;Interrupt Vectors
;******************************************
		ORG	    H'000'			;ORGs beginning of code
		GOTO	    SETUP			;RESET CONDITION GOTO SETUP
		ORG	    H'004'			;ORGs interrupt location
		GOTO	    INTERRUPT			;Interrupt occurred, carry out ISR

;******************************************
;SETUP ROUTINE
;******************************************
SETUP
		CALL	    INITIALIZE			;Call setup include file to initialize the PIC
		GOTO	    MAIN			;END OF SETUP ROUTINE
		
;******************************************
;INTERRUPT SERVICE ROUTINE
;******************************************
INTERRUPT
		BANKSEL	    PIR1
		BTFSC	    PIR1,5
		CALL	    RECEIVE_BYTE
		BTFSC	    PIR1,1
		;GOTO	    TRANSMIT_STUFF
		CALL	    TEST_CONTINUE
	GOBACK
		BANKSEL	    PIR1
		BCF	    PIR1,5			;Clear RCIF
		BCF	    PIR1,1
		RETFIE					;Return to MAIN, Re-enable global interrupt
		
;******************************************
;Main Code
;******************************************
MAIN	
		;BANKSEL	    RC1STA
		;BTFSC	    RC1STA,1
		;GOTO	    THROW_OERR
		GOTO	    MAIN
	THROW_OERR
		;BANKSEL	    PORTA
		;BSF	    PORTA,7
		;GOTO	    MAIN
END
		
;******************************************		
;END PROGRAM DIRECTIVE
;******************************************