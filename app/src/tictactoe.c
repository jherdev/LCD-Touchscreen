/*
 * tictactoe.c
 *
 *  Created on: Oct 29, 2026
 *      Author: Jorge
 */

#include "lcd.h"

/*
 * Draws an X on LCD Screen depending on Row/Column
 */
void ttt_DrawX(uint8_t row, uint8_t column)
{
	uint16_t xStart, yStart, xEnd, yEnd;

	switch(row)
	{
		case 1:
			xStart = 180;
			xEnd = 220;
			break;
		case 2:
			xStart = 100;
			xEnd = 140;
			break;
		case 3:
			xStart = 20;
			xEnd = 60;
			break;
		default:
			// error
			break;
	}

	switch(column)
	{
		case 1:
			yStart = 20;
			yEnd = 86;
			break;
		case 2:
			yStart = 126;
			yEnd = 193;
			break;
		case 3:
			yStart = 233;
			yEnd = 300;
			break;
		default:
			// error
			break;
	}

	LCD_DrawLine(xStart+1, yStart, xEnd+1, yEnd);
	LCD_DrawLine(xStart, yStart, xEnd, yEnd);
	LCD_DrawLine(xStart-1, yStart, xEnd-1, yEnd);

	LCD_DrawLine(xEnd+1, yStart, xStart+1, yEnd);
	LCD_DrawLine(xEnd, yStart, xStart, yEnd);
	LCD_DrawLine(xEnd-1, yStart, xStart-1, yEnd);
}

/*
 * Draws an O on LCD Screen depending on Row/Column
 */
void ttt_DrawO(uint8_t row, uint8_t column)
{
	uint16_t xOrigin, yOrigin;
	uint16_t radius = 29;

	switch(row)
	{
		case 1:
			xOrigin = 200;
			break;
		case 2:
			xOrigin = 120;
			break;
		case 3:
			xOrigin = 40;
			break;
		default:
			break;
	}

	switch(column)
	{
		case 1:
			yOrigin = 53;
			break;
		case 2:
			yOrigin = 159;
			break;
		case 3:
			yOrigin = 265;
			break;
		default:
			break;
	}

	LCD_DrawFillCircle(xOrigin, yOrigin, radius, false, TFT_BLACK);
	LCD_DrawFillCircle(xOrigin, yOrigin, radius+1, false, TFT_BLACK);
	LCD_DrawFillCircle(xOrigin, yOrigin, radius+2, false, TFT_BLACK);
}

/*
 * 	Draws two horizontal lines and two vertical lines to create the tic tac toe grid
 */
void ttt_DrawGrid(void)
{
	// Draw Tic Tac Toe
	LCD_DrawLine(80, 0, 80, (LCD_HEIGHT-1));		// Draw line horizontally
	LCD_DrawLine(160, 0, 160, (LCD_HEIGHT-1));		// Draw line horizontally

	LCD_DrawLine(0, 106, LCD_WIDTH-1, 106);			// Draw line vertically
	LCD_DrawLine(0, 213, LCD_WIDTH-1, 213);			// Draw line vertically
}









