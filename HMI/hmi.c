/*
 * hmi.c
 *
 *  Created on: Feb 1, 2020
 *      Author: ahmed
 *
 * HMI(human machine interface) micro used for interfacing with the user
 * of the application using keypad and LCD.
 *       	     |---------------|
 *				 | 7 | 8 | 9 | # |
 *				 |---------------|
 *				 | 4 | 5 | 6 | * |				|----------------------|
 *				 |---------------|   ------>    |   Enter Sys Pass:    |
 *				 | 1 | 2 | 3 |   |				|   *****              |
 *				 |---------------|				|----------------------|
 *				 |-> | 0 |   |   |
 *				 |---------------|
 *
 * These Are the used Buttons in the 4*4 keypad.
 */
#include "uart.h"
#include "lcd.h"
#include "keypad.h"
#include "timer.h"
/*macros used to synch between two micros using UART module*/
#define M2_READY 0x10
#define M1_READY 0x01
#define STEP1	0x02
#define STEP2	0x03
#define STEP3	0x03
#define OPEN	0x04
#define CLOSE	0x05
#define MAIN_OPTION 0x06
#define FAULT_OCCURANCE 0x11

#define PASS_NUM_DIGITS 	5
#define FIFTEEN_SEC_DELAY 	15
#define ONE_MIN_DELAY 		60
#define ENTERKEY_ASCII_VAL	13

/*********************************************************************
 *[Function Name]: userPassword  									 *
 *[Description]: This function is responsible for taking new password*
 *				 from user through 4*4 keypad followed by enter key. *
 *[In]: void														 *
 *[Out]: void														 *
 ********************************************************************/
void userPassword(void)
{
	uint8 pass[PASS_NUM_DIGITS]; //variable to save 5 digits password
	uint8 enterkey;
	LCD_goToRowColoumn(1,0);
	/* taking pass from keypad */
	for(uint8 i = 0 ; i < PASS_NUM_DIGITS ; i++)
	{
		pass[i] = Keypad_getPressedKey();
		LCD_displayCharacter('*');
		_delay_ms(500);
	}

	/* you should press enter */
	do{
		enterkey = Keypad_getPressedKey();
		_delay_ms(500);
	}while(enterkey != ENTERKEY_ASCII_VAL);

	/* sending password to control micro*/
	for(uint8 i = 0 ; i < PASS_NUM_DIGITS ; i++)
	{
		/* sending password to control micro*/
		UART_sendByte(pass[i]);
	}
}

int main()
{
	uint8 uartdata; //carries all uart synch messages
	uint8 keydata;  // carries main option data '*' or '#'
	/*Initialization*/
	LCD_init();
	UART_init();
	/* shaking hands between two micros */
	UART_sendByte(M1_READY); // MC1 is ready
	while(UART_recieveByte() != M2_READY){} // wait until MC2 is ready
	/* Super Loop */
	while(1)
	{
		/* step 1 & step 2 used for taking pass from user two times
		 * and send it to the second micro
		 * it only happens one time at the
		 * first running of the application.
		 * */
		uartdata = UART_recieveByte(); //synch step1
		if(uartdata == STEP1)
		{
			LCD_clearScreen();
			LCD_displayString("Enter new Pass:");
			userPassword();
			uartdata = UART_recieveByte(); //synch step2
			if(uartdata == STEP2)
			{
				LCD_clearScreen();
				LCD_displayString("Enter new Pass:");
				userPassword();
			}
		}
		/*step 3 used in the main application features and didn't back*/
		else if(uartdata == STEP3)
		{
			/*main application super loop*/
			while(1)
			{
				uartdata = UART_recieveByte(); //synch step3(main app)
				if(uartdata == MAIN_OPTION)
				{
					LCD_clearScreen();
					LCD_displayString("*:open the door.");
					LCD_goToRowColoumn(1,0);
					LCD_displayString("#:change password.");
					keydata = Keypad_getPressedKey();
					_delay_ms(500);
					switch (keydata)
					{
						case '*':
							UART_sendByte('*');
							LCD_clearScreen();
							LCD_displayString("Enter password");
							userPassword();
							break;
						case '#':
							UART_sendByte('#');
							LCD_clearScreen();
							LCD_displayString("Enter old password");
							userPassword();
							uartdata = UART_recieveByte();
							if(uartdata == STEP1)
							{
								LCD_clearScreen();
								LCD_displayString("Enter new Pass:");
								userPassword();
								uartdata = UART_recieveByte();
								if(uartdata == STEP2)
								{
									LCD_clearScreen();
									LCD_displayString("Enter new Pass:");
									userPassword();
								}
							}
							break;
					}
				}
				else if(uartdata == OPEN)
				{
					/*Door is unlocked for 15 SEC*/
					LCD_clearScreen();
					LCD_displayString("Door is unlocked");
					/* setting timer for 15Sec
					 *  Busy loop Technique */
					Timer_Init();
					while(timerFlag < FIFTEEN_SEC_DELAY);
					Timer_DeInit();
				}
				else if(uartdata == CLOSE)
				{
					/*Door is locked for 15 SEC*/
					LCD_clearScreen();
					LCD_displayString("Door is locked");
					/* setting timer for 15Sec
					 *  Busy loop Technique */
					Timer_Init();
					while(timerFlag < FIFTEEN_SEC_DELAY);
					Timer_DeInit();
				}
				/*if any fault occurred in main application system is locked
				 * for 1 min then back again to main application*/
				else if(uartdata == FAULT_OCCURANCE)
				{
					//lock system for 1Min
					LCD_clearScreen();
					LCD_displayString("System is locked");
					/* setting timer for 1Min
					 *  Busy loop Technique */
					Timer_Init();
					while(timerFlag < ONE_MIN_DELAY);
					Timer_DeInit();
				}
			}
		}
	}
}
