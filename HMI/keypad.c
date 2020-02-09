/*
 * keypad.c
 *
 *  Created on: Dec 29, 2019
 *      Author: ahmed
 */
#include "keypad.h"
#if(N_COL == 4)
static uint8 Keypad_4X4_adjustSwitchNumber(uint8 digit);
#else
static uint8 Keypad_4X3_adjustSwitchNumber(uint8 digit);
#endif
uint8 Keypad_getPressedKey(void)
{
	while(1)
	{
		for(uint8 col = 0; col < N_COL ; col++)
		{
			KEYPAD_PORT_DIR 	= 	(0b00010000 << col);
			KEYPAD_PORT 		=	(~(0b00010000<<col));
			for(uint8 row = 0; row < N_ROW ; row++)
			{
				#if(N_COL == 4)

				if(BIT_IS_CLEAR(KEYPAD_PORT_PIN,row))
					return Keypad_4X4_adjustSwitchNumber(row*N_COL+col+1);
				#else
				if(BIT_IS_CLEAR(KEYPAD_PORT_PIN,row))
					return Keypad_4X3_adjustSwitchNumber(row*N_COL+col+1);
				#endif

			}
		}
	}
}
#if(N_COL == 4)
static uint8 Keypad_4X4_adjustSwitchNumber(uint8 digit)
{
	uint8 res;
		switch(digit)
		{
		case 1:
			res = 7;
			break;
		case 2:
			res = 8;
			break;
		case 3:
			res = 9;
			break;
		case 4:
			res = '#';
			break;
		case 5:
			res = 4;
			break;
		case 6:
			res = 5;
			break;
		case 7:
			res = 6;
			break;
		case 8:
			res = '*';
			break;
		case 9:
			res = 1;
			break;
		case 10:
			res = 2;
			break;
		case 11:
			res = 3;
			break;
		case 12:
			res = '-';
			break;
		case 13:
			res = 13;
			break;
		case 14:
			res = 0;
			break;
		case 15:
			res = '=';
			break;
		case 16:
			res = '+';
			break;
		}
		return res;
}
#else
static uint8 Keypad_4X3_adjustSwitchNumber(uint8 digit)
{
	uint8 res;
	switch(digit)
	{
	case '10':
		res = '*';
		break;
	case '11':
		res = '0';
		break;
	case'12':
		res = '#';
		break;
	default:
		res = digit;
	}
	return res;
}
#endif
