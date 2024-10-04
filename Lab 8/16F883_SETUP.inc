;************************************************************************************
;										    *
;   Filename:	    16F883_SETUP.inc						    *
;   Date:	    October 4, 2024						    *
;   File Version:   1								    *
;   Author:	    Alex Wheelock						    *
;   Company:	    Idaho State University					    *
;   Description:    Firmware for setting up a PIC16F883				    *
;										    *
;************************************************************************************

;************************************************************************************
;										    *
;   Revision History:								    *
;										    *
;   1:	Initialize file for setting up the PIC16F883 micro-controller for Lab 8	    *
;	for A/D conversion.		    					    *
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
;EXT		|EXT		|UNUSED		|UNUSED		|UNUSED		|UNUSED		|UNUSED		|UNUSED		*
;OSCILLATOR	|OSCILLATOR	|		|		|		|		|		|		*
;IN		|IN		|		|		|		|		|		|		*
;		|		|		|		|		|		|		|		*		
;-------------------------------------------------------------------------------------------------------------------------------*
		
;===============================================================================================================================*
;							   PORTB BITMAP								*
;===============================================================================================================================*		
;		|		|		|		|		|		|		|		*
;	7	|	6	|	5	|	4	|	3	|	2	|	1	|	0	*
;---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------*	
;DIGITAL	|DIGITAL	|ANALOG		|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	*
;INPUT		|INPUT		|INPUT		|INPUT		|INPUT		|INPUT		|INPUT		|INPUT		*
;---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------*		
;UNUSED		|UNUSED		|TAKES ANALOG	|UNUSED		|UNUSED		|UNUSED		|UNUSED		|UNUSED		*
;		|		|INPUT FROM THE	|		|		|		|		|		*
;		|		|LM34 TEMP	|		|		|		|		|		*
;		|		|SENSOR		|		|		|		|		|		*		
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
;DRIVES BIT-7	|DRIVES BIT-6	|DRIVES BIT-5	|DRIVES BIT-4	|DRIVES BIT-3	|DRIVES BIT-2 	|DRIVES BIT-1	|DRIVES BIT-0	*
;LED		|LED		|LED    	|LED		|LED		|LED		|LED		|LED		*
;		|		|		|		|		|		|		|		*
;		|		|		|		|		|		|		|		*		
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
		BCF		PIE1, TMR2IE		;DISABLE TMR2 INTERUPT POR PWM
		BCF		PIE1, CCP1IE		;DISABLE CCP1 INTERUPT
		BCF		PIE1, SSPIE		;DISABLE SSP INTERUPT
		BCF		PIE1, TXIE		;DISABLE USART TRANSMIT INTERUPT
		BCF		PIE1, RCIE		;DISABLE USART RECIEVE INTERUPT
		BSF		PIE1, ADIE		;DISABLE A/D INTERUPT
		BCF		PIE1, 7			;DISABLE PSP INTERUPT
		
;*** SET CCP1CON REG: **
		
		BANKSEL CCP1CON
		MOVLW	H'00'				;DISABLE PWM & CCP
		MOVWF	CCP1CON

;*** TIMER 1 SETUP *****
		
		BANKSEL T1CON
		CLRF	T1CON	
		
;*** TIMER 2 SETUP *****

		BANKSEL T2CON				;/
		CLRF	T2CON
 
;*** PORT A SETUP **** PORT A SET AS LOGIC OUTPUT (NOT USED)

		BANKSEL PORTA
		CLRF	PORTA				;INIT PORTA
		BANKSEL	TRISA
		MOVLW	H'FF'				;/SET PORTA ALL AS INPUT
		MOVWF	TRISA				;\
		
;*** PORT B SETUP **** PORT B USED AS LOGIC INPUT
		
		BANKSEL PORTB
		CLRF	PORTB				;INIT PORT B
		BANKSEL	TRISB
		MOVLW	H'FF'				;/SET PORTB RB7:0 as inputs
		MOVWF	TRISB				;\
		BANKSEL ANSELH
		MOVLW	H'20'				;/ENABLES RB5 AS ANALOG INPUT, LEAVES THE REST AS DIGITAL INPUTS
		MOVWF	ANSELH				;\
		BANKSEL	IOCB
		MOVLW	H'00'				;/DISABLES PORTB IOC
		MOVWF	IOCB				;\
		
;*** PORT C SETUP **** PORT C USED AS LOGIC OUTPUT TO DRIVE DOT MATRIX
		
		BANKSEL PORTC
		MOVLW	0x21				;/
		MOVWF	PORTC				;\INIT PORTC
		BANKSEL	TRISC
		MOVLW	0x00				;/
		MOVWF	TRISC				;\SET PORTC ALL AS OUTPUT
		
;*** ADCON SETUP ****
		
		BANKSEL	ADCON0
		MOVLW	0x74
		MOVWF	ADCON0
		BANKSEL	ADCON1
		MOVLW	0x00
		MOVWF	ADCON1
		BANKSEL	PIR1
		CLRF	PIR1
		
		BANKSEL	ADCON0
		BSF	ADCON0,0
		BSF	ADCON0,1
		
		RETURN					;Return to .asm file