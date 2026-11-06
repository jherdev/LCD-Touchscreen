/*
 * touch.h
 *
 *  Created on: Nov 5, 2026
 *      Author: Jorge
 */

#ifndef INC_TOUCH_H_
#define INC_TOUCH_H_

#include "stm32f0xx.h"

#define	TS_READ_X			0xD0	// 12-bit Read x coordinate
#define TS_READ_X8			0xD8	// 8-bit Read x coordinate
#define TS_READ_Y			0x90	// 12-bit Read y coordinate
#define TS_READ_Y8			0x98	// 8-bit Read y coordinate

/*
 * 	Touch Screen Functions
 */
uint8_t TS_ReadCoordinate(uint8_t tx_byte);

#endif /* INC_TOUCH_H_ */
