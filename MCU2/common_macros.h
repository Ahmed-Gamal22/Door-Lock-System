/*
 * common_macros.h
 *
 *  Created on: Dec 29, 2019
 *      Author: ahmed
 */

#ifndef COMMON_MACROS_H_
#define COMMON_MACROS_H_

#define SET_BIT(REG,BN) (REG|=(1<<BN))
#define CLEAR_BIT(REG,BN) (REG&=~(1<<BN))
#define TOGGLE_BIT(REG,BN) (REG^=(1<<BN))
#define BIT_IS_SET(REG,BN) (REG & (1<<BN))
#define BIT_IS_CLEAR(REG,BN) (!(REG & (1<<BN)))


#endif /* COMMON_MACROS_H_ */
