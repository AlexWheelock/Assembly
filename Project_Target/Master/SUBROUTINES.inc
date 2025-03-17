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
  
    SEND_ENABLE
	    CALL	    I2CIDLE		;ENSURE THAT THE I2C LINES ARE IDLE
	    BANKSEL	    SSPCON2
	    BSF		    SSPCON2,0
	    BTFSC	    SSPCON2,0		;WAIT UNTIL START CONDITION IS COMPLETE
	    GOTO	    $-1
	    BANKSEL	    SSPBUF
	    MOVLW	    0x02		;SEND I2C ADDRESS OUT (WRITE TO ADDRESS 1)
	    MOVWF	    SSPBUF
	    BANKSEL	    SSPSTAT
	    BTFSC	    SSPSTAT,0		;WAIT UNTIL ALL DATA HAS BEEN SHIFTED OUT OF SSPBUF
	    GOTO	    $-1
	    CALL	    I2CIDLE		;ENSURE THAT THE I2C LINES ARE IDLE
	    BANKSEL	    SSPCON2
	    BTFSC	    SSPCON2,6		;DETERMINE IF THERE IS AN ACK FROM THE SLAVE
	    GOTO	    BAD1		;NACK FROM THE SLAVE, STOP COMMUNICATION
	    BANKSEL	    SSPBUF
	    MOVLW	    0x45
	    MOVWF	    SSPBUF		;TRANSMIT DATA TO SLAVE
	    BANKSEL	    SSPSTAT
	    BTFSC	    SSPSTAT,0		;WAIT UNTIL ALL DATA HAS BEEN SHIFTED OUT OF SSPBUF
	    GOTO	    $-1
	    CALL	    I2CIDLE		;ENSURE THAT THE I2C LINES ARE IDLE
	    BANKSEL	    SSPCON2
	    BTFSC	    SSPCON2,6		;DETERMINE IF THERE IS AN ACK FROM THE SLAVE
	    GOTO	    BAD1		;NACK FROM THE SLAVE, STOP COMMUNICATION
	    BSF		    SSPCON2,2		;CREATE STOP CONDITION
	    BTFSC	    SSPCON2,2
	    GOTO	    $-1			;WAIT FOR STOP CONDITION TO COMPLETE
	    BANKSEL	    PORTA
	    BSF		    ACTIVE_TARGET,0
	    RETURN

	    
    READ_TARGET_STATUS
	    CALL	    I2CIDLE		;ENSURE THAT THE I2C LINES ARE IDLE
	    BANKSEL	    SSPCON2
	    BSF		    SSPCON2,0
	    BTFSC	    SSPCON2,0		;WAIT UNTIL START CONDITION IS COMPLETE
	    GOTO	    $-1
	    BANKSEL	    SSPBUF
	    MOVLW	    0x03		;SEND I2C ADDRESS OUT (READ FROM ADDRESS 1)
	    MOVWF	    SSPBUF
	    BANKSEL	    SSPSTAT
	    BTFSC	    SSPSTAT,0		;WAIT UNTIL ALL DATA HAS BEEN SHIFTED OUT OF SSPBUF
	    GOTO	    $-1
	    CALL	    I2CIDLE		;ENSURE THAT THE I2C LINES ARE IDLE
	    BANKSEL	    SSPCON2
	    BTFSC	    SSPCON2,6		;DETERMINE IF THERE IS AN ACK FROM THE SLAVE
	    GOTO	    BAD1		;NACK FROM THE SLAVE, STOP COMMUNICATION
	    BSF		    SSPCON2,3		;ENABLE RCEN TO RECEIVE DATA FROM THE SLAVE
	    BANKSEL	    SSPSTAT
	    BTFSS	    SSPSTAT,0		;WAIT UNTIL ALL DATA HAS BEEN SHIFTED INTO SSPBUF FROM THE SLAVE
	    GOTO	    $-1
	    BANKSEL	    SSPBUF		;\
	    MOVFW	    SSPBUF		;\\READ STATUS OF CURRENT TARGET
	    BANKSEL	    PORTA		;//STORE BYTE INTO RECEIVED_BYTE REGISTER
	    MOVWF	    RECEIVED_BYTE	;/
	    BSF		    SSPCON2,2		;CREATE STOP CONDITION
	    BTFSC	    SSPCON2,2
	    GOTO	    $-1			;WAIT FOR STOP CONDITION TO COMPLETE
	    CALL	    TEST_RECEIVED_BYTE	;TEST TO SEE IF A PLAYER HAS SCORED
	    RETURN
	    
    
    TEST_RECEIVED_BYTE
	    ;PLAYER1 = 0x0F
	    ;PLAYER2 = 0xF0
	    ;NO SCORE = 0x00
	    BANKSEL	    PORTA
	    BTFSC	    RECEIVED_BYTE,2	;DETERMINE IF PLAYER 1 SCORED
	    GOTO	    PLAYER1_SCORED
	    BTFSC	    RECEIVED_BYTE,6	;DETERMINE IF PLAYER 2 SCORED
	    GOTO	    PLAYER2_SCORED
	    RETURN				;NEITHER PLAYER SCORED
	
	PLAYER1_SCORED
	    CLRF	    ACTIVE_TARGET
	    BSF		    PORTA,1
	    BCF		    PORTA,0
	    RETURN
	    
	PLAYER2_SCORED
	    CLRF	    ACTIVE_TARGET
	    BSF		    PORTA,0
	    BCF		    PORTA,1
	    RETURN
	    
	    
    I2CIDLE	
	    MOVLW	    0X1F		;Load Bus Test Value
	    BANKSEL	    SSPCON2		
	    ANDWF	    SSPCON2,W		;Compare 1F to check for 5 busy conditions
	    BANKSEL	    STATUS
	    BTFSS	    STATUS,Z		;Test Zero Bit
	    GOTO	    I2CIDLE		;Z=0  Bus is still busy -repeat
    CHECKR_W					;Z=1 Not Busy
	    BANKSEL	    SSPSTAT
	    BTFSC	    SSPSTAT,2		;see if SSP is transmitting data
	    GOTO	    CHECKR_W		;R_W = 1 - Still transmitting data
	    RETURN				;R_W = 0 - Transmit done



    BAD1	
	    MOVLW	    0XFF		;SET RETURN CODE TO -1
	    BANKSEL	    SSPCON2
	    BSF		    SSPCON2,PEN		;GENERATE A STOP CONDITION
    LOOP5	
	    BTFSC	    SSPCON2,PEN		;IS STOP CONDITION DONE
	    GOTO	    LOOP5
	    RETURN

		