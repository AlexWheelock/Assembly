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

TEST_HANDSHAKE
		MOVFW	RCREG
		MOVWF	STORED_RCREG
		CLRF	RCREG
		BCF	PIR1,5
		BTFSC	COMM_STARTED,0
		GOTO	SAVE_RCREG
		MOVWF	STORED_RCREG
		BTFSC	STORED_RCREG,7
		GOTO	LEAVE_SUB
		BTFSC	STORED_RCREG,6
		GOTO	LEAVE_SUB
		BTFSS	STORED_RCREG,5
		GOTO	LEAVE_SUB
		BTFSC	STORED_RCREG,4
		GOTO	LEAVE_SUB
		BTFSC	STORED_RCREG,3
		GOTO	LEAVE_SUB
		BTFSS	STORED_RCREG,2
		GOTO	LEAVE_SUB
		BTFSC	STORED_RCREG,1
		GOTO	LEAVE_SUB
		BTFSC	STORED_RCREG,0
		GOTO	LEAVE_SUB
		BSF	COMM_STARTED,0
		MOVLW	0x24
		MOVWF	TXREG
		GOTO	LEAVE_SUB
SAVE_RCREG
		MOVFW	RCREG
		MOVWF	CURRENT_RCREG
		BCF	COMM_STARTED,0
LEAVE_SUB
		RETURN
		
NEW_CYCLE
		BSF	PW_TRUE,0
		BSF	PORTA,1
		MOVLW	0xC8
		MOVWF	PERIOD
