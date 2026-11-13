/*
 * main.c
 *
 *  Created on: July 28, 2026
 *      Author: Jorge
 */

#include "stm32f0xx.h"
#include "main.h"
#include "bsp.h"
#include "delay.h"
#include "lcd.h"
#include "tictactoe.h"
#include "touch.h"

int main()
{
	SystemClock_Config();		// calls the clock configuration function
	BSP_SPI1_Init();			// configure SPI1 for LCD Screen
	BSP_SPI2_Init();			// configure SPI2 for Touch Screen
	BSP_Console_Init();			// initialize debug console

	LCD_Reset();				// Reset LCD Screen
	LCD_Init();					// Initialize LCD Screen as per the User Manual

	while(1)
	{
		TTT_Game();
	}
}
