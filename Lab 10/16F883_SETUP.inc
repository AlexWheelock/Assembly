;************************************************************************************
;										    *
;   Filename:	    16F883_SETUP.inc						    *
;   Date:	    September 24, 2024						    *
;   File Version:   2								    *
;   Author:	    Alex Wheelock						    *
;   Company:	    Idaho State University					    *
;   Description:    Firmware for setting up a PIC16F883				    *
;										    *
;************************************************************************************

;************************************************************************************
;										    *
;   Revision History:								    *
;										    *
;   1:	Initialize file for setting up the PIC16F883 micro-controller for Lab 7	    *
;	which simulates a traffic light.					    *
;										    *
;   2:	File setup for the final working traffic light with for logic supporting    *
;	cars approaching the light using buttons on RB1:0			    *
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
;DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	*
;OUTPUT		|OUTPUT		|OUTPUT		|OUTPUT		|OUTPUT		|OUTPUT		|INPUT		|INPUT		*
;---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------*		
;UNUSED		|UNUSED		|UNUSED		|UNUSED		|UNUSED		|UNUSED		|CONNECTED TO	|CONNECTED TO	*
;		|		|		|		|		|		|PUSH BUTTON	|PUSH BUTTON	*
;		|		|		|		|		|		|(USED FOR INT)	|(USED FOR INT)	*
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
;UNUSED		|UNUSED		|DRIVES E/W	|DRIVES E/W	|DRIVES E/W	|DRIVES N/S 	|DRIVES N/S	|DRIVES N/S	*
;		|		|RED LED	|YELLOW LED	|GREEN LED	|RED LED	|YELLOW LED	|GREEN LED	*
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
		BSF		PIE1, TMR2IE		;DISABLE TMR2 INTERUPT POR PWM
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
		CLRF	T1CON	
		
;*** TIMER 2 SETUP *****

		BANKSEL T2CON				;/
		MOVLW	0x6A				;Enable Timer 2, set pre-scale to 1:16, post-scale to 1:13
		MOVWF	T2CON				;\
		BANKSEL	PR2				;/
		MOVLW	0x6F				;Set PR2 to 156
		MOVWF	PR2				;\
 
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
		MOVLW	H'03'				;/SET PORTB RB1:0 as inputs, RB7:2 as outputs
		MOVWF	TRISB				;\
		BANKSEL ANSELH
		MOVLW	H'00'				;/DISABLES ANALOG INPUT TO ENSURE THAT IT IS A DIGITAL INPUT
		MOVWF	ANSELH				;\
		BANKSEL	IOCB
		MOVLW	H'00'				;/ENABLES INTERRUPT ON CHANGE FOR RB1:0
		MOVWF	IOCB				;\
		
;*** PORT C SETUP **** PORT C USED AS LOGIC OUTPUT TO DRIVE DOT MATRIX
		
		BANKSEL PORTC
		MOVLW	0x20				;/
		MOVWF	PORTC				;\INIT PORTC
		BANKSEL	TRISC
		MOVLW	0x00				;/
		MOVWF	TRISC				;\SET PORTC ALL AS OUTPUT
		
		BANKSEL	T2CON				;/
		BSF	T2CON,2				;\Turn Timer 2 on
		BSF	NS_TRUE,0			;\
		MOVLW	0xC8
		MOVWF	END_GREEN
		MOVLW	0xF0
		MOVWF	END_YELLOW
		CLRF	NS_CAR
		CLRF	EW_CAR
		MOVLW	0x28
		
		RETURN					;Return to .asm file