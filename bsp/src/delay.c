/*
 * delay.c
 *
 *  Created on: Oct 28, 2026
 *      Author: Jorge
 */

#include "delay.h"

/*
 * Millisecond Delay Function
 */

void DELAY_ms(uint32_t delay)
{
	uint32_t i;
	for(i = 0; i < (delay*2500); i++);	// Tuned for ms at 48Mhz
}

void DELAY_us(uint32_t delay)
{
	uint32_t i;
	for(i =0; i < (delay*3); i++);		// tuned for μs at 48 Mhz
}
