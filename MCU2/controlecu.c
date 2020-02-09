/*
 * controlecu.c
 *
 *  Created on: Feb 2, 2020
 *      Author: ahmed
 *      CECU(control micro) used for all checking and decision maker.
 *
 */
#include "uart.h"
#include "external_eeprom.h"
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
#define SUCCESS 			1
#define FAIL				0
#define INITAL_VAL			0
#define MAX_FAULT_NUM		3

uint8 g_newPass[5];
uint8 g_eepromPass[5];

/*********************************************************************
 *[Function Name]: comparePass  		     						 *
 *[Description]: This function is responsible for taking pass from   *
 * 				UART module for second time and compare them.	     *
 *[In]: array of the first pass.									 *
 *[Out]: Success(1) or Fail(0)										 *
 ********************************************************************/
uint8 comparePass(uint8 *pass)
{
	uint8 counter = INITAL_VAL;
	UART_sendByte(STEP2);
	for(uint8 i = 0 ; i < PASS_NUM_DIGITS ; i++)
	{
		g_newPass[i] = UART_recieveByte();
	}
	for(uint8 i = 0 ; i < PASS_NUM_DIGITS ; i++)
	{
		if(pass[i] == g_newPass[i])
		{
			counter++;
			if(counter == PASS_NUM_DIGITS)
			{
				return SUCCESS;
			}
		}
		else
		{
			return FAIL;
		}
	}
}

/*********************************************************************
 *[Function Name]: createPass  		     							 *
 *[Description]: This function is responsible for taking pass from   *
 * 				UART module for first time.							 *
 *[In]: void														 *
 *[Out]: Success(1) or Fail(0)										 *
 ********************************************************************/
uint8 createPass(void)
{
	uint8 pass[5];
	UART_sendByte(STEP1); //UART Synch
	for(uint8 i = 0 ; i < PASS_NUM_DIGITS ; i++)
	{
		pass[i] = UART_recieveByte();
	}
	if(comparePass(pass) == SUCCESS)
	{
		return SUCCESS;
	}
	else
	{
		return FAIL;
	}
}

/*********************************************************************
 *[Function Name]: compareTwoPass  		     						 *
 *[Description]: This function is responsible for taking two  	     *
 * 				passwords and compare matching.						 *
 *[In]: array of the first and second pass							 *
 *[Out]: Success(1) or Fail(0)										 *
 ********************************************************************/
uint8 compareTwoPass(uint8 *enteredPass,uint8 *eepromPass)
{
	uint8 counter = 0 ;
	for(uint8 i = 0 ; i < PASS_NUM_DIGITS ; i++)
	{
		if(enteredPass[i] == eepromPass[i])
		{
			counter++;
			if(counter == PASS_NUM_DIGITS)
			{
				return SUCCESS;
			}
		}
		else
		{
			return FAIL;
		}
	}
}

/*********************************************************************
 *[Function Name]: changePass  		     							 *
 *[Description]: Repeat step one + save new password to the global	 *
 *				 variable and eeprom 								 *
 *[In]: void														 *
 *[Out]: void														 *
 ********************************************************************/
void changePass(void)
{
	uint8 returnCheck = 0;
	do{
		returnCheck = createPass();
	}while(returnCheck != SUCCESS);
	for(uint8 i = 0 ; i < PASS_NUM_DIGITS ; i++)
	{
		g_eepromPass[i] = g_newPass[i];
		EEPROM_writeByte(0x0400+i, g_newPass[i]);
		_delay_ms(10);
	}
}

/*********************************************************************
 *[Function Name]: openDoor  		     							 *
 *[Description]: PD3 = 0 & PD4 = 1 to make motor running clockwise   *
 *				 for 15Sec then stop motor. 						 *
 *[In]: void														 *
 *[Out]: void														 *
 ********************************************************************/
void openDoor(void)
{
	PORTD &= (~(1<<3));
	PORTD |= (1<<4);
	Timer_Init();
	while(timerFlag < FIFTEEN_SEC_DELAY);
	Timer_DeInit();
	PORTD &= (~(1<<4));
}

