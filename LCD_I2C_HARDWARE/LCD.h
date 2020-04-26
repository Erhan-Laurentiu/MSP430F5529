/*
 *Library for LCD 16x2  4bit mode
 *
 *LCD pins
 *
 *P0-P7 are PCF8574 pins
 *
 * RS = P0
 * RW = P1
 * EN = P2
 *
 * D4 = P4
 * D5 = P5
 * D6 = P6
 * D7 = P7
 *
 * BackLight = P3
 *
 */

#include<msp430.h>

#ifndef LCD_H_
#define LCD_H_

#define RS 0x01
#define RW 0x02
#define EN 0x04

void LCD4bit_Init();
void LCD4bit_Write_data_control(unsigned char data , unsigned char control);
void LCD4bit_Cmd(unsigned char command);
void LCD4bit_Data(unsigned char data);
void LCD4bit_SendString(char *buff);
void LCD4bit_Cursor_Position(int row , int column);
inline void LCD4bit_Clear();
inline void LCD4bit_Cursor_Blink();
inline void LCD4bit_Cursor_StopBlink();
inline void LCD4bit_Cursor_On();
inline void LCD4bit_Cursor_Off();


#endif /* LCD_H_ */
