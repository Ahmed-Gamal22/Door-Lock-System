/*
 * keypad.h
 *
 *  Created on: Dec 29, 2019
 *      Author: ahmed
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_
#include "std_Types.h"
#include "micro_config.h"
#include "common_macros.h"

#define N_ROW 4
#define N_COL 4

#define KEYPAD_PORT_DIR 	DDRA
#define KEYPAD_PORT_PIN 	PINA
#define KEYPAD_PORT 		PORTA

uint8 Keypad_getPressedKey(void);

#endif /* KEYPAD_H_ */
