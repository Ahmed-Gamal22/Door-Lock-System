/*
 * timer.h
 *
 *  Created on: Feb 7, 2020
 *      Author: ahmed
 */

#ifndef TIMER_H_
#define TIMER_H_
#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

extern volatile uint8 timerFlag;
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void Timer_Init(void);

void Timer_DeInit(void);
#endif /* TIMER_H_ */
