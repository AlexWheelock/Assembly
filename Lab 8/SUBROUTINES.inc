;************************************************************************************
;										    *
;   Filename:	    SUBROUTINES.asm						    *
;   Date:	    October 4, 2024						    *
;   File Version:   1								    *
;   Author:	    Alex Wheelock						    *
;   Company:	    Idaho State University					    *
;   Description:    Contains all subroutines needed for the traffic light in Lab 7. *
;										    *
;************************************************************************************

;************************************************************************************
;										    *
;   Revision History:								    *
;										    *
;   1:	Initialize file, set up for ADC conversion and output to PORTC.		    *
;										    *
;************************************************************************************		
	    
SUBROUTINES_CODE CODE

READADC
	    BANKSEL ADRESH
	    MOVFW   ADRESH
	    MOVWF   PORTC

