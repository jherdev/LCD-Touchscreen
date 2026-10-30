/*
 * delay.h
 *
 *  Created on: Oct 28, 2026
 *      Author: Jorge
 */

#ifndef INC_DELAY_H_
#define INC_DELAY_H_

#include "stm32f0xx.h"

/*
 * Software Counting Delay
 */
void DELAY_ms(uint32_t delay);
void DELAY_us(uint32_t delay);

#endif /* INC_DELAY_H_ */