DETERMINE_PW
		MOVLW	0x07				    ;/Test if the ADC is currently in step 0 or at least step 1,
		SUBWF	CURRENT_RCREG,0			    ;by subtracting the stored ADC value and subtracting 17
		BTFSS	STATUS,0			    ;Test if the ALU function resulted in a borrow by the carry/borrow being low (indicating a negative)
		GOTO	STEP0				    ;Negative number, ADC is currently in the Step 0 range
		
		MOVLW	0x13				    ;/Test if the ADC is currently in step 1 or at least step 2,
		SUBWF	CURRENT_RCREG,0			    ;by subtracting the stored ADC value and subtracting 34
		BTFSS	STATUS,0			    ;Test if the ALU function resulted in a borrow by the carry/borrow being low (indicating a negative)
		GOTO	STEP1				    ;\Negative number, ADC is currently in the Step 1 range
		
		MOVLW	0x1F				    ;/Test if the ADC is currently in step 0 or at least step 3,
		SUBWF	CURRENT_RCREG,0			    ;by subtracting the stored ADC value and subtracting 51
		BTFSS	STATUS,0			    ;Test if the ALU function resulted in a borrow by the carry/borrow being low (indicating a negative)
		GOTO	STEP2				    ;\Negative number, ADC is currently in the Step 2 range
		
		MOVLW	0x2B				    ;/Test if the ADC is currently in step 0 or at least step 4,
		SUBWF	CURRENT_RCREG,0			    ;by subtracting the stored ADC value and subtracting 68
		BTFSS	STATUS,0			    ;Test if the ALU function resulted in a borrow by the carry/borrow being low (indicating a negative)
		GOTO	STEP3				    ;\Negative number, ADC is currently in the Step 3 range
		
		MOVLW	0x37				    ;/Test if the ADC is currently in step 0 or at least step 5,
		SUBWF	CURRENT_RCREG,0			    ;by subtracting the stored ADC value and subtracting 85
		BTFSS	STATUS,0			    ;Test if the ALU function resulted in a borrow by the carry/borrow being low (indicating a negative)	
		GOTO	STEP4				    ;\Negative number, ADC is currently in the Step 4 range
		
		MOVLW	0x43				    ;/Test if the ADC is currently in step 5 or at least step 6,
		SUBWF	CURRENT_RCREG,0			    ;by subtracting the stored ADC value and subtracting 102
		BTFSS	STATUS,0			    ;Test if the ALU function resulted in a borrow by the carry/borrow being low (indicating a negative)
		GOTO	STEP5				    ;\Negative number, ADC is currently in the Step 5 range
		
		MOVLW	0x4F				    ;/Test if the ADC is currently in step 6 or at least step 7,
		SUBWF	CURRENT_RCREG,0			    ;by subtracting the stored ADC value and subtracting 119
		BTFSS	STATUS,0			    ;Test if the ALU function resulted in a borrow by the carry/borrow being low (indicating a negative)
		GOTO	STEP6				    ;\Negative number, ADC is currently in the Step 6 range
		
		MOVLW	0x5B				    ;/Test if the ADC is currently in step 7 or at least step 8,
		SUBWF	CURRENT_RCREG,0			    ;by subtracting the stored ADC value and subtracting 136
		BTFSS	STATUS,0			    ;Test if the ALU function resulted in a borrow by the carry/borrow being low (indicating a negative)	
		GOTO	STEP7				    ;\Negative number, ADC is currently in the Step 7 range
		
		MOVLW	0x67				    ;/Test if the ADC is currently in step 8 or at least step 9,
		SUBWF	CURRENT_RCREG,0			    ;by subtracting the stored ADC value and subtracting 153
		BTFSS	STATUS,0			    ;Test if the ALU function resulted in a borrow by the carry/borrow being low (indicating a negative)	
		GOTO	STEP8				    ;\Negative number, ADC is currently in the Step 8 range
		
		MOVLW	0x73				    ;/Test if the ADC is currently in step 9 or at least step 10,
		SUBWF	CURRENT_RCREG,0			    ;by subtracting the stored ADC value and subtracting 170
		BTFSS	STATUS,0			    ;Test if the ALU function resulted in a borrow by the carry/borrow being low (indicating a negative)	
		GOTO	STEP9				    ;\Negative number, ADC is currently in the Step 9 range
		
		MOVLW	0x7F				    ;/Test if the ADC is currently in step 10 or at least step 11,
		SUBWF	CURRENT_RCREG,0			    ;by subtracting the stored ADC value and subtracting 187
		BTFSS	STATUS,0			    ;Test if the ALU function resulted in a borrow by the carry/borrow being low (indicating a negative)	
		GOTO	STEP10				    ;\Negative number, ADC is currently in the Step 10 range
		
		MOVLW	0x8B				    ;/Test if the ADC is currently in step 11 or at least step 12,
		SUBWF	CURRENT_RCREG,0			    ;by subtracting the stored ADC value and subtracting 204
		BTFSS	STATUS,0			    ;Test if the ALU function resulted in a borrow by the carry/borrow being low (indicating a negative)	
		GOTO	STEP11				    ;\Negative number, ADC is currently in the Step 11 range
		
		MOVLW	0x97				    ;/Test if the ADC is currently in step 12 or at least step 13,
		SUBWF	CURRENT_RCREG,0			    ;by subtracting the stored ADC value and subtracting 221
		BTFSS	STATUS,0			    ;Test if the ALU function resulted in a borrow by the carry/borrow being low (indicating a negative)	
		GOTO	STEP12				    ;\Negative number, ADC is currently in the Step 12 range
		
		MOVLW	0xAF				    ;/Test if the ADC is currently in step 13 or at least step 14,
		SUBWF	CURRENT_RCREG,0			    ;by subtracting the stored ADC value and subtracting 238
		BTFSS	STATUS,0			    ;Test if the ALU function resulted in a borrow by the carry/borrow being low (indicating a negative)	
		GOTO	STEP13				    ;\Negative number, ADC is currently in the Step 13 range
		
		MOVLW	0xBB				    ;/Test if the ADC is currently in step 14 or step 15,
		SUBWF	CURRENT_RCREG,0			    ;by subtracting the stored ADC value and subtracting 255
		BTFSS	STATUS,0			    ;Test if the ALU function resulted in a borrow by the carry/borrow being low (indicating a negative)	
		GOTO	STEP14				    ;\Negative number, ADC is currently in the Step 14 range
		
		MOVLW	0xC7				    ;/Test if the ADC is currently in step 14 or step 15,
		SUBWF	CURRENT_RCREG,0			    ;by subtracting the stored ADC value and subtracting 255
		BTFSS	STATUS,0			    ;Test if the ALU function resulted in a borrow by the carry/borrow being low (indicating a negative)	
		GOTO	STEP15				    ;\Negative number, ADC is currently in the Step 14 range
		
		MOVLW	0xD3				    ;/Test if the ADC is currently in step 14 or step 15,
		SUBWF	CURRENT_RCREG,0			    ;by subtracting the stored ADC value and subtracting 255
		BTFSS	STATUS,0			    ;Test if the ALU function resulted in a borrow by the carry/borrow being low (indicating a negative)	
		GOTO	STEP16				    ;\Negative number, ADC is currently in the Step 14 range
		
		MOVLW	0xDF				    ;/Test if the ADC is currently in step 14 or step 15,
		SUBWF	CURRENT_RCREG,0			    ;by subtracting the stored ADC value and subtracting 255
		BTFSS	STATUS,0			    ;Test if the ALU function resulted in a borrow by the carry/borrow being low (indicating a negative)	
		GOTO	STEP17				    ;\Negative number, ADC is currently in the Step 14 range
		
		MOVLW	0xEB				    ;/Test if the ADC is currently in step 14 or step 15,
		SUBWF	CURRENT_RCREG,0			    ;by subtracting the stored ADC value and subtracting 255
		BTFSS	STATUS,0			    ;Test if the ALU function resulted in a borrow by the carry/borrow being low (indicating a negative)	
		GOTO	STEP18				    ;\Negative number, ADC is currently in the Step 14 range
	
		MOVLW	0xF7				    ;/Test if the ADC is currently in step 14 or step 15,
		SUBWF	CURRENT_RCREG,0			    ;by subtracting the stored ADC value and subtracting 255
		BTFSS	STATUS,0			    ;Test if the ALU function resulted in a borrow by the carry/borrow being low (indicating a negative)	
		GOTO	STEP19				    ;\Negative number, ADC is currently in the Step 14 range

