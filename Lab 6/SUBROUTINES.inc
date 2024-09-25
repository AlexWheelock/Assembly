SUBROUTINES_CODE CODE
DELAY						    ;2 second Delay for displaying the "7"
	    MOVLW   0XE5			    ;229
	    MOVWF   COUNT3
    LOOP3
	    MOVLW   0X00			    ;256
	    MOVWF   COUNT1
    LOOP1
	    MOVLW   0X0A			    ;10
	    MOVWF   COUNT2
    LOOP2
	    DECFSZ  COUNT2
	    GOTO    LOOP2
	    DECFSZ  COUNT1
	    GOTO    LOOP1
	    DECFSZ  COUNT3
	    GOTO    LOOP3
	    MOVLW   0XE1			    ;225
	    MOVWF   COUNT5
    LOOP5
	    MOVLW   0X08			    ;8
	    MOVWF   COUNT4
    LOOP4
	    DECFSZ  COUNT4
	    GOTO    LOOP4
	    NOP
	    DECFSZ  COUNT5
	    GOTO    LOOP5
	    NOP					    ;/
	    NOP					    ;
	    NOP					    ;
	    NOP					    ;
	    NOP					    ;
	    NOP					    ;
	    NOP					    ;
	    NOP					    ;				
	    NOP					    ;NOPs used to get the delay to exactly 2 seconds
	    NOP					    ;
	    NOP					    ;
	    NOP					    ;
	    NOP					    ;
	    NOP					    ;
	    NOP					    ;
	    NOP					    ;
	    NOP					    ;\
	    RETURN   
DELAY2						    ;2 second Delay for displaying the "6"
	    MOVLW   0XE5			    ;229
	    MOVWF   COUNT8
    LOOP8
	    MOVLW   0X00			    ;256
	    MOVWF   COUNT6
    LOOP6
	    MOVLW   0X0A			    ;10
	    MOVWF   COUNT7
    LOOP7
	    DECFSZ  COUNT7
	    GOTO    LOOP7
	    DECFSZ  COUNT6
	    GOTO    LOOP6
	    DECFSZ  COUNT8
	    GOTO    LOOP8
	    MOVLW   0XE1			    ;225
	    MOVWF   COUNT10
    LOOP10
	    MOVLW   0X08			    ;8
	    MOVWF   COUNT9
    LOOP9
	    DECFSZ  COUNT9
	    GOTO    LOOP9
	    NOP
	    DECFSZ  COUNT10
	    GOTO    LOOP10
	    NOP					    ;/
	    NOP					    ;
	    NOP					    ;
	    NOP					    ;
	    NOP					    ;
	    NOP					    ;
	    NOP					    ;
	    NOP					    ;
	    NOP					    ;NOPs used to get the delay to exactly 2 seconds
	    NOP					    ;
	    NOP					    ;
	    NOP					    ;
	    NOP					    ;
	    NOP					    ;
	    NOP					    ;
	    NOP					    ;
	    NOP					    ;\
	    RETURN