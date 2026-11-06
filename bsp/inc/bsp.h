/*
 * bsp.h
 *
 *  Created on: Oct 27, 2026
 *      Author: Jorge
 */

#ifndef INC_BSP_H_
#define INC_BSP_H_

#include "stm32f0xx.h"

/*
 *	LCD SPI Functions
 */
void BSP_SPI1_Init(void);

/*
 * 	Touch Screen SPI functions
 */
void BSP_SPI2_Init(void);

/*
 * 	UART Console Initalization
 */
void BSP_Console_Init(void);

#endif /* INC_BSP_H_ */
