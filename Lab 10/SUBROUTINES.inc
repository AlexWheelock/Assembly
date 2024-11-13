;************************************************************************************
;										    *
;   Filename:	    SUBROUTINES.asm						    *
;   Date:	    October 24, 2024						    *
;   File Version:   2								    *
;   Author:	    Alex Wheelock						    *
;   Company:	    Idaho State University					    *
;   Description:    Contains all subroutines needed for Lab 10. Being able to use a *
;		    Keypad to write ASCII character codes into the PIC EEPROM, then *
;		    displaying it onto a dot-matrix.				    *
;										    *
;************************************************************************************

;************************************************************************************
;										    *
;   Revision History:								    *
;										    *
;   1:	Initialize file, and set up the file along with includes		    *
;										    *
;   2:	Added Subroutines to get EEPROM write/read and displaying it on a dot-matrix*
;	working.								    *
;										    *
;************************************************************************************		
	    
SUBROUTINES_CODE CODE
 
    SET_F				
		MOVLW	H'46'				    ;F
		GOTO	SAVE_DATA			    ;Store F into CURRENT_VALUE
    SET_E
		MOVLW	H'45'				    ;E
		GOTO	SAVE_DATA			    ;Store E into CURRENT_VALUE
    SET_D   
		MOVLW	H'44'				    ;D
		GOTO	SAVE_DATA			    ;Store D into CURRENT_VALUE
    SET_C
		MOVLW	H'43'				    ;C
		GOTO	SAVE_DATA			    ;Store C into CURRENT_VALUE
    SET_B
		MOVLW	H'42'				    ;B
		GOTO	SAVE_DATA			    ;Store B into CURRENT_VALUE
    SET_A
		MOVLW	H'41'				    ;A
		GOTO	SAVE_DATA			    ;Store A into CURRENT_VALUE
    SET_9
		MOVLW	H'39'				    ;9
		GOTO	SAVE_DATA			    ;Store 9 into CURRENT_VALUE
    SET_8
		MOVLW	H'38'				    ;8 
		GOTO	SAVE_DATA			    ;Store 8 into CURRENT_VALUE
    SET_7
		MOVLW	H'37'				    ;7
		GOTO	SAVE_DATA			    ;Store 7 into CURRENT_VALUE
    SET_6
		MOVLW	H'36'				    ;6
		GOTO	SAVE_DATA			    ;Store 6 into CURRENT_VALUE
    SET_5
		MOVLW	H'35'				    ;5
		GOTO	SAVE_DATA			    ;Store 5 into CURRENT_VALUE
    SET_4
		MOVLW	H'34'				    ;4
		GOTO	SAVE_DATA			    ;Store 4 into CURRENT_VALUE
    SET_3
		MOVLW	H'33'				    ;3
		GOTO	SAVE_DATA			    ;Store 3 into CURRENT_VALUE	
    SET_2
		MOVLW	H'32'				    ;2
		GOTO	SAVE_DATA			    ;Store 2 into CURRENT_VALUE
    SET_1
		MOVLW	H'31'				    ;1
		GOTO	SAVE_DATA			    ;Store 1 into CURRENT_VALUE
    SET_0
		MOVLW	H'30'				    ;0
	SAVE_DATA
		MOVWF	CURRENT_VALUE			    ;Store ASCII character
		BCF 	TEST_INPUTS,0			    ;Stop tesing keypad input
		BSF	INPUT_FLAG,0			    ;Indicate that a valid input was registered
		GOTO	MAIN
		
		
    TEST_MODE_CHANGE
		BANKSEL	PORTA				    ;
		BTFSC	PORTA,0				    ;Determine if the Record button is pressed
		GOTO	START_RECORD			    ;Record button was presesd, start record
		BTFSC	PORTA,1				    ;Determine if the Play button is pressed
		GOTO	START_PLAY			    ;Play button was pressed, start playing stored EEPROM data
		DECFSZ	IDLE_COUNT			    ;Decrement IDLE_COUNT, lasts 1 second then changes display between blank and S
		GOTO	GOBACK				    ;IDLE_COUNT does not equal 0
    CONTINUE_IDLE
		BTFSC	PORTC,0				    ;Test if the current display is a S or blank
		GOTO	CLEAR_OUTPUT			    ;Output is S, make it blank
		MOVLW	0x53				    ;/Output is blank, make it S
		MOVWF	PORTC				    ;\
		GOTO	RESET_IDLE			    ;Reset the timing of IDLE_COUNT
	CLEAR_OUTPUT
		MOVLW	0x20				    ;/Make display blank
		MOVWF	PORTC				    ;\
	RESET_IDLE
		MOVLW	0x64				    ;/Reset IDLE_COUNT for 1 second intervals
		MOVWF	IDLE_COUNT			    ;\
		GOTO	GOBACK				    ;
		
		
    START_RECORD
		BANKSEL	PORTA				    ;
		MOVLW	0x32				    ;/Reset DELAY_COUNT for the input delay to be .5s
		MOVWF	DELAY_COUNT			    ;\
		BSF	TEST_INPUTS,0			    ;Allow PIC to test inputs from the keypad
		BCF	DELAY_FLAG,0			    ;Don't allow the PIC to go into the delay yet
		BCF	IDLE,0				    ;Take the PIC out of IDLE
		BSF	RECORD_MODE,0			    ;Put the PIC into Record mode
		BCF	PLAY_MODE,0			    ;Ensure that the PIC is not in Play mode
		BCF	INPUT_FLAG,0			    ;Ensure that no unexpected value is written into EEPROM
		CLRF	CURRENT_REGISTER		    ;Reset EEPROM address that we are writing to to 0
		MOVLW	0x23				    ;/Display # on the display
		MOVWF	PORTC				    ;\
		GOTO	GOBACK				    ;
    
    
    CONTINUE_RECORD
		BANKSEL	PORTA				    ;
		BTFSC	DELAY_FLAG,0			    ;Test if the inputs are currently being ignored	
		GOTO	DELAY_INPUTS			    ;Inputs are being ignored, handle delay
		BTFSS	INPUT_FLAG,0			    ;Test if there is a new valid input recorded
		GOTO	GOBACK				    ;PIC is currently waiting for an input
	VALID_INPUT_RECOGNIZED				    ;
		BCF	INPUT_FLAG,0			    ;Clear INPUT_FLAG
		BSF	DELAY_FLAG,0			    ;Set DELAY_FLAG
		MOVLW	0x32				    ;/Reset DELAY_COUNT
		MOVWF	DELAY_COUNT			    ;\
		BCF	PIR2,4				    ;Clear EEIF
		MOVFW	CURRENT_REGISTER		    ;Grab current EEPROM register address
		BANKSEL	EEADR				    ;
		MOVWF	EEADR				    ;Write the current EEPROM register address into EEADR
		BANKSEL	PORTC				    ;
		MOVFW	CURRENT_VALUE			    ;Grab the most recent stored character
		MOVWF	PORTC				    ;
		BANKSEL	EEDATA				    ;Write the most recent stored data into EEDAT
		MOVWF	EEDATA				    ;
		BANKSEL	EECON1				    ;
		BCF	EECON1,7			    ;Disable EEPGD, to access data EEPROM memory
		BSF	EECON1,2			    ;Enable EEPROM write
		BCF	INTCON,7			    ;Clear GIE
		BTFSC	INTCON,7			    ;/Ensure that GIE is cleared to prevent any interrupts
		GOTO	$-2				    ;\from pulling us out of the charge pump process
		MOVLW	0x55				    ;/
		MOVWF	EECON2				    ;Start the charge pump so that we are able to write
		MOVLW	0xAA				    ;EEDAT data into EEPROM at address of EEADR data
		MOVWF	EECON2				    ;\
		BSF	EECON1,1			    ;Write to EEPROM
	POLL_END_WRITE
		BTFSC	EECON1,1			    ;/Wait for the write to be done before moving on
		GOTO	POLL_END_WRITE			    ;\	
		BCF	EECON1,2			    ;Disable WREN to prevent any unexpected writes
		BANKSEL	PIR2				    ;
		BCF	PIR2,4				    ;Clear EEIF
		GOTO	GOBACK				    ;
    
    DELAY_INPUTS
		BANKSEL	PORTA				    ;
		DECFSZ	DELAY_COUNT			    ;Decrement DELAY_COUNT, test if zero
		GOTO	GOBACK				    ;DELAY_COUNT not equal to zero, leave ISR
	DELAY_DONE	
		BSF	TEST_INPUTS,0			    ;Allow the PIC to poll the keypad again
		BCF	DELAY_FLAG,0			    ;Clear DELAY_FLAG
		MOVLW	0x23				    ;/Set display back to #
		MOVWF	PORTC				    ;\
    		INCF	CURRENT_REGISTER		    ;Increment CURRENT_REGISTER
		MOVLW	0x0A				    ;10
		SUBWF	CURRENT_REGISTER,0		    ;/If not negative, then we wnat to end Record
		BTFSC	STATUS,0			    ;\If it is negative we want to continue until we've dealt with 10 registers
		GOTO	END_PLAY_RECORD			    ;Result was not negative, end Record
		GOTO	GOBACK				    ;Result was negative, continue
    
    
    START_PLAY
		BANKSEL	PORTA
		MOVLW	0x64				    ;/LOAD PLAY_COUNT
		MOVWF	PLAY_COUNT			    ;\
		BSF	PLAY_MODE,0			    ;SET PLAY_MODE FLAG
		BCF	IDLE,0				    ;CLEAR IDLE FLAG
		CLRF	CURRENT_REGISTER		    ;RESET CURRENT REGISTER ADDRESS
		MOVFW	CURRENT_REGISTER		    ;LOAD THE 0 INTO W
		BANKSEL	EEADR				    ;/SET THE EEPROM ADDRESS
		MOVWF	EEADR				    ;\
		BANKSEL	EECON1				    ;/
		BCF	EECON1,7			    ;CLEAR EEPG TO READ DATA MEMORY, START READ OF REGISTER
		BSF	EECON1,0			    ;\
		BANKSEL	EEDATA				    ;/GRAB THE READ DATA
		MOVFW	EEDATA				    ;\
		BANKSEL	PORTC				    ;/DISPLAY THE DATA FROM EEPROM ON THE DOT-MATRIX
		MOVWF	PORTC				    ;\
		GOTO	GOBACK
    
    
    CONTINUE_PLAY
		BANKSEL	PORTA				    ;
		DECFSZ	PLAY_COUNT			    ;Decrement PLAY_COUNT timer, determine if 1s is up
		GOTO	GOBACK				    ;1s is not up, continue displaying the same character
		INCF	CURRENT_REGISTER		    ;PLAY_COUNT equals zero, Increment the EEPROM register we are reading from
		MOVLW	0x0A				    ;10
		SUBWF	CURRENT_REGISTER,0		    ;/Determine if we have read all 10 registers
		BTFSC	STATUS,0			    ;\
		GOTO	END_PLAY_RECORD			    ;the PIC has read all 10 EEPROM registers, end Play
	STILL_PLAYING
		MOVLW	0x64				    ;/Reset PLAY_COUNT
		MOVWF	PLAY_COUNT			    ;\
		MOVFW	CURRENT_REGISTER		    ;LOAD THE 0 INTO W
		BANKSEL	EEADR				    ;/SET THE EEPROM ADDRESS
		MOVWF	EEADR				    ;\
		BANKSEL	EECON1				    ;/
		BCF	EECON1,7			    ;CLEAR EEPG TO READ DATA MEMORY, START READ OF REGISTER
		BSF	EECON1,0			    ;\
		BANKSEL	EEDATA				    ;/GRAB THE READ DATA
		MOVFW	EEDATA				    ;\
		BANKSEL	PORTC				    ;/DISPLAY THE DATA FROM EEPROM ON THE DOT-MATRIX
		MOVWF	PORTC				    ;\
		GOTO	GOBACK
		
		
    END_PLAY_RECORD
    		BSF	IDLE,0				    ;Set IDLE
		CLRF	CURRENT_REGISTER		    ;Clear CURRENT_REGISTER back to 0
		MOVLW	0x64				    ;/Reset IDLE_COUNT
		MOVWF	IDLE_COUNT			    ;\
		BCF	PLAY_MODE,0			    ;Clear PLAY_MODE
		BCF	RECORD_MODE,0			    ;Clear RECORD_MODE
		BCF	INPUT_FLAG,0			    ;Clear INPUT_FLAG
		MOVLW	0x53				    ;/Display S
		MOVWF	PORTC				    ;\
		GOTO	GOBACK				    ;