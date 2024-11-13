;************************************************************************************
;										    *
;   Filename:	    SUBROUTINES.asm						    *
;   Date:	    October 4, 2024						    *
;   File Version:   2								    *
;   Author:	    Alex Wheelock						    *
;   Company:	    Idaho State University					    *
;   Description:    Contains all subroutines needed for the Servo section of Lab 8. *
;										    *
;************************************************************************************

;************************************************************************************
;										    *
;   Revision History:								    *
;										    *
;   1:	Initialize file, set up for ADC conversion and output to PORTC.		    *
;										    *	    
;   2:	Setup file to use the ADC to control a servo motor on PORTC		    *
;										    *
;************************************************************************************		
	    
SUBROUTINES_CODE CODE
		
NEW_CYCLE		
		BANKSEL	PORTA
		MOVFW	CURRENT_PW1
		MOVWF	PW1_COUNT
		MOVFW	CURRENT_PW2
		MOVWF	PW2_COUNT
		MOVFW	CURRENT_PW3		
		MOVWF	PW3_COUNT
		BSF	PW1_TRUE,0
		BSF	PW2_TRUE,0
		BSF	PW3_TRUE,0
		BSF	PORTC,5
		BSF	PORTC,6
		BSF	PORTC,7
		MOVLW	0xC8				;/SET THE PERIOD OF THE SERVO SIGNAL TO 20ms
		MOVWF	PERIOD				;\
		GOTO	GOBACK
		
NEW_CONVERSATION
		BANKSEL	PORTA
		BTFSC	WRITE,0
		GOTO	WRITE_DATA
		BTFSC	READ,0
		GOTO	READ_DATA
		BANKSEL	SSPBUF
		CLRF	SSPBUF
		BANKSEL	SSPSTAT
		BTFSC	SSPSTAT,2
		GOTO	SET_READ
    SET_WRITE
		BANKSEL	PORTA
		BSF	WRITE,0
		BCF	READ,0
		RETURN
    SET_READ
		BANKSEL	PORTA
		BSF	READ,0
		BCF	WRITE,0
		RETURN
		
    WRITE_DATA
		BANKSEL	SSPBUF
		MOVFW	SSPBUF
		CLRF	SSPBUF
		BANKSEL	PORTA
		MOVWF	RECEIVED_DATA
		BTFSC	RECEIVED_DATA,5
		BSF	UPDATE_SERVO1,0
		BTFSC	RECEIVED_DATA,6
		BSF	UPDATE_SERVO2,0
		BTFSC	RECEIVED_DATA,7
		BSF	UPDATE_SERVO3,0
		BCF	RECEIVED_DATA,5
		BCF	RECEIVED_DATA,6
		BCF	RECEIVED_DATA,7
				
		MOVLW	0x02
		SUBWF	RECEIVED_DATA,0
		BTFSS	STATUS,0
		GOTO	STEP1
		
		MOVLW	0x04
		SUBWF	RECEIVED_DATA,0
		BTFSS	STATUS,0
		GOTO	STEP2
		
		MOVLW	0x06
		SUBWF	RECEIVED_DATA,0
		BTFSS	STATUS,0
		GOTO	STEP3
		
		MOVLW	0x08
		SUBWF	RECEIVED_DATA,0
		BTFSS	STATUS,0
		GOTO	STEP4
		
		MOVLW	0x0A
		SUBWF	RECEIVED_DATA,0
		BTFSS	STATUS,0
		GOTO	STEP5
		
		MOVLW	0x0C
		SUBWF	RECEIVED_DATA,0
		BTFSS	STATUS,0
		GOTO	STEP6
		
		MOVLW	0x0E
		SUBWF	RECEIVED_DATA,0
		BTFSS	STATUS,0
		GOTO	STEP7
		
		MOVLW	0x10
		SUBWF	RECEIVED_DATA,0
		BTFSS	STATUS,0
		GOTO	STEP8
		
		MOVLW	0x12
		SUBWF	RECEIVED_DATA,0
		BTFSS	STATUS,0
		GOTO	STEP9
		
		MOVLW	0x14
		SUBWF	RECEIVED_DATA,0
		BTFSS	STATUS,0
		GOTO	STEP10
		
		MOVLW	0x16
		SUBWF	RECEIVED_DATA,0
		BTFSS	STATUS,0
		GOTO	STEP11
		
		MOVLW	0x18
		SUBWF	RECEIVED_DATA,0
		BTFSS	STATUS,0
		GOTO	STEP12
		
		MOVLW	0x1A
		SUBWF	RECEIVED_DATA,0
		BTFSS	STATUS,0
		GOTO	STEP13
		
		MOVLW	0x1C
		SUBWF	RECEIVED_DATA,0
		BTFSS	STATUS,0
		GOTO	STEP14
		
		MOVLW	0x1E
		SUBWF	RECEIVED_DATA,0
		BTFSS	STATUS,0
		GOTO	STEP15
		GOTO	STEP16
				
		STEP1
		MOVLW	0x07
		GOTO	TEST_SERVO_TO_UPDATE
		
		STEP2
		MOVLW	0x08
		GOTO	TEST_SERVO_TO_UPDATE
		
		STEP3
		MOVLW	0x09
		GOTO	TEST_SERVO_TO_UPDATE
		
		STEP4
		MOVLW	0x0A
		GOTO	TEST_SERVO_TO_UPDATE
		
		STEP55
		MOVLW	0x0B
		GOTO	TEST_SERVO_TO_UPDATE
		
		STEP6
		MOVLW	0x0C
		GOTO	TEST_SERVO_TO_UPDATE
		
		STEP7
		MOVLW	0x0D
		GOTO	TEST_SERVO_TO_UPDATE
		
		STEP8
		MOVLW	0x0E
		GOTO	TEST_SERVO_TO_UPDATE
		
		STEP9
		MOVLW	0x0F
		GOTO	TEST_SERVO_TO_UPDATE
		
		STEP10
		MOVLW	0x10
		GOTO	TEST_SERVO_TO_UPDATE
		
		STEP11
		MOVLW	0x11
		GOTO	TEST_SERVO_TO_UPDATE
		
		STEP12
		MOVLW	0x12
		GOTO	TEST_SERVO_TO_UPDATE
		
		STEP13
		MOVLW	0x13
		GOTO	TEST_SERVO_TO_UPDATE
		
		STEP14
		MOVLW	0x14
		GOTO	TEST_SERVO_TO_UPDATE
		
		STEP15
		MOVLW	0x15
		GOTO	TEST_SERVO_TO_UPDATE
		
		STEP16
		MOVLW	0x16
		GOTO	TEST_SERVO_TO_UPDATE
		
	
	TEST_SERVO_TO_UPDATE
		BTFSC	UPDATE_SERVO1
		GOTO	WRITE_TO_UPDATE_SERVO1
		BTFSC	UPDATE_SERVO2
		GOTO	WRITE_TO_UPDATE_SERVO2
		BTFSC	UPDATE_SERVO3
		GOTO	WRITE_TO_UPDATE_SERVO3
		GOTO	RESTART
		
	WRITE_TO_UPDATE_SERVO1
		
	
	WRITE_TO_UPDATE_SERVO2
	
	
	WRITE_TO_UPDATE_SERVO3
		
	
	RESTART
		BCF	UPDATE_SERVO1,0
		BCF	UPDATE_SERVO2,0
		BCF	UPDATE_SERVO3,0
		BCF	WRITE,0
		BCF	READ,0
		RETURN
	
    READ_DATA
		RETURN