/*********************************************************************
 *[Function Name]: closeDoor  		     							 *
 *[Description]: PD3 = 3 & PD4 = 0 to make motor running 		     *
 *				 counterclockwise for 15Sec then stop motor.		 *
 *[In]: void														 *
 *[Out]: void														 *
 ********************************************************************/
void closeDoor(void)
{
	PORTD &= (~(1<<4));
	PORTD |= (1<<3);
	Timer_Init();
	while(timerFlag < FIFTEEN_SEC_DELAY);
	Timer_DeInit();
	PORTD &= (~(1<<3));
}
int main()
{
	/* configure pin PD3 and PD4 as output pins for motor */
	DDRD |= (1<<3)|(1<<4);
	/* Buzzer output pin */
	DDRB|=(1<<0);
	/* Motor is stop at the beginning */
	PORTD &=~(1<<3)&~(1<<4);
	/*initializations*/
	EEPROM_init();
	UART_init();
	uint8 returnCheck=INITAL_VAL; //functions return checkval
	uint8 faultCount=INITAL_VAL;
	uint8 frstTimeFlag=INITAL_VAL; //EEPROM check flag
	uint8 enteredPass[5];	//save the pass entered through UART
	/*Burn starting flag address in address 0x0300*/
	/*EEPROM_writeByte(0x0300, 0x00);
	_delay_ms(10);*/

	/* shaking hands between two micros */
	while(UART_recieveByte() != M1_READY){} // wait until MC1 is ready
	UART_sendByte(M2_READY); // MC2 is ready

	while(1)
	{
		/* reading permanent flag from EEPROM */
		EEPROM_readByte(0x0300,&frstTimeFlag);
		/* if flag = 0 --> this is the first time to run application */
		if(frstTimeFlag == 0)
		{
			/* user should enter pass two times matching */
			do{
				returnCheck = createPass();
			}while(returnCheck != SUCCESS);
			frstTimeFlag++;
			/*write the flag with 1 so we didn't get here again */
			EEPROM_writeByte(0x0300, frstTimeFlag);
			_delay_ms(10);
			/* saving entered password to EEPROM */
			for(uint8 i = 0 ; i < PASS_NUM_DIGITS ; i++)
			{
				EEPROM_writeByte(0x0400+i, g_newPass[i]);
				_delay_ms(10);
			}
		}
		/*not first time to run application*/
		else
		{
			/* reading password from EEPROM */
			for(uint8 i = 0 ; i < PASS_NUM_DIGITS ; i++)
			{
				EEPROM_readByte(0x0400+i,&g_eepromPass[i]);
			}
			/*super loop application*/
			while(1)
			{
				while(faultCount < MAX_FAULT_NUM)
				{
					UART_sendByte(STEP3);
					UART_sendByte(MAIN_OPTION);
					uint8 key = UART_recieveByte();
					for(uint8 i = 0 ; i < PASS_NUM_DIGITS ; i++)
					{
						enteredPass[i]= UART_recieveByte();
					}
					if(compareTwoPass(enteredPass,g_eepromPass) == SUCCESS)
					{
						switch(key)
						{
							case '*':
								UART_sendByte(OPEN);
								openDoor();
								UART_sendByte(CLOSE);
								closeDoor();
							break;
							case '#':
								changePass();
							break;
						}
						faultCount = 0;
					}
					else
					{
						faultCount++;
					}
				}
				/*BUZEER ON for 1min*/
				PORTB|=(1<<0);
				Timer_Init();
				while(timerFlag < ONE_MIN_DELAY);
				Timer_DeInit();
				PORTB&=~(1<<0);
				//system lock for 1min then Back again to options
				UART_sendByte(FAULT_OCCURANCE);
				Timer_Init();
				while(timerFlag < ONE_MIN_DELAY);
				Timer_DeInit();
				faultCount = 0;//clear fault flag to start option again
				UART_sendByte(MAIN_OPTION); //Synch UART to start options again
			}
		}
	}
}
