/*
 * main.c
 *
 *  Created on: July 28, 2026
 *      Author: Jorge
 */
//#include <stdio.h>

#include "stm32f0xx.h"
#include "main.h"
#include "bsp.h"
#include "delay.h"
#include "lcd.h"
#include "tictactoe.h"
#include "touch.h"

int main()
{
	uint16_t x = 0, y = 0;

	SystemClock_Config();		// calls the clock configuration function
	BSP_SPI1_Init();			// configure SPI1 for LCD Screen
	BSP_SPI2_Init();			// configure SPI2 for Touch Screen
	BSP_Console_Init();			// initialize debug console

	LCD_Reset();				// Reset LCD Screen
	LCD_Init();					// Initialize LCD Screen as per the User Manual

	LCD_Clear(TFT_RED);			// set the entire LCD Screen to red
	ttt_DrawGrid();				// draws 3x3 Tic Tac Toe Grid

	while(1)
	{
		if(TouchScreenPressed())
		{
			x = TS_SendReceive(TS_CMD_X);
			y = TS_SendReceive(TS_CMD_Y);
			my_printf("X: %d, Y: %d\r\n", x, y);
			DELAY_ms(500);
		}
	}
}
