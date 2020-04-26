/*
 *
 * Inspired by malichao
 *
 *
 */
#include"msp430.h"
#ifndef I2C_H_
#define I2C_H_

#define SDA BIT0
#define SCL BIT1
#define I2C_PIN_MASK SDA+SCL
#define I2C_PIN_SEL P3SEL

#define ACK 0x00
#define NACK 0xFF



//unsigned char *PTxData;                     // Pointer to TX data
//unsigned char TXByteCtr;
//unsigned char sending;
//unsigned char *PRxData;                     // Pointer to RX data
//unsigned char RXByteCtr;
//volatile unsigned char RxBuffer[10];                 // Allocate 128 byte of RAM


void I2C_PIN_INIT() ;

void I2C_INIT();

void I2C_ENABLE_RX_TX_INTERRUPT();

void I2C_DISABLE_RX_TX_INTERRUPT();
void I2C_SET_ADDRESS(unsigned char address);

void I2C_START_W();
void I2C_START_R();

void I2C_STOP();
void I2C_SEND_DATA(unsigned char data);

unsigned char I2C_CHECK_ACK();


void I2C_WRITE_BYTE(unsigned char data );
void I2C_WRITE_BYTES(unsigned char *data , unsigned char lenght);
unsigned char I2C_READ_BYTE();


/*
*
*
*functions that don't use interrupts
*
*
*
*/
unsigned char I2C_WRITE_BYTE_WITHOUT_INTERRUPT(unsigned char data);
unsigned char I2C_WRITE_BYTES_WITHOUT_INTERRUPT_UNKNOWN_LENGHT(unsigned char *data);
unsigned char I2C_WRITE_BYTES_WITHOUT_INTERRUPT_KNOWN_LENGHT(unsigned char *data , unsigned char lenght);

unsigned char I2C_READ_BYTE_WITHOUT_INTERRUPT();


#endif /* I2C_H_ */
