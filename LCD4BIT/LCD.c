/*
 *
 * I've let the delays a bit longer(you can shorten the delays and it would still work)
 * The ACK (external 32768 Hz crystal) can be unstable because of the enviroment
 *  so if you face problems use __delay_cycles(cycles) instead
 *                                                           (cycles = time in us)  if smclk~1MHz
 *__delays work only if mclk~1MHz
 */

#include <LCD.h>
#include"Delay.h"


void LCD4bit_Init(){

    //configure used Pins(see LCD.h for pin map)

    P6SEL  &= ~(0x08);
    P6DIR  |=   0xF7;
    P6OUT  &= ~(0x08);

    //~0.5 sec delay ( lcd boots slower that microcontroller)
    //    DELAY_TIMER_A2_ACLK(16384);
    DELAY_TIMER_A2_SMCLK(62500);

    LCD4bit_Cmd(0x02);         // Initialize Lcd in 4-bit mode
    LCD4bit_Cmd(0x28);         // enable 5x7 mode for chars
    LCD4bit_Cmd(0x0E);         // Display OFF, Cursor ON
    LCD4bit_Cmd(0x01);         // Clear Display
    LCD4bit_Cmd(0x80);         // Move the cursor to beginning of first line
    LCD4bit_Cmd(0x06);         // cursor increment
    LCD4bit_Cmd(0x01);         //clear display
    LCD4bit_Cmd(0x0F);         //turn on display without cursor (0x0F)->blinking cursor

    //on my display it won't work at powerup without these commands
    LCD4bit_Cmd(0x01);
    LCD4bit_Cmd(0x80);
    LCD4bit_Cmd(0x01);

}

void LCD4bit_Write_data_control(unsigned char data , unsigned char control){
    data &= 0xF0;           //get higher bits
    control &= 0x0F;        //get lower bits
    P6OUT = data + control ;
    P6OUT &= ~EN;

    //    DELAY_TIMER_A2_ACLK(4);         //~200us delay
    DELAY_TIMER_A2_SMCLK(25);         //~200us delay

    //Pulse on En Pin to confirm command
    P6OUT |= EN;

    //    DELAY_TIMER_A2_ACLK(4);         //~200us delay
    DELAY_TIMER_A2_SMCLK(25);         //~200us delay

    P6OUT &= ~EN;

    //    DELAY_TIMER_A2_ACLK(4);         //~200us delay
    DELAY_TIMER_A2_SMCLK(25);         //~200us delay

    P6OUT = data;

}

void LCD4bit_Cmd(unsigned char command){               //RS=0 -> send command

    LCD4bit_Write_data_control(command&(0xF0), 0);     //upper 4 bits   RS=0(command register)
    LCD4bit_Write_data_control(command<<4, 0);         //lower 4 bits RS=0(command register)

    if(command<4){                                      // first commands require more time
//        DELAY_TIMER_A2_ACLK(40);                                //~2000us delay
        DELAY_TIMER_A2_SMCLK(250);                                //~2000us delay

    }
    else     __delay_cycles(40);                        //~40us  at 1MHz Mclk
}

void LCD4bit_Data(unsigned char data){                 //RS=1 -> send data

    LCD4bit_Write_data_control(data&(0xF0), 1);        //upper 4 bits  RS=1(data register)
    LCD4bit_Write_data_control(data<<4, 1);            //lower 4 bits  RS=1(data register)
    __delay_cycles(40);                                 //~40us  at 1MHz Mclk
}

void LCD4bit_SendString(char *buff){
    while(*buff!='\0'){

        LCD4bit_Data(*buff);
        buff++;

    }
}


void LCD4bit_Cursor_Position(int row , int column){
    LCD4bit_Cmd(0x02);                     //return to position 0 ,0
    if(row==1) LCD4bit_Cmd(0xC0);          //Row 1

    while(column>0){
        LCD4bit_Cmd(0x14);                 //Shift right by 1
        column--;
    }

}




inline void LCD4bit_Clear(){
    LCD4bit_Cmd(0x01); // Clear Display
}

inline void LCD4bit_Cursor_Blink(){
    LCD4bit_Cmd(0x0F); // Blink cursor

}

inline void LCD4bit_Cursor_StopBlink(){
    LCD4bit_Cmd(0x0E); // Don't blink cursor

}

inline void LCD4bit_Cursor_On(){
    LCD4bit_Cmd(0x0E); // Cursor On

}

inline void LCD4bit_Cursor_Off(){
    LCD4bit_Cmd(0x0C); // Cursor Off

}



