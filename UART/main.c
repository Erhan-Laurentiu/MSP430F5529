#include <msp430.h>
#include "UART0.h"
#include "UART1.h"


int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
    char a[]= "Hello again";
    __delay_cycles(100000); //Let the smclk to settle at startup

    UART1_Init();
    UART1_SendString_UnknownLenght("Hello World\r\n");
    UART1_SendInt(1234);
    UART1_SendNewLine();
    UART1_SendString_KnownLenght(a,11);
    UART1_SendNewLine();

//    UART0_Init();
//    UART0_SendString_UnknownLenght("Hello World");
//    UART0_SendChar('A');
//    UART0_SendInt(1234);
//    UART0_SendString_KnownLenght(a,11);

}
