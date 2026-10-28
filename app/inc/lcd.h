/*
 * lcd.h
 *
 *  Created on: Oct 28, 2026
 *      Author: Jorge
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "stm32f0xx.h"

/*
 * LCD Functions
 */
void LCD_Reset		(void);
void LCD_Command	(uint8_t data);
void LCD_Data		(uint8_t data);


#endif /* INC_LCD_H_ */
