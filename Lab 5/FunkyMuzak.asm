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
;Table Code
;******************************************		
		ORG	H'00A'					;RE-ORGED THE MEMORY TO SOLVE A PROBLEM OF THE PCL JUMPING BACK INTO SETUP
TABLE
		ADDWF	PCL					;ADD W TO THE PCL TO GIVE IT THE OFFSET TO RETURN WITH THE CORRECT COUNT FOR THE DELAY
		RETLW	0XA5					;/
		RETLW	0XB9					;
		RETLW	0XC4					;
		RETLW	0XDC					;
		RETLW	0XF8					;
		RETLW	0X8B					;
		RETLW	0X93					;SET THE ITERATIONS FOR THE DELAYS TO ACHIEVE THE CORRECT NOTES
		RETLW	0XC5					;SOME USED THE DELAY TWICE FOR THE LOWER FREQUENCIES REQUIRING GREATER DELAYS
		RETLW	0XDD					;
		RETLW	0XA6					;
		RETLW	0XBA					;
		RETLW	0XC6					;
		RETLW	0XDE					;
		RETLW	0XFA					;\
		
;******************************************
;SETUP ROUTINE
;******************************************
		ORG	H'020'
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
		MOVWF	COUNT1				;LOAD W FROM THE TABLE INTO COUNT1 TO SET THE ITERATIONS
	LOOP
		NOP					;/DELAY TO ACHIEVE THE CORRECT MAX FREQUENCY
		NOP					;\
		DECFSZ	COUNT1				;DECRIMENT COUNT1, CONTINUE LOOP IF NOT ZERO, STEP OUT OF LOOP
		GOTO	LOOP				;COUNT1 > 0, CONTINUE LOOP
		RETURN					;RETRUN TO THE CALL LOCATION
SETOUTPUT
		BTFSS	PORTA,0				;TEST RA0, TO SET TOGGLE THE OUTPUT
		GOTO	SETRA0				;RA0 LOW, SET HIGH
		NOP					;NOP MAKES PULSE WIDTH AND PULSE SPACE EQUAL
		BCF	PORTA,0				;RA0 HIGH, SET RA0 LOW
		RETURN					;RETURN TO CALL LOCATION
	SETRA0
		BSF	PORTA,0				;RA0 LOW, SET RA0 HIGH
		RETURN					;RETURN TO CALL LOCATION
							
		
