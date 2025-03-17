;************************************************************************************
;										    *
;   Filename:	    SUBROUTINES.inc						    *
;   Date:	    November 5, 2024						    *
;   File Version:   1								    *
;   Author:	    Alex Wheelock						    *
;   Company:	    Idaho State University					    *
;   Description:    Contains all subroutines needed for the Laser Shooting Game     *
;		    project. Subroutines include the ability to swap between the    *
;		    two player PWM frequencies of 56kHz and 38kHz, and the ability  *
;		    to shoot in three different modes (single-shot, burst,	    *
;		    & continuous), to move a solenoid and shoot	the laser with the  *
;		    press of a button.						    *
;										    *
;************************************************************************************

;************************************************************************************
;										    *
;   Revision History:								    *
;										    *
;   1:	Initialize file, get everything working the way that I thought it should    *
;	work.									    *								    *
;										    *
;************************************************************************************		
	    
SUBROUTINES_CODE CODE
  
   RECEIVE
	    BANKSEL	SSPBUF
	    MOVFW	SSPBUF
	    BANKSEL	SSPSTAT			;DETERMINE IF THE RECEIVED BYTE WAS DATA OR ADDRESS
	    BTFSS	SSPSTAT,5		;1=DATA, 0=ADDRESS
	    GOTO	CHECK_READ_OR_WRITE
	
    CHECK_READ_OR_WRITE	
	    BTFSS	SSPSTAT,2
	    GOTO	SAVE_DATA
	    GOTO	TRANSMIT_STATUS
	
    SAVE_DATA
	    BANKSEL	PORTA
	    MOVWF	RECEIVED_DATA
	    BSF		NEW_DATA,0
	    RETURN
	    
	    
    TRANSMIT_STATUS
    
    
	    RETURN