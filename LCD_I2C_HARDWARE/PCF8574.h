//#include "SW_I2C.h"
#include "I2C.h"

#define PCF8574_address                 0x4E                    //actual address is 0x27 ( 0x4E = 0x27 <<1 )
#define PCF8574_actual_address          0x27                    //actual address is 0x27 ( 0x4E = 0x27 <<1 )

//unsigned char PCF8574_read(void);

void PCF8574_WRITE(unsigned char data);
