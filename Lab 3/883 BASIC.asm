	;873ABasic.asm
	;September 5, 2024
	;Alex Wheelock
	;Program that uses 8 inputs from a DIP switch, that displays the corresponding number on the input onto a dot matrix display.
	;If no switch is pressed, display a 0. If port B1 is pressed then display a 1 and so on. Have the highest number have priority.
	;Repeat this in an endless loop. Use port C for the display output and port B for the DIP switch inputs.

		#INCLUDE <p16f883.inc>        		; processor specific variable definitions
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
;Interupt Vectors
;******************************************
		ORG H'000'					
		GOTO SETUP					;RESET CONDITION GOTO SETUP

;******************************************
;SETUP ROUTINE
;******************************************
SETUP
;*** SFR SETUP **********
 
		
;*** SET OPTION_REG: ****
		BANKSEL OPTION_REG
		BCF		OPTION_REG, PS0		;\\
		BCF		OPTION_REG, PS1		; >>PRESCALER RATIO SET 1:1
		BCF		OPTION_REG, PS2		;//
		BSF		OPTION_REG, PSA		;PRESCALER ASSIGNED TO WDT 
		BCF		OPTION_REG, T0SE	;TMR0 EDGE SET RISING
		BCF		OPTION_REG, T0CS	;TMR0 CLOCK SOURCE SET TO INTERNAL
		BSF		OPTION_REG, INTEDG	;RB0/INT SET TO RISING EDGE
		BSF		OPTION_REG, 7		;PORTB PULLUP ENABLED
;*** SET INTCON REG: ****
		BANKSEL INTCON
		BCF		INTCON, RBIE		;DISABLE PORT B CHANGE INTERUPT 
		BCF		INTCON, INTE		;DISABLE RB0/INT EXTERNAL INTERUPT
		BCF		INTCON, TMR0IE		;DISABLE TMR0 INTERUPT (ENABLED IN MAIN PROGRAM)
		BSF		INTCON, PEIE		;ENABLE PERIPHERAL INTERUPTS
		BSF		INTCON, GIE		;ENABLE ALL UNMASKED INTERUPTS
;*** SET PIE1 REG: *****
		BANKSEL PIE1
		BCF		PIE1, TMR1IE		;DISABLE TMR1 INTERUPT
		BCF		PIE1, TMR2IE		;DISABLE TMR2 INTERUPT POR PWM
		BCF		PIE1, CCP1IE		;DISABLE CCP1 INTERUPT
		BCF		PIE1, SSPIE		;DISABLE SSP INTERUPT
		BCF		PIE1, TXIE		;DISABLE USART TRANSMIT INTERUPT
		BCF		PIE1, RCIE		;DISABLE USART RECIEVE INTERUPT
		BCF		PIE1, ADIE		;DISABLE A/D INTERUPT
		BCF		PIE1, 7			;DISABLE PSP INTERUPT
;*** SET CCP1CON REG: **
		BANKSEL CCP1CON
		MOVLW	H'000'				;DISABLE PWM & CCP
		MOVWF	CCP1CON

;*** TIMER 1 SETUP *****
		BANKSEL T1CON
		MOVLW	H'000'				;
		MOVWF	T1CON				;DISABLE TIMER 1

;*** TIMER 2 SETUP *****

		BANKSEL T2CON
		CLRF	T2CON				;DISABLE TIMER 2, 1:1 POST SCALE, PRESCALER 1
		MOVLW	H'0FF'				;SET PR2 FOR FULL COUNT 
		BANKSEL	PR2				;
		MOVWF	PR2				;PR2 IS SETS OUTPUT OF PWM HIGH WHEN = TMR2
 
;*** PORT A SETUP **** PORT B RB0 IS USED AS EDGE TRIGGERED INPUT

		BANKSEL	ADCON1
		BCF	ADCON1,3
		BSF	ADCON1,2
		BSF	ADCON1,1
		BANKSEL	TRISA
		MOVLW	H'000'				;SET PORT A AS OUTPUT
		MOVWF	TRISA
		
;*** PORT B SETUP **** PORT B RB0 IS USED AS EDGE TRIGGERED INPUT
		BANKSEL	TRISB
		MOVLW	H'000'				;SET PORT B AS OUTPUT
		MOVWF	TRISB

;*** PORT C SETUP **** PORT B RB0 IS USED AS EDGE TRIGGERED INPUT
		BANKSEL	TRISC
		MOVLW	H'000'				;SET PORT C AS OUTPUT
		MOVWF	TRISC
		GOTO	MAIN				;END OF SETUP ROUTINE

;******************************************
;Main Code
;******************************************
MAIN	
		NOP

		END					
;END PROGRAM DIRECTIVE *******************************
;*************************************************************************************************************