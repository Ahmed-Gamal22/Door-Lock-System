/*
 * lcd.h
 *
 *  Created on: Dec 29, 2019
 *      Author: ahmed
 */

#include "uart.h"

void UART_init(void)
{
	/* U2X = 1 for double transmission speed */
	UCSRA = (1<<U2X);
	/* RXEN  = 1 Receiver Enable, RXEN  = 1 Transmitter Enable */
	UCSRB = (1<<RXEN) | (1<<TXEN);
	/* 8-bit data mode */
	UCSRC = (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1); 
	/* 9600 BaudRate 8MHZ F_CPU */
	UBRRH = 0;
	UBRRL = 103;
}
	
void UART_sendByte(const uint8 data)
{
	/* transmitting a new byte so wait until this flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,UDRE));
	/* the UDR register is not empty now */	 
	UDR = data;
}

uint8 UART_recieveByte(void)
{
	/* wait until RXC flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,RXC));
	/* Read the received data from the Rx buffer (UDR)*/
    	return UDR;		
}

void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
}

void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;
	Str[i] = UART_recieveByte();
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_recieveByte();
	}
	Str[i] = '\0';
}
