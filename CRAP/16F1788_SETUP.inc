;************************************************************************************
;										    *
;   Filename:	    16LF1788_SETUP.inc						    *
;   Date:	    November 5, 2024						    *
;   File Version:   1								    *
;   Author:	    Alex Wheelock						    *
;   Company:	    Idaho State University					    *
;   Description:    Firmware for setting up a PIC16LF1788 for the laser gun of the  *
;		    Laser Shooting Game project.				    *
;										    *
;************************************************************************************

;************************************************************************************
;										    *
;   Revision History:								    *
;										    *
;   1:	Iniitialize file, setup how I think it should be setup for the project.	    *
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
;UNUSED		|UNUSED		|UNUSED		|UNUSED		|UNUSED		|UNUSED	 	|UNUSED		|UNUSED		*
;		|		|		|		|		|	 	|		|		*
;		|		|		|		|		|		|		|		*
;		|		|		|		|		|		|		|		*		
;-------------------------------------------------------------------------------------------------------------------------------*
		
;===============================================================================================================================*
;							   PORTB BITMAP								*
;===============================================================================================================================*		
;		|		|		|		|		|		|		|		*
;	7	|	6	|	5	|	4	|	3	|	2	|	1	|	0	*
;---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------*	
;DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	|DIGITAL	*
;INPUT		|INPUT		|INPUT		|INPUT		|INPUT		|INPUT		|INPUT		|INPUT		*
;---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------*		
;UNUSED		|UNUSED		|UNUSED		|UNUSED		|INPUT USED FOR	|INPUT USED FOR	|INPUT USED FOR	|INPUT USED FOR	*
;		|		|		|		|BURST-FIRE	|CONTINUOUS/	|THE FREQUENCY	|TRIGGER BUTTON	*
;		|		|		|		|MODE SELECT	|FULL-AUTO MODE	|SELECT SWITCH	|		*
;		|		|		|		|		|SELECT		|		|		*		
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
;UNUSED		|UNUSED		|UNUSED		|UNUSED		|OUTPUT USED TO	|OUTPUT USED TO	|UNUSED		|UNUSED		*
    ;		|		|		|		|CONTROL THE	|CONTROL THE	|		|		*
;		|		|		|		|SOLENOID	|LASER	    	|		|		*
;		|		|		|		|		|		|		|		*		
;-------------------------------------------------------------------------------------------------------------------------------*			
		
INITIALIZE_CODE	CODE		
INITIALIZE	
	
;*** INTOSC SETUP *******
	
		BANKSEL	    OSCCON
		MOVLW	    0x3A			;ENABLE INTOSC, SET TO 4MHz
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
		BCF	    INTCON, IOCIE		;ENABLE IOC INTERUPT
		BCF	    INTCON, INTE		;DISABLE INT EXTERNAL INTERUPT
		BSF	    INTCON, PEIE		;ENABLE PERIPHERAL INTERUPTS
		BSF	    INTCON, GIE			;ENABLE ALL UNMASKED INTERUPTS
		
;*** SET PIE1-4 REG: *****
		
		BANKSEL	    PIE1
		MOVLW	    0x22
		MOVWF	    PIE1			;DISABLE ALL PIE1 INTERUPTS
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
		BSF	    TRISC,2			;DISABLE PWM OUTPUT DRIVER FOR CCP1 SETUP
		BANKSEL	    CCP1CON
		MOVLW	    0x3C		
		MOVWF	    CCP1CON			;SELECT PWM MODE FOR CCP1
		BANKSEL	    CCPR1L
		MOVLW	    0x0B			;PW FOR INITIALIZING 1.5ms PW AT 50Hz
		MOVWF	    CCPR1L

;*** TIMER 1 SETUP *****
		
		BANKSEL	    T1CON			
		MOVLW	    0xA8			;ENABLE & SELECT TMR1 LP OSCILLATOR, SET PRESCALE TO 1:4, DISABLE TMR1 AT THIS POINT
		BANKSEL	    T1GCON
		CLRF	    T1GCON			;DISABLE TMR1 GATE
		
;*** TIMER 2 SETUP *****

		BANKSEL	    T2CON
		MOVLW	    0x02
		MOVWF	    T2CON
		BANKSEL	    PR2				
		MOVLW	    0x9D			;SET PR2 FOR 20ms PERIOD
		MOVWF	    PR2				
		
