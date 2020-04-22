#include "PCF8574.h"
#include "SoftwareI2C.h"
unsigned char PCF8574_read(void)
{
    unsigned char data = 0x00;

    I2C_SOFTWARE_START();
    I2C_SOFTWARE_WRITE(PCF8574_address,1);
    if(I2C_SOFTWARE_WAIT_ACK() == NACK) return NACK; //NACK
    //    data = I2C_SOFTWARE_READ(0x00); // this could be used if we wanted to send ACK and receive the data again
    data = I2C_SOFTWARE_READ(0x01); //0x01 = NACK
    I2C_SOFTWARE_STOP();

    return data;
}

void PCF8574_write(unsigned char data_byte)
{
    I2C_SOFTWARE_WRITE_SEQUENCE(PCF8574_address,data_byte);
}
