/*
 *
 *Inspired by malichao
 *
 *
 */

#include"I2C.h"

#define SDA BIT0
#define SCL BIT1
#define I2C_PIN_MASK SDA+SCL
#define I2C_PIN_SEL P3SEL

#define ACK 0x00
#define NACK 0xFF


unsigned char *PTxData;                     // Pointer to TX data
unsigned char TXByteCtr;
unsigned char *PRxData;                     // Pointer to RX data
unsigned char RXByteCtr;
volatile unsigned char RxBuffer[10];                 // Allocate 128 byte of RAM



void I2C_PIN_INIT(){

    P3SEL |= SCL + SDA;

}

void I2C_INIT(){

    I2C_PIN_INIT();

    UCB0CTL1 |= UCSWRST; //Configuration mode I2C

    UCB0CTL0 = UCMST+ UCMODE_3 + UCSYNC; // Master mode I2C Synchronous mode
    UCB0CTL1 = UCSSEL_2 + UCSWRST;       //SMCLK keep configuration mode
    UCB0BR0 = 12;    // I2C speed = SMCLK/12 ~ 100KHz
    UCB0BR1 = 0;

    UCB0CTL1 &= ~UCSWRST; //Exit configuration mode I2C

}

void I2C_ENABLE_RX_TX_INTERRUPT(){
    UCB0IE |= UCTXIE + UCRXIE;                         // Enable RX and TX interrupt

}

void I2C_DISABLE_RX_TX_INTERRUPT(){
    UCB0IE &= ~(UCTXIE + UCRXIE);                         // Enable RX and TX interrupt

}

void I2C_SET_ADDRESS(unsigned char address){

    UCB0I2CSA = address;

}

void I2C_START_W(){

    while(UCB0CTL1&UCTXSTP);// Ensure stop condition got send
    UCB0CTL1 |= UCTXSTT + UCTR; //send START condition I2C transmission mode

}

void I2C_START_R(){

    while(UCB0CTL1&UCTXSTP);// Ensure stop condition got send
    UCB0CTL1 &= ~UCTR;   // I2C receiver mode
    UCB0CTL1 |= UCTXSTT; //send START condition
}


void I2C_STOP(){
    UCB0CTL1 |= UCTXSTP; //send STOP condition
}

void I2C_SEND_DATA(unsigned char data){

    UCB0TXBUF = data;
}

unsigned char I2C_READ_DATA(){

    while(!(UCB0IFG&UCRXIFG));          //wait until UCB0RXIFG is set ( when received a full character)
    return UCB0RXBUF;

}



void I2C_WRITE_BYTES(unsigned char *data , unsigned char lenght){
    I2C_ENABLE_RX_TX_INTERRUPT();

    PTxData = data; //pointerTX start address
    TXByteCtr = lenght;
    I2C_START_W();

    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM3 w/ interrupt (CPU->OFF)

    while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent

}



unsigned char I2C_READ_BYTE()
{
    I2C_ENABLE_RX_TX_INTERRUPT();
    PRxData = (unsigned char *)RxBuffer;    // Start of RX buffer
    RXByteCtr = 1;                          // Load RX byte counter
    UCB0CTL1 &= ~UCTR;
    UCB0CTL1 |= UCTXSTT;                    // I2C start condition
    while(UCB0CTL1 & UCTXSTT);              // Start condition sent?
    UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
    while(UCB0CTL1 & UCTXSTT);              // Start condition sent?
    while(RXByteCtr);
    return RxBuffer[0];
}



