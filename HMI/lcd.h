/*
 * lcd.h
 *
 *  Created on: Dec 29, 2019
 *      Author: ahmed
 */

#ifndef LCD_H_
#define LCD_H_
#include "std_Types.h"
#include "micro_config.h"
#include "common_macros.h"

//#define FOUR_BIT_MODE

#define LCD_CONTROL_PORT	PORTD
#define LCD_CONTROL_DIR		DDRD

#define LCD_DATA_PORT		PORTC
#define LCD_DATA_DIR  		DDRC

#define RS	4
#define RW	5
#define E	6

#define TWO_LINE_LCD_Eight_BIT_MODE		0x38
#define TWO_LINE_LCD_FOUR_BIT_MODE		0x28
#define SWITCH_TWO_FOUR_BIT_MODE		0x02
#define CURSOR_OFF						0x0c
#define CLEAR_COMMAND 					0x01

#define CURSOR_LOCATION  0x80

void LCD_init(void);
void LCD_displayCharacter(uint8 character);
void LCD_displayString(uint8 *str);
void LCD_goToRowColoumn(uint8 row, uint8 col);
void LCD_displayStringRowColoumn(uint8 row, uint8 col, uint8 *str);
void LCD_clearScreen(void);
void LCD_integerToString(uint32 val);

#endif /* LCD_H_ */
