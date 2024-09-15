	;DelayUsingSubroutine.asm
	;September 13, 2024
	;Alex Wheelock
	;This program switches between 0 and 5 on the output dot-matrix with a 0.5s delay. The output should be 1Hz as it flips its bits to change the output.
	;The delay was done using loops and understanding the number of Q-states that it takes for each command, to calculate the 0.5s delay within +-1us.
	;This runs it the delay in a subroutine rather than doing it in MAIN.

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
		MOVLW	H'FF'				;SET PORT A AS INPUT
		MOVWF	TRISA
		
;*** PORT B SETUP **** PORT B USED AS LOGIC INPUT
		
		BANKSEL PORTB
		CLRF	PORTB				;INIT PORT B
		BANKSEL	TRISB
		MOVLW	H'FF'				;SET PORT B AS INPUT
		MOVWF	TRISB
		BANKSEL ANSELH
		MOVLW	H'00'				;DISABLES ANALOG INPUT TO ENSURE THAT IT IS A DIGITAL INPUT
		MOVWF	ANSELH
		
;*** PORT C SETUP **** PORT C USED AS LOGIC OUTPUT TO DRIVE DOT MATRIX
		
		BANKSEL PORTC
		CLRF PORTC				;INIT PORTC
		BANKSEL	TRISC
		MOVLW	H'00'
		MOVWF	TRISC				;SET PORT C AS OUTPUT
		
		GOTO	MAIN				;END OF SETUP ROUTINE

		
;******************************************
;Subroutines
;******************************************

DELAY
		MOVLW	0xF3				;244
		MOVWF	COUNT2				;Set count of the outermost loop of the nested loop to 244
    LOOP2		    				;Outermost loop of nested loop start
		MOVLW	0X00				;0 decrements initially to 255 = 256 iterations
		MOVWF	COUNT1				;Set count of the innermost loop to 256
    LOOP1		    				;Innermost loop of nested loop start
		NOP					;/
		NOP					;
		NOP					;No operation, wasting time to get the correct timing
		NOP					;
		NOP					;\
		DECFSZ	COUNT1				;Decrement innermost loop count, skip GOTO to decrement outer loop
		GOTO	LOOP1				;COUNT1 did not equal 0, continuing through the loop
		DECFSZ	COUNT2				;Decrement outer loop, skip restarting through the loop if COUNT2 = 0 
		GOTO	LOOP2				;Restart through outer loop, and reset the innermost loop
		MOVLW	0XE1				;225
		MOVWF	COUNT3				;Set the simple loop to do 225 iterations
    LOOP3
		NOP					;/
		NOP					;No operation, wasting time to get the correct timing
		NOP					;\
		DECFSZ	COUNT3
		GOTO	LOOP3
		NOP					;/
		NOP					;
		NOP					;
		NOP					;
		NOP					;
		NOP					;
		NOP					;
		NOP					;No operation, additional time waste to get correct timing out
		NOP					;
		NOP					;
		NOP					;
		NOP					;
		NOP					;
		NOP					;
		NOP					;\
		RETURN					;Return to the original place of the call
		
;******************************************
;Main Code
;******************************************
MAIN	
		BANKSEL	PORTC
		CALL	DELAY				;Call the DELAY subroutine to delay in order to get the correct frequency on the output
		BTFSS	PORTC,0				;Test bit 0 of PORTC output, if its high the output is 5, if true set to zero
		GOTO	SET5				;Output is displaying 0, set output to 5
		NOP					;Additional NOP to make timing equal regardless of what it sets the output to
		MOVLW	0X30				;ASCII 0
		MOVWF	PORTC				;set PORTC to display 0
		GOTO	MAIN				;Restart back to MAIN
    SET5
		MOVLW	0X35				;ASCII 5
		MOVWF	PORTC				;Set PORTC to display 5
		GOTO	MAIN				;Restart back to MAIN
END
		
;******************************************		
;END PROGRAM DIRECTIVE
;******************************************