#include <LCD.h>
#include <msp430.h> 
#include"Delay.h"


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer
    __delay_cycles(1000000);
    LCD4bit_Init();

    LCD4bit_SendString("Hello");
    LCD4bit_Cursor_Position(1,4);
    LCD4bit_SendString("World");

    DELAY_TIMER_A2_ACLK(65535);             //~2sec
    LCD4bit_Clear();
    LCD4bit_Cursor_Off();

    DELAY_TIMER_A2_ACLK(65535);             //~2sec
    LCD4bit_SendString("Line 1");
    LCD4bit_Cursor_Position(1,4);
    LCD4bit_SendString("Line 2");
    P1DIR|=BIT0;
    P1OUT|=BIT0;


    __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3 w/ interrupt

    return 0;
}



