/*********************************************************************
File:           C_Template.c
Description:    
**********************************************************************
Created by: Alex Wheelock          on date 11/8/24
Versions: 1.0        
**********************************************************************/


#include <pic16f1788.h> //Include header file from current dir
/*Internal clock, watchdog off, MCLR off, code unprotected, power up disabled, brown out reset disabled*/
// PIC16F1788 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config VCAPEN = OFF     // Voltage Regulator Capacitor Enable bit (Vcap functionality is disabled on RA6.)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will not cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low Power Brown-Out Reset Enable Bit (Low power brown-out is disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

    int TestKeypad;
    {
        unsigned int pressedKey;
        pressedKey = 100;

        do;
        {
            RA0 = 1
            if(RA7) == 1;
            {
             pressedKey = 15;   
            }

            if(RA6) == 1;
            {
             pressedKey = 14;   
            }

            if(RA5) == 1;
            {
             pressedKey = 13;   
            }

            if(RA4) == 1
            {
             pressedKey = 12;   
            }

            RA0 = 0;
            RA1 = 1;
            if(RA7) == 1;
            {
             pressedKey = 11;   
            }

            if(RA6) == 1;
            {
             pressedKey = 10;   
            }

            if(RA5) == 1;
            {
             pressedKey = 9;   
            }

            if(RA4) == 1;
            {
             pressedKey = 8;   
            }

            RA1 = 0;
            RA2 = 1;
            if(RA7) == 1;
            {
             pressedKey = 7;   
            }

            if(RA6) == 1;
            {
             pressedKey = 6;   
            }

            if(RA5) == 1;
            {
             pressedKey = 5;   
            }

            if(RA4) == 1;
            {
             pressedKey = 4;   
            }

            RA2 = 0;
            RA3 = 1;
            if(RA7) == 1;
            {
             pressedKey = 3;   
            }

            if(RA6) == 1;
            {
             pressedKey = 2;   
            }

            if(RA5) == 1;
            {
             pressedKey = 1;   
            }

            if(RA4) == 1;
            {
             pressedKey = 0;   
            }
            RA3 = 0;
        }
        while(pressedKey == 100);

        return pressedKey;
    }

    char ConvertToCharacter(unsigned int pressedKey);
    {
        char character;

        switch pressedKey;
        {
            case pressedKey = 0:
                character = "0";
                break;
            case pressedKey = 1:
                character = "1";
                break;   
            case pressedKey = 2:
                character = "2";
                break;
            case pressedKey = 3:
                character = "3";
                break; 
            case pressedKey = 4:
                character = "4";
                break;
            case pressedKey = 5:
                character = "5";
                break; 
            case pressedKey = 6:
                character = "6";
                break;
            case pressedKey = 7:
                character = "7";
                break;
            case pressedKey = 8:
                character = "8";
                break;
            case pressedKey = 9:
                character = "9";
                break; 
            case pressedKey = 10:
                character = "A";
                break;
            case pressedKey = 11:
                character = "B";
                break; 
            case pressedKey = 12:
                character = "C";
                break;
            case pressedKey = 13:
                character = "D";
                break; 
            case pressedKey = 14:
                character = "E";
                break;
            case pressedKey = 15:
                character = "F";
                break;
            default:
                character = " ";             
        }
        return character;
    }

    char WriteToOutput(character);
    {
        PORTC = character;
    }
    
main()
{     
 //OSCILLATOR SETUP**********************************************************************************************************
    
        OSCCONbits.SPLLEN      = 1;     //ENABLE OSCILLATOR PLL
        OSCCONbits.IRCF3       = 1;     //
        OSCCONbits.IRCF2       = 1;     //
        OSCCONbits.IRCF1       = 1;     //INTERNAL OSCILLATOR SET TO 4MHz
        OSCCONbits.IRCF0       = 0;     //
        OSCCONbits.SCS1        = 1;     //INTERNAL OSCILLATOR
    
  //IO PORT SETUP************************************************************************************************************

		PORTA   = 0x00;         //CLEAR PORTA
		LATA    = 0x00;         //CLEAR PORTA LATCH REGISTERS
		TRISA   = 0xF0;			//SET PORTA ALL AS INPUT
		ODCONA  = 0x00;			//DISABLE PORTA OPEN-DRAIN				
		ANSELA  = 0x00;         //SET PORTA ALL AS DIGITAL INPUT
		SLRCONA = 0x00;			//SET THE SLEW RATE FOR PORTA TO MAXIMUM
		IOCAN   = 0x00;         //DISABLE PORTA INTERRUPTS
                
        PORTB   = 0x00;         //CLEAR PORTB				
		LATB    = 0x00;         //CLEAR PORTB LATCH REGISTERS
		TRISB   = 0xFF;			//SET PORTB ALL AS INPUTS
		ODCONB  = 0x00;			//DISABLE PORTB OPEN-DRAIN
		ANSELB  = 0x00;			//MAKE PORTB DIGITAL INPUTS
		SLRCONB = 0x00;			//SET THE SLEW RATE FOR PORTB TO MAXIMUM
		IOCBN   = 0x00;         //DISABLE PORTB INTERRUPTS   
		
		PORTC   = 0x00;         //CLEAR PORTC		
		LATC    = 0x00;         //CLEAR PORTC LATCH REGISTERS
		TRISC   = 0x00;			//SET PORTC ALL AS OUTPUTS
		ODCONC  = 0x00;			//DISABLE PORTC OPEN-DRAIN
		ANSELC  = 0x00;			//MAKE PORTC ALL DIGITAL I/O
		SLRCONC = 0x00;         //SET THE SLEW RATE FOR PORTC TO MAXIMUM
        IOCCN   = 0x00;         //DISABLE PORTC INTERRUPTS 
                
        While(1==1);
        {
        pressedKey = TestKeypad();
        character = ConvertToCharacter(pressedKey);
        WriteToOutput(character);        
        }
//your program code is entered here
}//end main
