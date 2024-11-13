;************************************************************************************
;										    *
;   Filename:	    16LF1788_SETUP.inc						    *
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
	
;*** INTOSC SETUP *******
	
		BANKSEL	    OSCCON
		MOVLW	    0xEA			;ENABLE INTOSC, SET TO 4MHz
		MOVWF	    OSCCON
		BANKSEL	    CLKRCON
		CLRF	    CLKRCON			;DISABLE CLOCK REFERENCE
	
		
;*** SFR SETUP **********
 
		
;*** SET OPTION_REG: ****
		
		BANKSEL	    OPTION_REG
		BCF	    OPTION_REG, PS0		;\\
		BCF	    OPTION_REG, PS1		; >>PRESCALER RATIO SET 1:1
		BCF	    OPTION_REG, PS2		;//
		BSF	    OPTION_REG, PSA		;PRESCALER ASSIGNED TO WDT 
		BCF	    OPTION_REG, TMR0SE		;TMR0 EDGE SET RISING
		BCF	    OPTION_REG, TMR0CS		;TMR0 CLOCK SOURCE SET TO INTERNAL
		BSF	    OPTION_REG, INTEDG		;RB0/INT SET TO RISING EDGE
		BSF	    OPTION_REG, NOT_WPUEN	;WEAK PULLUP ENABLED
		
;*** SET INTCON REG: ****
		
		BANKSEL INTCON
		BCF	    INTCON, IOCIF		;CLEAR IOC INTERUPT FLAG
		BCF	    INTCON, INTF		;CLEAR INT EXTERNAL INTERUPT
		BCF	    INTCON, TMR0IF		;DISABLE TMR0 INTERUPT (ENABLED IN MAIN PROGRAM)
		BSF	    INTCON, IOCIE		;ENABLE IOC INTERUPT
		BCF	    INTCON, INTE		;DISABLE INT EXTERNAL INTERUPT
		BSF	    INTCON, PEIE		;ENABLE PERIPHERAL INTERUPTS
		BSF	    INTCON, GIE			;ENABLE ALL UNMASKED INTERUPTS
		
;*** SET PIE1-4 REG: *****
		
		BANKSEL	    PIE1
		MOVLW	    0x01
		MOVWF	    PIE1			;DISABLE ALL PIE1 INTERUPTS, EXCEPT TMR1IE
		BANKSEL	    PIE2
		CLRF	    PIE2			;DISABLE ALL PIE2 INTERUPTS
		BANKSEL	    PIE3
		CLRF	    PIE3			;DISABLE ALL PIE3 INTERUPTS
		BANKSEL	    PIE4
		CLRF	    PIE4			;DISABLE ALL PIE4 INTERUPTS
		
;*** CLEAR INTERUPT FLAGS ***
		
		BANKSEL	    PIR1
		CLRF	    PIR1			;CLEAR ALL PIR1 INTERUPT FLAGS
		BANKSEL	    PIR2
		CLRF	    PIR2			;CLEAR ALL PIR2 INTERUPT FLAGS
		BANKSEL	    PIR3
		CLRF	    PIR3			;CLEAR ALL PIR3 INTERUPT FLAGS
		BANKSEL	    PIR4
		CLRF	    PIR4			;CLEAR ALL PIR4 INTERUPT FLAGS
		
;*** SET CCP1CON REG: **
		
		BANKSEL	    TRISC
		BSF	    TRISC,2
		BANKSEL	    CCP1CON
		MOVLW	    0x0C		
		MOVWF	    CCP1CON
		BANKSEL	    CCPR1L
		MOVLW	    0x13
		;MOVLW	    0x0D
		MOVWF	    CCPR1L

;*** TIMER 1 SETUP *****
		
		BANKSEL	    T1CON			
		MOVLW	    0xA8			;ENABLE & SELECT TMR1 LP OSCILLATOR, SET PRESCALE TO 1:4
		BANKSEL	    T1GCON
		CLRF	    T1GCON			;DISABLE TMR1 GATE
		
;*** TIMER 2 SETUP *****

		BANKSEL	    T2CON			
		CLRF	    T2CON
		BANKSEL	    PR2				
		MOVLW	    0x19			;SET PR2 TO 25 FOR 38kHz PERIOD
		;MOVLW	    0x11
		MOVWF	    PR2				
		
;*** ALTERNATE PIN FUNCTION *****
		
		BANKSEL	    APFCON1
		CLRF	    APFCON1
		CLRF	    APFCON2
 
;*** PORT A SETUP **** PORT A SET AS LOGIC OUTPUT (NOT USED)

		BANKSEL	    PORTA
		CLRF	    PORTA	
		BANKSEL	    LATA
		CLRF	    LATA
		BANKSEL	    TRISA
		MOVLW	    0xFF
		MOVWF	    TRISA			;SET PORTA ALL AS INPUT
		BANKSEL	    ODCONA
		CLRF	    ODCONA			;DISABLE PORTA OPEN-DRAIN		
		BANKSEL	    ANSELA			
		CLRF	    ANSELA			;SET PORTA ALL AS DIGITAL INPUT
		BANKSEL	    SLRCONA
		CLRF	    SLRCONA			;SET THE SLEW RATE FOR PORT A TO MAXIMUM
		
;*** PORT B SETUP **** PORT B USED AS LOGIC INPUT
		
		BANKSEL	    PORTB
		CLRF	    PORTB				
		BANKSEL	    LATB
		CLRF	    LATB
		BANKSEL	    TRISB
		MOVLW	    0xFF				
		MOVWF	    TRISB			;SET PORTB ALL AS INPUTS
		BANKSEL	    ODCONB
		CLRF	    ODCONB			;DISABLE PORTB OPEN-DRAIN
		BANKSEL	    ANSELB
		CLRF	    ANSELB			;MAKE PORTB DIGITAL INPUTS
		BANKSEL	    SLRCONB
		CLRF	    SLRCONB			;SET THE SLEW RATE FOR PORT B TO MAXIMUM
		BANKSEL	    IOCBN
		MOVLW	    0x1F
		MOVWF	    IOCBN
		
;*** PORT C SETUP **** 
		
		BANKSEL	    PORTC
		CLRF	    PORTC		
		BANKSEL	    LATC
		CLRF	    LATC
		BANKSEL	    TRISC
		MOVLW	    0x00				
		MOVWF	    TRISC			;SET PORTC ALL AS INPUTS
		BANKSEL	    ODCONC
		CLRF	    ODCONC			;DISABLE PORTC OPEN-DRAIN
		BANKSEL	    ANSELC
		CLRF	    ANSELC			;MAKE PORTC ALL DIGITAL I/O
		BANKSEL	    SLRCONC
		;MOVLW	    0xFF
		;MOVWF	    SLRCONC
		CLRF	    SLRCONC			;SET THE SLEW RATE FOR PORT C TO MAXIMUM
		BANKSEL	    INLVLA
		CLRF	    INLVLA
		
;*** BOOT CODE ****
			    
		BCF	    INTCON,0
		BANKSEL	    T1CON
		BSF	    T1CON,0
		BANKSEL	    T2CON
		BSF	    T2CON, 2			;ENABLE TIMER 2
		
		
		RETURN					;RETURN TO .asm FILE