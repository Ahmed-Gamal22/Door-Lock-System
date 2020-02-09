/*
 * timer.c
 *
 *  Created on: Feb 7, 2020
 *      Author: ahmed
 */
#include "timer.h"
volatile uint8 timerFlag = 0;
void Timer_Init(void)
{
	TCCR1A = (1<<FOC1A)|(1<<FOC1B);			//Non PWM mode
	TCCR1B = (1<<WGM12)|(1<<CS12)|(1<<CS10);			//CTC Mode, 1024 prescaler
	TCNT1 = 0;
	OCR1A = 8000;							//1S = 8000 * 0.122 MS
	TIMSK|=(1<<OCIE1A);						//Timer1 CTC interrupt enable
	SREG=(1<<7);							//I-Bit enable
}
void Timer_DeInit(void)
{
	TCCR1B = 0;  	//Disable Clock source
	timerFlag = 0; 	// clear counter flag
}
ISR(TIMER1_COMPA_vect)
{
	timerFlag++;
}
