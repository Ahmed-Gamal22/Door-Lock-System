/*
 * lcd.c
 *
 *  Created on: Dec 29, 2019
 *      Author: ahmed
 */
#include "lcd.h"
static void LCD_sendCommand(uint8 command);
void LCD_init(void)
{
	/*data port direction output*/
#ifdef FOUR_BIT_MODE
	LCD_DATA_DIR |= 0x0f;
#else
	LCD_DATA_DIR = 0xff;
#endif
	/*control pins are output*/
	LCD_CONTROL_DIR |=(1<<RS)|(1<<RW)|(1<<E);
#ifdef FOUR_BIT_MODE
	LCD_sendCommand(SWITCH_TWO_FOUR_BIT_MODE);
	LCD_sendCommand(TWO_LINE_LCD_FOUR_BIT_MODE);
#else
	LCD_sendCommand(TWO_LINE_LCD_Eight_BIT_MODE); /* use 2-line lcd + 8-bit Data Mode + 5*7 dot display Mode */
#endif
	LCD_sendCommand(CURSOR_OFF); /* cursor off */
	LCD_sendCommand(CLEAR_COMMAND); /* clear LCD at the beginning */
}
static void LCD_sendCommand(uint8 command)
{
	/*RS=0 transfer instruction data*/
	CLEAR_BIT(LCD_CONTROL_PORT,RS);
	/*RW=0 write mode selected*/
	CLEAR_BIT(LCD_CONTROL_PORT,RW);
	_delay_ms(1);
	SET_BIT(LCD_CONTROL_PORT,E);
	_delay_ms(1);
#ifdef FOUR_BIT_MODE
	LCD_DATA_PORT = (LCD_DATA_PORT&0xf0)|((command>>4)&0x0f);
	_delay_ms(1);
	CLEAR_BIT(LCD_CONTROL_PORT,E);
	_delay_ms(1);
	SET_BIT(LCD_CONTROL_PORT,E);
	_delay_ms(1);
	LCD_DATA_PORT = (LCD_DATA_PORT&0xf0)|(command&0x0f);
#else
	LCD_DATA_PORT = command;
#endif
	_delay_ms(1);
	CLEAR_BIT(LCD_CONTROL_PORT,E);
	_delay_ms(1);
}
void LCD_displayCharacter(uint8 character)
{
	/*RS=1 transfer display data*/
	SET_BIT(LCD_CONTROL_PORT,RS);
	/*RW=0 write mode selected*/
	CLEAR_BIT(LCD_CONTROL_PORT,RW);
	_delay_ms(1);
	SET_BIT(LCD_CONTROL_PORT,E);
	_delay_ms(1);
#ifdef FOUR_BIT_MODE
	LCD_DATA_PORT = (LCD_DATA_PORT&0xf0)|((character>>4)&0x0f);
	_delay_ms(1);
	CLEAR_BIT(LCD_CONTROL_PORT,E);
	_delay_ms(1);
	SET_BIT(LCD_CONTROL_PORT,E);
	_delay_ms(1);
	LCD_DATA_PORT = (LCD_DATA_PORT&0xf0)|(character&0x0f);
#else
	LCD_DATA_PORT = character;
#endif
	_delay_ms(1);
	CLEAR_BIT(LCD_CONTROL_PORT,E);
	_delay_ms(1);
}
void LCD_displayString(uint8 *str)
{
	while(*str != '\0')
	{
		LCD_displayCharacter(*str);
		str++;
	}
}
void LCD_goToRowColoumn(uint8 row, uint8 col)
{
	uint8 lineNum;
	switch(row)
	{
	case 0:
		lineNum = col;
		break;
	case 1:
		lineNum = col + 0x40;
		break;
	case 2:
		lineNum = col + 0x10;
		break;
	case 3:
		lineNum = col + 0x50;
	}
	LCD_sendCommand(lineNum|CURSOR_LOCATION);
}
void LCD_displayStringRowColoumn(uint8 row, uint8 col, uint8 *str)
{
	LCD_goToRowColoumn(row,col);
	LCD_displayString(str);
}
void LCD_clearScreen(void)
{
	LCD_sendCommand(CLEAR_COMMAND); /* clear LCD at the beginning */
}
void LCD_integerToString(uint32 val)
{
	char buff[16];
	itoa(val,buff,10);
	LCD_displayString(buff);
}