#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_B0_VECTOR))) USCI_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(UCB0IV,12))
    {
    case  0: break;                           // Vector  0: No interrupts
    case  2: break;                           // Vector  2: ALIFG
    case  4:
    case  6: break;                           // Vector  6: STTIFG
    case  8: break;                           // Vector  8: STPIFG
    case 10:                                 // Vector 10: RXIFG
        RXByteCtr--;                            // Decrement RX byte counter
        if (RXByteCtr)
        {
            *PRxData++ = UCB0RXBUF;               // Move RX data to address PRxData
            if (RXByteCtr == 1)                   // Only one byte left?
                UCB0CTL1 |= UCTXSTP;                // Generate I2C stop condition
        }
        else
        {
            *PRxData = UCB0RXBUF;                 // Move final RX data to PRxData
            //__bic_SR_register_on_exit(LPM0_bits); // Exit active CPU
        }
    case 12:                                  // Vector 12: TXIFG
        if (TXByteCtr)                          // Check TX byte counter
        {
            UCB0TXBUF = *PTxData++;               // Load TX buffer
            TXByteCtr--;                          // Decrement TX byte counter
        }
        else
        {
            //        if(stop)
            UCB0CTL1 |= UCTXSTP;                  // I2C stop condition

            if(RXByteCtr==0) UCB0IFG &= ~UCRXIFG;                  // Clear USCI_B0 TX int flag

            UCB0IFG &= ~UCTXIFG;                  // Clear USCI_B0 TX int flag
            __bic_SR_register_on_exit(LPM0_bits);   //Exit LPM3 (CPU->ON)

        }

        break;
    default: break;

    }
}

/*
 *
 *
 *functions that don't use interrupts
 *
 *
 *
 */


unsigned char I2C_CHECK_ACK(){

    while(!(UCB0IFG & UCTXIFG));         //wait until UCB0TXBUF is empty (finish transmitting data)
    if(UCB0IFG & UCNACKIFG)    return NACK;   // received NACK
    else                        return ACK;  // received ACK
}




unsigned char I2C_WRITE_BYTE_WITHOUT_INTERRUPT(unsigned char data){

    I2C_DISABLE_RX_TX_INTERRUPT();      //disable interrupts

    I2C_START_W(); //Start transmission in write mode

    if(I2C_CHECK_ACK()==NACK) return NACK; //check if the slave acknowledged(if there is any slave with that address)

    I2C_SEND_DATA(data);   //send data in TXBUFF

    if(I2C_CHECK_ACK()==NACK) return NACK; //check if the slave acknowledged(if there is any slave with that address)

    I2C_STOP();     //Stop transmission

    return ACK;

}

unsigned char I2C_WRITE_BYTES_WITHOUT_INTERRUPT_UNKNOWN_LENGHT(unsigned char *data){

    I2C_DISABLE_RX_TX_INTERRUPT();      //disable interrupts

    I2C_START_W(); //Start transmission in write mode

    //    if(I2C_CHECK_ACK()==NACK) return NACK; //check if the slave acknowledged(if there is any slave with that address)

    while(*data != '\0'){
        if(I2C_CHECK_ACK()==NACK) return NACK;//check if the slave acknowledged/if there is any slave with that address

        I2C_SEND_DATA(*data);//send data

        data++;

    }

    if(I2C_CHECK_ACK()==NACK) return NACK; //check if the slave acknowledged/if there is any slave with that address

    I2C_STOP();                    //send STOP condition

    return ACK;
}


unsigned char I2C_WRITE_BYTES_WITHOUT_INTERRUPT_KNOWN_LENGHT(unsigned char *data , unsigned char lenght){

    I2C_DISABLE_RX_TX_INTERRUPT();      //disable interrupts

    I2C_START_W(); //Start transmission in write mode

    //    if(I2C_CHECK_ACK()==NACK) return NACK; //check if the slave acknowledged(if there is any slave with that address)

    while(lenght != 0){
        if(I2C_CHECK_ACK()==NACK) return NACK;//check if the slave acknowledged/if there is any slave with that address

        I2C_SEND_DATA(*data);//send data

        data++;
        lenght--;
    }

    if(I2C_CHECK_ACK()==NACK) return NACK; //check if the slave acknowledged/if there is any slave with that address

    I2C_STOP();                    //send STOP condition

    return ACK;
}



unsigned char I2C_READ_BYTE_WITHOUT_INTERRUPT(){

    unsigned char data=0;


    I2C_DISABLE_RX_TX_INTERRUPT();      //disable interrupts

    I2C_START_R(); //Start transmission in write mode

    while(!(UCB0IFG&UCRXIFG));

    I2C_STOP();     //Stop transmission

    data=UCB0RXBUF;

    return data;
}

