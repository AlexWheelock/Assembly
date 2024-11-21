;************************************************************************************
;										    *
;   Filename:	    Project_Gun.asm						    *
;   Date:	    November 5, 2024						    *
;   File Version:   1								    *
;   Author:	    Alex Wheelock						    *
;   Company:	    Idaho State University					    *
;   Description:    Assembly file for Laser Shooting Game gun. A gun that can fire  *
;		    a laser at two frequencies (38kHz & 56kHz), with firing mode    *
;		    select (single-shot, three round burst, continuous). Different  *
;		    frequencies will be used to determine between player 1 (38kHz)  *
;		    and 2 (56kHz) for scoring. Gun will use 12V (with solenoid), or *
;		    5V (without solenoid), can also use 3.3V if using the LF PIC,   *
;		    with the adjustment of some resistors for the solenoid current. *
;										    *
;		    Uses CCP1 (RC2) for laser PWM, RC3 for solenoid control. RB0 for*
;		    trigger, RB1 for frequency select and RB3:2 used to select from *
;		    the firing modes.						    *
;										    *
;************************************************************************************

;************************************************************************************
;										    *
;   Revision History:								    *
;										    *
;   1:	  Got everything for the gun working the way that I think it should with    *
;	  base features.
;										    *	
;************************************************************************************		
		
		#include "p16lf1788.inc"      		; processor specific variable definitions
		#INCLUDE <16LF1788_SETUP.inc>		; Custom setup file for the PIC16F883 micro-controller
		#INCLUDE <SUBROUTINES.inc>		; File containing all used subroutines
		LIST      p=16lf1788		  	; list directive to define processor
		errorlevel -302,-207,-305,-206,-203	; suppress "not in bank 0" message,  Found label after column 1,
							; Using default destination of 1 (file),  Found call to macro in column 1

		; CONFIG1
; __config 0xC9A4
 __CONFIG _CONFIG1, _FOSC_INTOSC & _WDTE_OFF & _PWRTE_OFF & _MCLRE_OFF & _CP_OFF & _CPD_OFF & _BOREN_OFF & _CLKOUTEN_OFF & _IESO_OFF & _FCMEN_OFF
; CONFIG2
; __config 0xDFFF
 __CONFIG _CONFIG2, _WRT_OFF & _PLLEN_ON & _STVREN_ON & _BORV_LO & _LPBOR_OFF & _LVP_OFF


		
;******************************************		
;Interrupt Vectors
;******************************************
		ORG	    H'000'			;BEGINNING OF CODE
		GOTO	    SETUP			;
		ORG	    H'004'			;INTERRUPT LOCATION
		GOTO	    INTERRUPT			;INTERRUPT OCCURRED, RUN THROUGH ISR

;******************************************
;SETUP ROUTINE
;******************************************
SETUP
		CALL	    INITIALIZE			;CALL SETUP INCLUDE FILE TO INITIALIZE PIC
		GOTO	    MAIN			;START MAIN CODE
		
;******************************************
;INTERRUPT SERVICE ROUTINE
;******************************************
INTERRUPT	
		BANKSEL	    PORTB		
		BTFSC	    PORTB,1			;TEST FREQUENCY SELECT SWITCH
		GOTO	    SET_56KHZ			;RB1 IS HIGH, SET PWM FREQUENCY TO 56kHz
		GOTO	    SET_38KHZ			;RB1 IS LOW, SET PWM FREQUENCY TO 38kHz
		
	TEST_MODE	
		BANKSEL	    PORTB		
		BTFSC	    PORTB,2			;DETERMINE IF IN CONTINUOUS/FULL-AUTO MODE
		GOTO	    FIRE_CONTINUOUS		;GUN IS IN CONTINOUS/FULL-AUTO MODE, CHECK IF READY TO FIRE
		BTFSC	    PORTB,3			;GUN IS NOT IN CONTINUOUS/FULL-AUTO MODE, TEST IF IT IS IN BURST-FIRE MODE
		GOTO	    FIRE_BURST			;GUN IS IN BURST-FIRE MODE, CHECK IF READY TO FIRE
		GOTO	    FIRE_SEMI			;GUN IS NOT IN CONTINUOUS OR BURST-FIRE MODE, DEFAULT TO SEMI-AUTO MODE, TEST IF READY TO FIRE
	GOBACK
		BANKSEL	    IOCBF
		CLRF	    IOCBF			;CLEAR PORTB IOC FLAGS
		BCF	    INTCON,IOCIF		;CLEAR IOCIF
		BANKSEL	    PIR1
		BCF	    PIR1,0			;CLEAR TMR1IF
		RETFIE					;RETURN TO MAIN, RE-ENABLE GIE
		
;******************************************
;Main Code
;******************************************
MAIN	
		GOTO	    MAIN			;ENDLESS LOOP, NOTHING HAPPENS (WAITING FOR INTERRUPT)
END
		
;******************************************		
;END PROGRAM DIRECTIVE
;******************************************