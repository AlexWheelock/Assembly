;************************************************************************************
;										    *
;   Filename:	    16F883_SETUP.inc						    *
;   Date:	    October 24, 2024						    *
;   File Version:   1								    *
;   Author:	    Alex Wheelock						    *
;   Company:	    Idaho State University					    *
;   Description:    Firmware for setting up a PIC16F883	for Lab 10, to read/write to*
;		    the PIC EEPROM.
;										    *
;************************************************************************************

;************************************************************************************
;										    *
;   Revision History:								    *
;										    *
;   1:	Initialize file for setting up the PIC16F883 micro-controller for Lab 10 to *
;	Read/Write to EEPROM of the PIC, using a keypad as an input, and a	    *
;	Dot-matrix to display the read/written character on.			    *
;										    *
;************************************************************************************
		
;===============================================================================================================================*
;							   PORTA BITMAP								*
;===============================================================================================================================*		
;		|		|		|		|		|		|		|		*
;	7	|	6	|	5	|	4	|	3	|	2	|	1	|	0	*
;---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------*	
;DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	*
;INPUT		|INPUT		|INPUT		|INPUT		|INPUT		|INPUT		|INPUT		|INPUT		*
;---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------*		
;EXT		|EXT		|UNUSED		|UNUSED		|UNUSED		|USED FOR 	|USED FOR	|USED FOR	*
;OSCILLATOR	|OSCILLATOR	|		|		|		|THE STOP 	|THE PLAY	|THE RECORD	*
;IN		|IN		|		|		|		|BUTTON INPUT	|BUTTON INPUT	|BUTTON INPUT	*
;		|		|		|		|		|		|		|		*		
;-------------------------------------------------------------------------------------------------------------------------------*
		
;===============================================================================================================================*
;							   PORTB BITMAP								*
;===============================================================================================================================*		
;		|		|		|		|		|		|		|		*
;	7	|	6	|	5	|	4	|	3	|	2	|	1	|	0	*
;---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------*	
;DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	*
;INPUT		|INPUT		|INPUT		|INPUT		|OUTPUT		|OUTPUT		|INPUT		|INPUT		*
;---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------*		
;KEYPAD INPUT	|KEYPAD INPUT	|KEYPAD INPUT	|KEYPAD INPUT	|KEYPAD OUTPUT	|KEYPAD OUTPUT	|KEYPAD OUTPUT	|KEYPAD OUTPUT	*
;(LAST COLUMN)	|(THIRD COLUMN)	|(SECOND COLUMN)|(FIRST	COLUMN)	|(BOTTOM ROW)	|(THIRD ROW)	|(SECOND ROW)	|(FIRST ROW)	*
;		|		|		|		|		|		|		|		*
;		|		|		|		|		|		|		|		*		
;-------------------------------------------------------------------------------------------------------------------------------*
		
;===============================================================================================================================*
;							   PORTC BITMAP								*
;===============================================================================================================================*		
;		|		|		|		|		|		|		|		*
;	7	|	6	|	5	|	4	|	3	|	2	|	1	|	0	*
;---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------*	
;DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	*
;OUTPUT		|OUTPUT		|OUTPUT		|OUTPUT		|OUTPUT		|OUTPUT		|OUTPUT		|OUTPUT		*
;---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------*		
;UNUSED		|CONNECTED TO 	|CONNECTED TO	|CONNECTED TO	|CONNECTED TO	|CONNECTED TO	|CONNECTED TO	|CONNECTED TO	*
;		|DOT-MATRIX	|DOT-MATRIX	|DOT-MATRIX	|DOT-MATRIX	|DOT-MATRIX	|DOT-MATRIX	|DOT-MATRIX	*
;		|DISPLAY	|DISPLAY	|DISPLAY	|DISPLAY	|DISPLAY	|DISPLAY	|DISPLAY	*
;		|(PIN 14)	|(PIN 13)	|(PIN 12)	|(PIN 11)	|(PIN 10)	|(PIN 9)	|(PIN 8)	*		
;-------------------------------------------------------------------------------------------------------------------------------*			
		
INITIALIZE_CODE	CODE		
INITIALIZE		
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
		BSF		INTCON, RBIE		;ENABLE PORT B CHANGE INTERUPT 
		BCF		INTCON, INTE		;DISABLE RB0/INT EXTERNAL INTERUPT
		BCF		INTCON, TMR0IE		;DISABLE TMR0 INTERUPT (ENABLED IN MAIN PROGRAM)
		BSF		INTCON, PEIE		;ENABLE PERIPHERAL INTERUPTS
		BSF		INTCON, GIE		;ENABLE ALL UNMASKED INTERUPTS
		
;*** SET PIE1 REG: *****
		
		BANKSEL PIE1
		BCF		PIE1, TMR1IE		;DISABLE TMR1 INTERUPT
		BSF		PIE1, TMR2IE		;ENABLE TMR2 INTERUPT POR PWM
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
		
		BANKSEL T1CON				;/DISABLE TMR1
		CLRF	T1CON				;\
		
;*** TIMER 2 SETUP *****

		BANKSEL T2CON				;/
		MOVLW	0x12				;Enable Timer 2, set pre-scale to 1:16, post-scale to 1:3
		MOVWF	T2CON				;\
		BANKSEL	PR2				;/
		MOVLW	0xD0				;Set PR2 to 208 for 10ms interrupt intervals
		MOVWF	PR2				;\
 
;*** PORT A SETUP **** PORT A SET AS LOGIC OUTPUT (NOT USED)

		BANKSEL PORTA
		CLRF	PORTA				;SET PORTA TO 00h
		BANKSEL	TRISA
		MOVLW	H'FF'				;/SET PORTA ALL AS INPUT
		MOVWF	TRISA				;\
		BANKSEL	ANSEL				;/SET PORTA ALL AS DIGITAL INPUT
		CLRF	ANSEL				;\
		
;*** PORT B SETUP **** PORT B USED AS LOGIC INPUT
		
		BANKSEL PORTB
		CLRF	PORTB				;INIT PORT B
		BANKSEL	TRISB
		MOVLW	H'F0'				;/SET PORTB RB7:4 AS INPUTS, RB3:1 AS OUTPUTS
		MOVWF	TRISB				;\
		BANKSEL ANSELH
		MOVLW	H'00'				;/DISABLE ANALOG INPUT FOR PORTB
		MOVWF	ANSELH				;\
		BANKSEL	IOCB
		MOVLW	H'00'				;/DISABLES INTERRUPT ON CHANGE FOR RB7:0
		MOVWF	IOCB				;\
		
;*** PORT C SETUP **** PORT C USED AS LOGIC OUTPUT TO DRIVE DOT MATRIX
		
		BANKSEL PORTC
		MOVLW	0x53				;/SET DISPLAY TO S UPON STARTUP
		MOVWF	PORTC				;\
		BANKSEL	TRISC
		MOVLW	0x00				;/
		MOVWF	TRISC				;\SET PORTC ALL AS OUTPUT
		
		BANKSEL	T2CON
		CLRF	CURRENT_REGISTER		;SET DEFAULT REGISTER TO 0
		BCF	TEST_INPUTS,0			;CLEAR TEST_INPUTS
		BSF	IDLE,0				;SET IDLE
		MOVLW	0x64				;/SET IDLE_COUNT FOR 1 SECOND PULSES
		MOVWF	IDLE_COUNT			;\    
		BCF	RECORD_MODE,0			;CLEAR RECORD_MODE
		BCF	PLAY_MODE,0			;CLEAR PLAY_MODE
		BSF	T2CON,2				;\Turn Timer 2 on
		
		
		RETURN					;Return to .asm file