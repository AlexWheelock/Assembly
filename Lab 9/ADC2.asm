;************************************************************************************
;										    *
;   Filename:	    ADC.asm							    *
;   Date:	    October 4, 2024						    *
;   File Version:   3								    *
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
;   3:	File setup to drive a servo motor on PORTC
;										    *	
;************************************************************************************		
		
		#INCLUDE <p16f883.inc>        		; processor specific variable definitions
		#INCLUDE <16F883_SETUP2.inc>		; Custom setup file for the PIC16F883 micro-controller
		#INCLUDE <SUBROUTINES2.inc>		; File containing all used subroutines
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
		BANKSEL	PIR1
		BTFSC	PIR1,5
		CALL	LOAD_RCREG
		BTFSS	PIR1,6
		GOTO	SKIP_ADC_SAVE
		CALL	SAVE_ADC
	SKIP_ADC_SAVE
		BTFSS	PIR1,1				;TEST IF TMR2IF IS SET, IF NOT THEN LEAVE ISR, OTHERWISE CONTINUE
		GOTO	GOBACK				;TMR2IF NOT SET, LEAVE ISR
		DECFSZ	PERIOD				;DECREMENT PERIOD TO SET 20ms CYCLE TIME ON OUTPUT
		GOTO	CONTINUE			;STILL IN THE MIDDLE OF A CYCLE, CONTINUE TO CHECK PW TIME REMAINING
		GOTO	NEW_CYCLE			;PERIOD = 0, RESTART A NEW CYCLE
	CONTINUE					;
		BTFSS	PW_TRUE,0			;TEST IF A PULSE WIDTH IS OCCURING, IF NOT THEN LEAVE ISR, OTHERWISE DECREMENT COUNT OF PW REMAINING
		GOTO	GOBACK				;PW IS NOT ACTIVE, CONTINUE FINISHING THE CYCLE BY LEAVING ISR
		DECFSZ	PW_COUNT			;PW IS ACTIVE, DECREMENT COUNT. IF COUNT IS UP THEN SET OUTPUT LOW AND CLEAR PW_TRUE
		GOTO	GOBACK				;PW IS STILL ACTIVE, LEAVE ISR
		BCF	PW_TRUE,0			;CLEAR PW_TRUE WHEN PW TIME IS UP
		BCF	PORTA,1				;CLEAR OUTPUT TO SET IT LOW WHEN PW TIME IS UP
	GOBACK
		BCF	PIR1,1
		BCF	PIR1,5
		BCF	PIR1,6
		SWAPF	STATUS_TEMP,W			;/
		MOVWF	STATUS				;Move the previous W & STATUS registers back into the W & STATUS registers
		SWAPF	W_TEMP,F			;
		SWAPF	W_TEMP,W			;\
		RETFIE					;Return to MAIN, Re-enable global interrupt

;******************************************
;Main Code
;******************************************
MAIN
		BANKSEL	PIR1
		BTFSS	HANDSHAKE,0
		CALL	TEST_HANDSHAKE
		
		BTFSS	NEW_BYTE,1
		GOTO	TEST_NEW_BYTE0
		BTFSC	ADC_REQUEST,0
		CALL	TEST_ADC_REQUEST
		GOTO	MAIN
	TEST_NEW_BYTE0
		BTFSC	NEW_BYTE,0
		CALL	SAVE_RCREG
		BTFSC	RCSTA,1
		BSF	PORTC,1
		GOTO	MAIN
END
		
;******************************************		
;END PROGRAM DIRECTIVE
;******************************************