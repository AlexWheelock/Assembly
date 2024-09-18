	;FunkyMuzak.asm
	;September 18, 2024
	;Alex Wheelock
	;This code is used to test a 4x4 keypad input on PORTB, to change the frequency on the output RA0 that goes to a class D amplifier.
	;Each key used will play a different note on the output, so that you can play "funky muzak" on a speaker with the keypad.
	
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
		MOVLW	H'00'				;DISABLE PWM & CCP
		MOVWF	CCP1CON

;*** TIMER 1 SETUP *****
		
		BANKSEL T1CON
		CLRF T1CON				;DISABLE TIMER 1

;*** TIMER 2 SETUP *****

		BANKSEL T2CON
		CLRF	T2CON				;DISABLE TIMER 2
 
;*** PORT A SETUP **** PORT A SET AS LOGIC OUTPUT (NOT USED)

		BANKSEL PORTA
		CLRF	PORTA				;INIT PORT A
		BANKSEL	TRISA
		MOVLW	H'FE'				;/SET RA0 AS OUTPUT, RA7:1 INPUTS
		MOVWF	TRISA				;\
		BANKSEL	ANSEL
		MOVLW	0X00				;/
		MOVWF	ANSEL				;\CLEAR ANSEL TO MAKE PORTA DIGITAL I/O
		
;*** PORT B SETUP **** PORT B USED AS LOGIC INPUT
		
		BANKSEL PORTB
		CLRF	PORTB				;INIT PORT B
		BANKSEL	TRISB
		MOVLW	H'F0'				;SET PORT B AS INPUT
		MOVWF	TRISB
		BANKSEL ANSELH
		MOVLW	H'00'				;DISABLES ANALOG INPUT TO ENSURE THAT IT IS A DIGITAL INPUT
		MOVWF	ANSELH
		
;*** PORT C SETUP **** PORT C USED AS LOGIC OUTPUT TO DRIVE DOT MATRIX
		
		BANKSEL PORTC	
		MOVLW	0x00
		MOVWF	PORTC				;INIT PORTC
		BANKSEL	TRISC
		MOVLW	0x00
		MOVWF	TRISC				;SET PORT C AS OUTPUT
		
		GOTO	MAIN				;END OF SETUP ROUTINE
		

;******************************************
;Subroutines
;******************************************	
DELAY	
		MOVWF	COUNT1
	LOOP
		NOP
		NOP
		DECFSZ	COUNT1				;DECRIMENT COUNT1, CONTINUE LOOP IF NOT ZERO, STEP OUT OF LOOP
		GOTO	LOOP				;COUNT1 > 0, CONTINUE LOOP
		BTFSS	PORTA,0				;TEST RA0, TO SET TOGGLE THE OUTPUT
		GOTO	SETRA0				;RA0 LOW, SET HIGH
		NOP
		BCF	PORTA,0				;RA0 HIGH, SET RA0 LOW
		RETURN				
	SETRA0
		BSF	PORTA,0				;RA0 LOW, SET RA0 HIGH
		RETURN
							
		
;******************************************
;Main Code
;******************************************
MAIN	
		BANKSEL	PORTA
	TESTINPUTS
		MOVLW	H'08'				;
		MOVWF	PORTB				;SET RB3, TEST ROW 4
		BTFSC	PORTB,7				;/
		GOTO	KEYF				;
		BTFSC	PORTB,6				;
		GOTO	KEYE				;UNUSED BUTTONS, RESTART TESTING INPUTS IF ANY ARE PRESSED
		BTFSC	PORTB,5				;
		GOTO	KEYD				;
		BTFSC	PORTB,4				;
		GOTO	KEYC				;\
    
		MOVLW	H'04'
		MOVWF	PORTB				;SET RB2, TEST ROW 3
		BTFSC	PORTB,7				;/
		GOTO	KEYB				;
		BTFSC	PORTB,6				;
		GOTO	KEYA				;UNUSED BUTTONS, RESTART TESTING INPUTS IF ANY ARE PRESSED
		BTFSC	PORTB,5				;
		GOTO	KEY9				;
		BTFSC	PORTB,4				;
		GOTO	KEY8				;\

		MOVLW	H'02'
		MOVWF	PORTB				;SET RB1, TEST ROW 2
	    	BTFSC	PORTB,7				;/
		GOTO	KEY7				;
		BTFSC	PORTB,6				;
		GOTO	KEY6				;UNUSED BUTTONS, RESTART TESTING INPUTS IF ANY ARE PRESSED
		BTFSC	PORTB,5				;
		GOTO	KEY5				;
		BTFSC	PORTB,4				;
		GOTO	KEY4				;\

		MOVLW	H'01'
		MOVWF	PORTB				;SET RB0, TEST ROW 1
		BTFSC	PORTB,7				;/
		GOTO	KEY3				;
		BTFSC	PORTB,6				;
		GOTO	KEY2				;UNUSED BUTTONS, RESTART TESTING INPUTS IF ANY ARE PRESSED
		BTFSC	PORTB,5				;
		GOTO	KEY1				;
		BTFSC	PORTB,4				;\
		GOTO	KEY0				;RB4 TRUE, ENABLE 1KHZ SQUARE WAVE TO THE CLASS D AMP
		GOTO	TESTINPUTS				;NO BUTTON IS PRESSED, RESTART INPUT TESETING
	KEY0
		MOVLW	0X96				;150
		MOVWF	COUNT1				;SET LOOP ITERATIONS TO 150
		GOTO	TESTINPUTS
	KEY1
	
		GOTO	TESTINPUTS	
	KEY2

		GOTO	TESTINPUTS	
	KEY3
	
		GOTO	TESTINPUTS	
	KEY4
	
		GOTO	TESTINPUTS	
	KEY5
	
		GOTO	TESTINPUTS
	KEY6
	
		GOTO	TESTINPUTS	
	KEY7
	
		GOTO	TESTINPUTS	
	KEY8
	
		GOTO	TESTINPUTS	
	KEY9
	
		GOTO	TESTINPUTS	
	KEYA
	
		GOTO	TESTINPUTS	
	KEYB
	
		GOTO	TESTINPUTS	
	KEYC
	
		GOTO	TESTINPUTS	
	KEYD
	
		GOTO	TESTINPUTS	
	KEYE
	
		GOTO	TESTINPUTS	
	KEYF
		NOP
		NOP
		NOP
		NOP
		MOVLW	0X00
		CALL	TABLE
		CALL	DELAY
		GOTO	TESTINPUTS
		
;******************************************
;Table Code
;******************************************		
TABLE
		ADDWF	PCL
		RETLW	0XA5
		RETLW	0XD4
		
		
END
		
;******************************************		
;END PROGRAM DIRECTIVE
;******************************************