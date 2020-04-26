#include "PCF8574.h"
#include "I2C.h"

void PCF8574_WRITE(unsigned char data)
{
    I2C_SET_ADDRESS(PCF8574_actual_address);

    I2C_WRITE_BYTE_WITHOUT_INTERRUPT(data);


    //      you can use this function if you want to use interrupts
    //    unsigned char a[]={data};
    //    I2C_WRITE_BYTES(a,1); //you can use this if you want to use interrupts

}



