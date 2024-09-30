;************************************************************************************
;										    *
;   Filename:	    SUBROUTINES2.asm						    *
;   Date:	    September 24, 2024						    *
;   File Version:   3								    *
;   Author:	    Alex Wheelock						    *
;   Company:	    Idaho State University					    *
;   Description:    Subroutines used for Lab 5.6 (ISR) where a dot-matrix displays  *
;		    a "1" at all times on PORTC, with two buttons causing interrupts*
;		    using IOCB on PORTB. One causes a "7" to display, one causes a  *
;		    "6", and the "7" has priority and is able to interrupt the "6"  *
;		    but will always return back to "6" before returning to "1".	    *
;										    *
;************************************************************************************

;************************************************************************************
;										    *
;   Revision History:								    *
;										    *
;   1:	Initialize file, and set up the file along with includes		    *
;										    *
;   2:	Setup interrupt when RB0 push button is pressed to cause a 2 second change  * 
;	on the output, making the dot-matrix display a 7, then reverting back to a  *
;	"1".									    *
;										    *	
;   3:	Adding a push button that displays a "6" on RB1, that can be interrupted by *
;	the button on RB0, but cannot override the RB0 ISR.			    *
;										    *
;************************************************************************************		
	    
SUBROUTINES_CODE CODE
 
DELAY						;/Start of 2 second Delay
	    MOVLW   0XE5		        ;229
	    MOVWF   COUNT3			;
    LOOP3					;
	    MOVLW   0XE8		        ;232
	    MOVWF   COUNT1			;
    LOOP1					;
	    MOVLW   0X0A			;10
	    MOVWF   COUNT2			;
    LOOP2					;
	    DECFSZ  COUNT2			;
	    GOTO    LOOP2			;
	    BTFSC   PRIORITY,0			;Test PRIORITY, if set, then a "7" is being displayed, skip past further testing
	    GOTO    PRIORITYHIGH		;PRIORITY is set, skip further testing
	    BTFSC   PORTB,RB0			;PRIORITY is not set, test RB0
	    CALL    SET7			;PRIORITY is not set, RB0 is set, interrupt to display a 7
    PRIORITYHIGH				;
	    DECFSZ  COUNT1			;
	    GOTO    LOOP1			;
	    DECFSZ  COUNT3			;
	    GOTO    LOOP3			;
	    MOVLW   0XE1			;225
	    MOVWF   COUNT5			;
    LOOP5					;
	    MOVLW   0X07			;7
	    MOVWF   COUNT4			;
    LOOP4					;
	    DECFSZ  COUNT4			;
	    GOTO    LOOP4			;
	    NOP					;
	    DECFSZ  COUNT5			;
	    GOTO    LOOP5			;\End of 2 second delay
	    RETURN				;Return to interrupt where delay was called
    SET7					;Skip to here if interrupting a 6, and the current W & STATUS need to be saved
	    MOVWF   W_TEMP			;/
	    SWAPF   STATUS,W			;Saves the W & STATUS registers into a temporary location to not interfere with the MAIN code that was interrupted, when resumed
	    MOVWF   STATUS_TEMP			;\
    SAVEDALREADY				;Skip here if not interrupting a 6, since W & STATUS are already saved
	    BSF	    PRIORITY,0			;Set priority bit to prevent polling and possible interrupt during delay later
	    MOVFW   PORTC			;/
	    MOVWF   PREVIOUSVALUE		;\Store the current output to be reloaded later
	    MOVFW   COUNT1			;/
	    MOVWF   COUNT1_TEMP			;
	    MOVFW   COUNT2			;
	    MOVWF   COUNT2_TEMP			;
	    MOVFW   COUNT3			;Store the current COUNTs into temporary registers, so that interrupting 6 does not break anything
	    MOVWF   COUNT3_TEMP			;
	    MOVFW   COUNT4			;
	    MOVWF   COUNT4_TEMP			;
	    MOVFW   COUNT5			;
	    MOVWF   COUNT5_TEMP			;\
	    MOVLW   0X37			;/Set the output to display a "7"
	    MOVWF   PORTC			;\
	    CALL    DELAY			;Cause a 2 second delay
	    MOVFW   PREVIOUSVALUE		;/Set the output to display previously stored output
	    MOVWF   PORTC			;\
	    BCF	    PRIORITY,0			;Clear PRIORITY, since 7 is no longer being displayed
	    MOVFW   COUNT1_TEMP			;/
	    MOVWF   COUNT1			;
	    MOVFW   COUNT2_TEMP			;
	    MOVWF   COUNT2			;
	    MOVFW   COUNT3_TEMP			;Reload the previously stored COUNTs into the current COUNT registers
	    MOVWF   COUNT3			;
	    MOVFW   COUNT4_TEMP			;
	    MOVWF   COUNT4			;
	    MOVFW   COUNT5_TEMP			;
	    MOVWF   COUNT5			;\
						;
	    BTFSS   INT6,0			;Test if 6 is being interrupted, if so, load stored W & STATUS back into the current registers
	    GOTO    OVERRIDDEN			;6 is not being interrupted, skip loading temp registers back into current
	    SWAPF   STATUS_TEMP,W		;/
	    MOVWF   STATUS			;Move the previous W & STATUS registers back into the W & STATUS registers
	    SWAPF   W_TEMP,F			;
	    SWAPF   W_TEMP,W			;\
    OVERRIDDEN					;6 is not being interrupted, jump here to bypass reloading W & STATUS
	    RETURN				;Return to previously called location
    SET6					;Set the output to display a 6
	    SWAPF   STATUS_TEMP,W		;/
	    MOVWF   STATUS			;Move the previous W & STATUS registers back into the W & STATUS registers
	    SWAPF   W_TEMP,F			;
	    SWAPF   W_TEMP,W			;\
	    MOVWF   W_TEMP2			;/
	    SWAPF   STATUS,W			;Saves the W & STATUS registers TEMP2 registers for the duration that 6 is being displayed, so that 7 can interrupt it
	    MOVWF   STATUS_TEMP2		;\
	    BANKSEL PORTC			;
	    BSF	    INT6,0			;Set INT6 to indicate that 6 is currently being displayed
	    MOVLW   0X36			;/Set the output to display a "6"
	    MOVWF   PORTC			;\
	    CALL    DELAY			;Cause a 2 second delay
	    BCF	    INT6,0			;Clear INT6 to indicate that 6 is no longer being displayed
	    SWAPF   STATUS_TEMP2,W		;/
	    MOVWF   STATUS			;Move the previous stored TEMP2 registers into the current W & STATUS registers
	    SWAPF   W_TEMP2,F			;
	    SWAPF   W_TEMP2,W			;\
	    MOVWF   W_TEMP			;/
	    SWAPF   STATUS,W			;Saves the W & STATUS registers into TEMP registers so that it can end properly
	    MOVWF   STATUS_TEMP			;\
	    GOTO    GOBACK			;GOTO GOBACK, which reloads temp registers back into W & STATUS, and clears RBIF before returning