;*** ALTERNATE PIN FUNCTION *****
		
		BANKSEL	    APFCON1
		MOVLW	    0x06
		MOVWF	    APFCON1			;SELECTS PORT MAPPING
		CLRF	    APFCON2			;SEE DATASHEET (pg 132-133) FOR MAPPING WITH BOTH CLEARED
 
;*** PORT A SETUP **** PORT A SET AS LOGIC OUTPUT (NOT USED)

		BANKSEL	    PORTA
		CLRF	    PORTA			;CLEAR PORTA
		BANKSEL	    LATA
		CLRF	    LATA			;CLEAR LATA
		BANKSEL	    TRISA
		MOVLW	    0x00
		;MOVLW	    0xFF
		MOVWF	    TRISA			;SET PORTA ALL AS INPUT
		BANKSEL	    ODCONA
		CLRF	    ODCONA			;DISABLE PORTA OPEN-DRAIN		
		BANKSEL	    ANSELA			
		CLRF	    ANSELA			;SET PORTA ALL AS DIGITAL INPUT
		BANKSEL	    SLRCONA
		CLRF	    SLRCONA			;SET THE SLEW RATE FOR PORT A TO MAXIMUM
		
;*** PORT B SETUP **** PORT B USED AS LOGIC INPUT
		
		BANKSEL	    PORTB
		CLRF	    PORTB			;CLEAR PORTB	
		BANKSEL	    LATB
		CLRF	    LATB			;CLEAR LATB
		BANKSEL	    TRISB
		MOVLW	    0xFF				
		MOVWF	    TRISB			;SET PORTB ALL AS INPUTS
		BANKSEL	    ODCONB
		CLRF	    ODCONB			;DISABLE PORTB OPEN-DRAIN
		BANKSEL	    ANSELB
		CLRF	    ANSELB			;MAKE PORTB DIGITAL INPUTS
		BANKSEL	    SLRCONB
		CLRF	    SLRCONB			;SET THE SLEW RATE FOR PORT B TO MAXIMUM
		BANKSEL	    IOCBP
		CLRF	    IOCBP
		BANKSEL	    IOCBN
		CLRF	    IOCBN
		BANKSEL	    INLVLB
		CLRF	    INLVLB			;SET PORTB INPUT THRESHOLD TO TTL (IGNORED NOISE BETTER FOR TRIGGER INPUT)	    	    
		
;*** PORT C SETUP **** 
		
		BANKSEL	    PORTC
		CLRF	    PORTC			;CLEAR PORTC
		BANKSEL	    LATC
		CLRF	    LATC			;CLEAR LATC
		BANKSEL	    TRISC
		MOVLW	    0x00				
		MOVWF	    TRISC			;SET PORTC ALL AS OUTPUTS
		BANKSEL	    ODCONC
		CLRF	    ODCONC			;DISABLE PORTC OPEN-DRAIN
		BANKSEL	    ANSELC
		CLRF	    ANSELC			;MAKE PORTC ALL DIGITAL I/O
		BANKSEL	    SLRCONC
		CLRF	    SLRCONC			;SET THE SLEW RATE FOR PORT C TO MAXIMUM
		
		
;*** UART RECEIVE SETUP ****
		
		;INITIALIZE SPBRGH/SPBRGL HERE********************
		;BANKSEL	    RCSTA
		;MOVLW	    0x90
		
		BANKSEL	SP1BRGH
		;MOVLW	0x19
		;MOVLW	0x13
		;MOVWF	SP1BRGH
		CLRF	SP1BRGH
		BANKSEL	SP1BRGL
		MOVLW	0x4A
		MOVWF	SP1BRGL
		;CLRF	SP1BRGL
		BANKSEL	BAUD1CON
		MOVLW	0x08
		MOVWF	BAUD1CON
		BANKSEL	TX1STA
		MOVLW	0x04
		MOVWF	TX1STA
		BANKSEL	RC1STA
		CLRF	RC1STA
		
;*** BOOT CODE ****
			    
		BANKSEL	    T2CON
		BSF	    T2CON,2			;ENABLE TIMER 2
		BANKSEL	TX1STA
		BSF	TX1STA,5
		BANKSEL	RC1STA
		BSF	RC1STA,4
		BSF	RC1STA,7
		BANKSEL	PIR1
		BCF	PIR1,5
		
		
		RETURN					;RETURN TO .asm FILE