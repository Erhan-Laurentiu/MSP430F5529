/*
 *
 * Inspired by :  Shawon Shahryiar from
 * http://embedded-lab.com/blog/introducing-ti-msp430-microcontrollers/
 *
 *
 */

#include"SoftwareI2C.h"


void I2C_SOFTWARE_INIT(){
    P3SEL&= ~(BIT0+BIT1);
    SCL_DIR_OUT;
    SDA_DIR_OUT;
    SCL_HIGH;
    SDA_HIGH;
}

void I2C_SOFTWARE_START(){
    I2C_SOFTWARE_INIT();                    //SCL,SDA=OUTPUT & HIGH
    __delay_cycles(100);
    SDA_LOW;
    __delay_cycles(100);
    SCL_LOW;
}

void I2C_SOFTWARE_STOP(){
    SDA_DIR_OUT;
    SCL_LOW;
    SDA_LOW;
    __delay_cycles(10);
    SCL_HIGH;
    __delay_cycles(10);
    SDA_HIGH;
    __delay_cycles(10);

}

char I2C_SOFTWARE_WAIT_ACK(){

    int counter=1000;

    SDA_DIR_IN;
    SDA_HIGH;                           //input pulled high
    __delay_cycles(10);

    SCL_HIGH;

    while(I2C_IN&SDA != 0){             //SDA pulled low->ACK
        counter--;
        if(counter == 0 ) {             //Didn't received ACK -> send NACK
            SDA_DIR_OUT;
            SDA_HIGH;
            SCL_LOW;
            I2C_SOFTWARE_STOP();        //Stop transmission
            return NACK;                //Nobody picked up
        }
    }

    SCL_LOW;

    return ACK;                         //Continue Transfering
}


void I2C_SOFTWARE_WRITE(int data ,int readOrWrite){
    //  read=1        write=0
    int i =8;

    if (readOrWrite==1) data=data+1;

    SDA_DIR_OUT;
    SCL_LOW;

    while(i>0){

        //check if transmitted bit(msb) is 0

        if (((data & 0x80)>>7) != 0x00 )    SDA_HIGH;   //msb is 1

        else SDA_LOW;    //msb is 0

        data=data<<1 ; //get rid of transmited bit, msb is now next bit

        //secure bit
        __delay_cycles(10);
        SCL_HIGH;
        __delay_cycles(10);
        SCL_LOW;
        __delay_cycles(10);

        i--;

    }
}



char I2C_SOFTWARE_WRITE_SEQUENCE(int address,int data){

    I2C_SOFTWARE_START();

    I2C_SOFTWARE_WRITE(address,0);

    if(I2C_SOFTWARE_WAIT_ACK() == NACK) return NACK; //NACK

    I2C_SOFTWARE_WRITE(data,0);

    if(I2C_SOFTWARE_WAIT_ACK() == NACK) return NACK; //NACK

    I2C_SOFTWARE_STOP();


    return 0; //there are no errors
}

unsigned char I2C_SOFTWARE_READ(unsigned char ackOrNack)
// NACK=0xFF
// ACK =0x00
{
    unsigned char i = 8;
    unsigned char j = 0;

    SDA_DIR_IN;

    while(i > 0)
    {
        SCL_LOW;
        __delay_cycles(2);
        SCL_HIGH;
        __delay_cycles(2);

        j <<= 1;

        if(I2C_IN&SDA != 0x00)    j++;
        __delay_cycles(2);

        i--;
    }

    //do we want to receive more data (ACK=0x00) or to stop receiving data(NACK=0xFF)

    ackOrNack == ACK ? I2C_SOFTWARE_SEND_ACK_NACK(ACK) : I2C_SOFTWARE_SEND_ACK_NACK(NACK);

    return j;
}


void I2C_SOFTWARE_SEND_ACK_NACK(unsigned char ackOrNack)

{
    SCL_LOW;
    SDA_DIR_OUT;

    if(ackOrNack == ACK) SDA_LOW;
    else  SDA_HIGH;

    //secure bit
    __delay_cycles(3);
    SCL_HIGH;
    __delay_cycles(3);
    SCL_LOW;
}