;******************************************
;Main Code
;******************************************
MAIN	
		BANKSEL	PORTA
	TESTINPUTS
		MOVLW	H'08'				;
		MOVWF	PORTB				;SET RB3, TEST ROW 4
		BTFSC	PORTB,7				;TEST THE F KEY
		GOTO	KEYF				;F KEY PRESSED, SET DELAY FOR CORRESPONDING NOTE
		BTFSC	PORTB,6				;TEST THE E KEY
		GOTO	KEYE				;E KEY PRESSED, SET DELAY FOR CORRESPONDING NOTE
		BTFSC	PORTB,5				;TEST THE D KEY
		GOTO	KEYD				;D KEY PRESSED, SET DELAY FOR CORRESPONDING NOTE
		BTFSC	PORTB,4				;TEST THE C KEY
		GOTO	KEYC				;C KEY PRESSED, SET DELAY FOR CORRESPONDING NOTE
    
		MOVLW	H'04'
		MOVWF	PORTB				;SET RB2, TEST ROW 3
		BTFSC	PORTB,7				;TEST THE B KEY
		GOTO	KEYB				;B KEY PRESSED, SET DELAY FOR CORRESPONDING NOTE
		BTFSC	PORTB,6				;TEST THE A KEY
		GOTO	KEYA				;A KEY PRESSED, SET DELAY FOR CORRESPONDING NOTE
		BTFSC	PORTB,5				;TEST THE 9 KEY
		GOTO	KEY9				;9 KEY PRESSED, SET DELAY FOR CORRESPONDING NOTE
		BTFSC	PORTB,4				;TEST THE 8 KEY
		GOTO	KEY8				;8 KEY PRESSED, SET DELAY FOR CORRESPONDING NOTE

		MOVLW	H'02'
		MOVWF	PORTB				;SET RB1, TEST ROW 2
	    	BTFSC	PORTB,7				;TEST THE 7 KEY
		GOTO	KEY7				;7 KEY PRESSED, SET DELAY FOR CORRESPONDING NOTE
		BTFSC	PORTB,6				;TEST THE 6 KEY
		GOTO	KEY6				;6 KEY PRESSED, SET DELAY FOR CORRESPONDING NOTE
		BTFSC	PORTB,5				;TEST THE 5 KEY
		GOTO	KEY5				;5 KEY PRESSED, SET DELAY FOR CORRESPONDING NOTE
		BTFSC	PORTB,4				;TEST THE 4 KEY
		GOTO	KEY4				;4 KEY PRESSED, SET DELAY FOR CORRESPONDING NOTE

		MOVLW	H'01'
		MOVWF	PORTB				;SET RB0, TEST ROW 1
		BTFSC	PORTB,7				;TEST THE 3 KEY
		GOTO	KEY3				;3 KEY PRESSED, SET DELAY FOR CORRESPONDING NOTE
		BTFSC	PORTB,6				;TEST THE 2 KEY
		GOTO	KEY2				;2 KEY PRESSED, SET DELAY FOR CORRESPONDING NOTE
		BTFSC	PORTB,5				;TEST THE 1 KEY
		GOTO	KEY1				;1 KEY PRESSED, SET DELAY FOR CORRESPONDING NOTE
		BTFSC	PORTB,4				;TEST THE 0 KEY
		GOTO	KEY0				;0 KEY PRESSED, SET DELAY FOR CORRESPONDING NOTE
		GOTO	TESTINPUTS			;NO BUTTON IS PRESSED, RESTART INPUT TESETING
	KEY0
		MOVLW	0X0D				;SET THE OFFSET REQUIRED TO RETURN THE CORRECT COUNT FOR THE CORRECT NOTE
		CALL	TABLE				;
		CALL	DELAY				;/
		CALL	DELAY				;CALL THE DELAY THE NUMBER OF TIMES NECESSARY TO GET THE PROPER FREQUENCY FOR THE CORRECT NOTE
		CALL	DELAY				;\
		CALL	SETOUTPUT			;TOGGLE THE OUTPUT			
		GOTO	TESTINPUTS			;TEST ALL OF THE INPUTS AGAIN
	KEY1
		MOVLW	0X0C				;SET THE OFFSET REQUIRED TO RETURN THE CORRECT COUNT FOR THE CORRECT NOTE
		CALL	TABLE				;
		CALL	DELAY				;/
		CALL	DELAY				;CALL THE DELAY THE NUMBER OF TIMES NECESSARY TO GET THE PROPER FREQUENCY FOR THE CORRECT NOTE
		CALL	DELAY				;\
		NOP					;/
		NOP					;
		NOP					;INLINE NOPS TO DIAL IN THE FREQUENCY TO THE NOTE
		NOP					;
		NOP					;
		NOP					;\
		CALL	SETOUTPUT			;TOGGLE THE OUTPUT
		GOTO	TESTINPUTS			;TEST ALL OF THE INPUTS AGAIN			
	KEY2
		MOVLW	0X0B				;SET THE OFFSET REQUIRED TO RETURN THE CORRECT COUNT FOR THE CORRECT NOTE
		CALL	TABLE
		CALL	DELAY				;/
		CALL	DELAY				;CALL THE DELAY THE NUMBER OF TIMES NECESSARY TO GET THE PROPER FREQUENCY FOR THE CORRECT NOTE
		CALL	DELAY				;\
		CALL	SETOUTPUT			;TOGGLE THE OUTPUT
		GOTO	TESTINPUTS			;TEST ALL OF THE INPUTS AGAIN	
	KEY3
		MOVLW	0X0A				;SET THE OFFSET REQUIRED TO RETURN THE CORRECT COUNT FOR THE CORRECT NOTE
		CALL	TABLE
		CALL	DELAY				;/
		CALL	DELAY				;CALL THE DELAY THE NUMBER OF TIMES NECESSARY TO GET THE PROPER FREQUENCY FOR THE CORRECT NOTE
		CALL	DELAY				;\
		NOP					;/
		NOP					;
		NOP					;
		NOP					;
		NOP					;INLINE NOPS TO DIAL IN THE FREQUENCY TO THE NOTE
		NOP					;
		NOP					;
		NOP					;
		NOP					;
		NOP					;\
		CALL	SETOUTPUT			;TOGGLE THE OUTPUT
		GOTO	TESTINPUTS			;TEST ALL OF THE INPUTS AGAIN	
	KEY4
		MOVLW	0X09				;SET THE OFFSET REQUIRED TO RETURN THE CORRECT COUNT FOR THE CORRECT NOTE
		CALL	TABLE
		CALL	DELAY				;/
		CALL	DELAY				;CALL THE DELAY THE NUMBER OF TIMES NECESSARY TO GET THE PROPER FREQUENCY FOR THE CORRECT NOTE
		CALL	DELAY				;\
		CALL	SETOUTPUT			;TOGGLE THE OUTPUT
		GOTO	TESTINPUTS			;TEST ALL OF THE INPUTS AGAIN	
	KEY5
		MOVLW	0X08				;SET THE OFFSET REQUIRED TO RETURN THE CORRECT COUNT FOR THE CORRECT NOTE
		CALL	TABLE
		CALL	DELAY				;/CALL THE DELAY THE NUMBER OF TIMES NECESSARY TO GET THE PROPER FREQUENCY FOR THE CORRECT NOTE
		CALL	DELAY				;\
		NOP					;/
		NOP					;
		NOP					;
		NOP					;INLINE NOPS TO DIAL IN THE FREQUENCY TO THE NOTE
		NOP					;
		NOP					;
		NOP					;
		NOP					;\
		CALL	SETOUTPUT			;TOGGLE THE OUTPUT		
		GOTO	TESTINPUTS			;TEST ALL OF THE INPUTS AGAIN
	KEY6
		MOVLW	0X07				;SET THE OFFSET REQUIRED TO RETURN THE CORRECT COUNT FOR THE CORRECT NOTE
		CALL	TABLE
		CALL	DELAY				;/CALL THE DELAY THE NUMBER OF TIMES NECESSARY TO GET THE PROPER FREQUENCY FOR THE CORRECT NOTE
		CALL	DELAY				;\    
		NOP					;/INLINE NOPS TO DIAL IN THE FREQUENCY TO THE NOTE
		NOP					;\
		CALL	SETOUTPUT			;TOGGLE THE OUTPUT	
		GOTO	TESTINPUTS			;TEST ALL OF THE INPUTS AGAIN	
	KEY7
		MOVLW	0X01				;SET THE OFFSET REQUIRED TO RETURN THE CORRECT COUNT FOR THE CORRECT NOTE
		CALL	TABLE
		CALL	DELAY				;/CALL THE DELAY THE NUMBER OF TIMES NECESSARY TO GET THE PROPER FREQUENCY FOR THE CORRECT NOTE
		CALL	DELAY				;\
		NOP					;/
		NOP					;
		NOP					;
		NOP					;
		NOP					;
		NOP					;INLINE NOPS TO DIAL IN THE FREQUENCY TO THE NOTE
		NOP					;
		NOP					;
		NOP					;
		NOP					;
		NOP					;\
		CALL	SETOUTPUT			;TOGGLE THE OUTPUT
		GOTO	TESTINPUTS			;TEST ALL OF THE INPUTS AGAIN	
	KEY8
		MOVLW	0X00				;SET THE OFFSET REQUIRED TO RETURN THE CORRECT COUNT FOR THE CORRECT NOTE
		CALL	TABLE
		CALL	DELAY				;/CALL THE DELAY THE NUMBER OF TIMES NECESSARY TO GET THE PROPER FREQUENCY FOR THE CORRECT NOTE
		CALL	DELAY				;\
		NOP					;/
		NOP					;
		NOP					;INLINE NOPS TO DIAL IN THE FREQUENCY TO THE NOTE
		NOP					;
		NOP					;
		NOP					;\
		CALL	SETOUTPUT			;TOGGLE THE OUTPUT
		GOTO	TESTINPUTS			;TEST ALL OF THE INPUTS AGAIN	
	KEY9
		MOVLW	0X06				;SET THE OFFSET REQUIRED TO RETURN THE CORRECT COUNT FOR THE CORRECT NOTE
		CALL	TABLE
		CALL	DELAY				;/CALL THE DELAY THE NUMBER OF TIMES NECESSARY TO GET THE PROPER FREQUENCY FOR THE CORRECT NOTE
		CALL	DELAY				;\
		NOP					;/INLINE NOPS TO DIAL IN THE FREQUENCY TO THE NOTE
		NOP					;\
		CALL	SETOUTPUT			;TOGGLE THE OUTPUT
		GOTO	TESTINPUTS	
	KEYA
		MOVLW	0X05				;SET THE OFFSET REQUIRED TO RETURN THE CORRECT COUNT FOR THE CORRECT NOTE
		CALL	TABLE
		CALL	DELAY				;/CALL THE DELAY THE NUMBER OF TIMES NECESSARY TO GET THE PROPER FREQUENCY FOR THE CORRECT NOTE
		CALL	DELAY				;\
		CALL	SETOUTPUT			;TOGGLE THE OUTPUT
		GOTO	TESTINPUTS			;TEST ALL OF THE INPUTS AGAIN	
	KEYB
		MOVLW	0X04				;SET THE OFFSET REQUIRED TO RETURN THE CORRECT COUNT FOR THE CORRECT NOTE
		CALL	TABLE
		CALL	DELAY				;CALL THE DELAY THE NUMBER OF TIMES NECESSARY TO GET THE PROPER FREQUENCY FOR THE CORRECT NOTE
		CALL	SETOUTPUT			;TOGGLE THE OUTPUT
		GOTO	TESTINPUTS			;TEST ALL OF THE INPUTS AGAIN	
	KEYC
		MOVLW	0X03				;SET THE OFFSET REQUIRED TO RETURN THE CORRECT COUNT FOR THE CORRECT NOTE
		CALL	TABLE
		CALL	DELAY				;CALL THE DELAY THE NUMBER OF TIMES NECESSARY TO GET THE PROPER FREQUENCY FOR THE CORRECT NOTE
		NOP					;/
		NOP					;INLINE NOPS TO DIAL IN THE FREQUENCY TO THE NOTE
		NOP					;
		NOP					;\
		CALL	SETOUTPUT			;TOGGLE THE OUTPUT
		GOTO	TESTINPUTS			;TEST ALL OF THE INPUTS AGAIN	
	KEYD
		MOVLW	0X02				;SET THE OFFSET REQUIRED TO RETURN THE CORRECT COUNT FOR THE CORRECT NOTE
		CALL	TABLE
		CALL	DELAY				;CALL THE DELAY THE NUMBER OF TIMES NECESSARY TO GET THE PROPER FREQUENCY FOR THE CORRECT NOTE
		NOP					;/
		NOP					;INLINE NOPS TO DIAL IN THE FREQUENCY TO THE NOTE
		NOP					;\
		CALL	SETOUTPUT			;TOGGLE THE OUTPUT
		GOTO	TESTINPUTS			;TEST ALL OF THE INPUTS AGAIN	
	KEYE
		MOVLW	0X01				;SET THE OFFSET REQUIRED TO RETURN THE CORRECT COUNT FOR THE CORRECT NOTE
		CALL	TABLE
		CALL	DELAY				;CALL THE DELAY THE NUMBER OF TIMES NECESSARY TO GET THE PROPER FREQUENCY FOR THE CORRECT NOTE
		NOP					;/
		NOP					;INLINE NOPS TO DIAL IN THE FREQUENCY TO THE NOTE
		NOP					;\
		CALL	SETOUTPUT			;TOGGLE THE OUTPUT	
		GOTO	TESTINPUTS			;TEST ALL OF THE INPUTS AGAIN	
	KEYF
		MOVLW	0X00				;SET THE OFFSET REQUIRED TO RETURN THE CORRECT COUNT FOR THE CORRECT NOTE
		CALL	TABLE
		CALL	DELAY				;CALL THE DELAY THE NUMBER OF TIMES NECESSARY TO GET THE PROPER FREQUENCY FOR THE CORRECT NOTE
		CALL	SETOUTPUT			;TOGGLE THE OUTPUT
		GOTO	TESTINPUTS			;TEST ALL OF THE INPUTS AGAIN
		

END
		
;******************************************		
;END PROGRAM DIRECTIVE
;******************************************