SUBROUTINES_CODE CODE
DELAY
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
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    NOP
	    RETURN