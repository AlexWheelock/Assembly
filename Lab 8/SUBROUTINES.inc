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
	    BANKSEL ADRESH		    ;/
	    MOVFW   ADRESH		    ;PUT ADC OUTPUT ONTO PORTC LEDS
	    MOVWF   PORTC		    ;\
	    BSF	    ADCON0,1		    ;RESTART ADC CONVERSION
	    RETURN			    ;RETURN TO ISR