STEP0		
		MOVLW	0x05				    ;/Set the PW_COUNT to achieve a 500us PW
		MOVWF	PW_COUNT			    ;\
		GOTO	GOBACK				    ;Leave the ISR
STEP1
		MOVLW	0x06				    ;/Set the PW_COUNT to achieve a 625us PW
		MOVWF	PW_COUNT			    ;\
		GOTO	GOBACK				    ;Leave the ISR
STEP2
		MOVLW	0x07	    			    ;/Set the PW_COUNT to achieve a 750us PW
		MOVWF	PW_COUNT			    ;\
		GOTO	GOBACK				    ;Leave the ISR
STEP3
		MOVLW	0x08				    ;/Set the PW_COUNT to achieve a 875us PW
		MOVWF	PW_COUNT			    ;\
		GOTO	GOBACK				    ;Leave the ISR	
STEP4
		MOVLW	0x09				    ;/Set the PW_COUNT to achieve a 1ms PW
		MOVWF	PW_COUNT			    ;\
		GOTO	GOBACK				    ;Leave the ISR
STEP5
		MOVLW	0x0A				    ;/Set the PW_COUNT to achieve a 1.125ms PW
		MOVWF	PW_COUNT			    ;\
		GOTO	GOBACK				    ;Leave the ISR
STEP6
		MOVLW	0x0B				    ;/Set the PW_COUNT to achieve a 1.25ms PW
		MOVWF	PW_COUNT			    ;\
		GOTO	GOBACK				    ;Leave the ISR
STEP7
		MOVLW	0x0C				    ;/Set the PW_COUNT to achieve a 1.375ms PW
		MOVWF	PW_COUNT			    ;\
		GOTO	GOBACK				    ;Leave the ISR
STEP8
		MOVLW	0x0D				    ;/Set the PW_COUNT to achieve a 1.5ms PW
		MOVWF	PW_COUNT			    ;\
		GOTO	GOBACK				    ;Leave the ISR
STEP9
		MOVLW	0x0E				    ;/Set the PW_COUNT to achieve a 1.625ms PW
		MOVWF	PW_COUNT			    ;\
		GOTO	GOBACK				    ;Leave the ISR
STEP10
		MOVLW	0x0F				    ;/Set the PW_COUNT to achieve a 1.75ms PW
		MOVWF	PW_COUNT			    ;\
		GOTO	GOBACK				    ;Leave the ISR
STEP11
		MOVLW	0x10				    ;/Set the PW_COUNT to achieve a 1.875ms PW
		MOVWF	PW_COUNT			    ;\
		GOTO	GOBACK				    ;Leave the ISR
STEP12
		MOVLW	0x11				    ;/Set the PW_COUNT to achieve a 2ms PW
		MOVWF	PW_COUNT			    ;\
		GOTO	GOBACK				    ;Leave the ISR
STEP13
		MOVLW	0x12				    ;/Set the PW_COUNT to achieve a 2.125ms PW
		MOVWF	PW_COUNT			    ;\
		GOTO	GOBACK				    ;Leave the ISR
STEP14
		MOVLW	0x13				    ;/Set the PW_COUNT to achieve a 2.25ms PW
		MOVWF	PW_COUNT			    ;\
		GOTO	GOBACK				    ;Leave the ISR
STEP15
		MOVLW	0x14				    ;/Set the PW_COUNT to achieve a 2.375ms PW
		MOVWF	PW_COUNT			    ;\
		GOTO	GOBACK				    ;Leave the ISR
STEP16
		MOVLW	0x15				    ;/Set the PW_COUNT to achieve a 2.375ms PW
		MOVWF	PW_COUNT			    ;\
		GOTO	GOBACK				    ;Leave the ISR
STEP17
		MOVLW	0x16				    ;/Set the PW_COUNT to achieve a 2.375ms PW
		MOVWF	PW_COUNT			    ;\
		GOTO	GOBACK				    ;Leave the ISR
STEP18
		MOVLW	0x17				    ;/Set the PW_COUNT to achieve a 2.375ms PW
		MOVWF	PW_COUNT			    ;\
		GOTO	GOBACK				    ;Leave the ISR
STEP19
		MOVLW	0x18				    ;/Set the PW_COUNT to achieve a 2.375ms PW
		MOVWF	PW_COUNT			    ;\
		GOTO	GOBACK				    ;Leave the ISR