/*
 * tictactoe.c
 *
 *  Created on: Oct 29, 2026
 *      Author: Jorge
 */

#include <stdbool.h>
#include "lcd.h"
#include "touch.h"
#include "delay.h"

/*
 * Draws an X on LCD Screen depending on Row/Column
 */
void TTT_DrawX(uint8_t row, uint8_t column)
{
	uint16_t xStart, yStart, xEnd, yEnd;

	switch(row)
	{
		case 0:
			xStart = 20;
			xEnd = 60;
			break;
		case 1:
			xStart = 100;
			xEnd = 140;
			break;
		case 2:
			xStart = 180;
			xEnd = 220;
			break;
		default:
			// error
			break;
	}

	switch(column)
	{
		case 0:
			yStart = 20;
			yEnd = 86;
			break;
		case 1:
			yStart = 126;
			yEnd = 193;
			break;
		case 2:
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
void TTT_DrawO(uint8_t row, uint8_t column)
{
	uint16_t xOrigin, yOrigin;
	uint16_t radius = 29;

	switch(row)
	{
		case 0:
			xOrigin = 40;
			break;
		case 1:
			xOrigin = 120;
			break;
		case 2:
			xOrigin = 200;
			break;
		default:
			break;
	}

	switch(column)
	{
		case 0:
			yOrigin = 53;
			break;
		case 1:
			yOrigin = 159;
			break;
		case 2:
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
void TTT_DrawGrid(void)
{
	// Draw Tic Tac Toe
	LCD_DrawLine(80, 0, 80, (LCD_HEIGHT-1));		// Draw line horizontally
	LCD_DrawLine(160, 0, 160, (LCD_HEIGHT-1));		// Draw line horizontally

	LCD_DrawLine(0, 106, LCD_WIDTH-1, 106);			// Draw line vertically
	LCD_DrawLine(0, 213, LCD_WIDTH-1, 213);			// Draw line vertically
}

/*
 *	converts x coordinate value to tic tac toe x grid value
 */
uint8_t CoordToGridX(uint16_t x)
{
	if(x <= 80)
	{
		return 0;
	}
	else if(x <= 160)
	{
		return 1;
	}
	else
	{
		return 2;
	}
}

/*
 *	convert y coordinate value to tic tac toe y grid value
 */
uint8_t CoordToGridY(uint16_t y)
{
	if(y <= 106)
	{
		return 0;
	}
	else if(y <= 213)
	{
		return 1;
	}
	else
	{
		return 2;
	}
}

/*
 * 	checks for tic tac toe winner
 */
bool TTT_CheckWinner(char grid[3][3])
{

	if(grid[0][0] == grid[0][1] && grid[0][0] == grid[0][2] && grid[0][0] != '\0')	// top horizontal win
	{
		return true;
	}
	else if(grid[1][0] == grid[1][1] && grid[1][0] == grid[1][2] && grid[1][0] != '\0')	// middle horizontal win
	{
		return true;
	}
	else if(grid[2][0] == grid[2][1] && grid[2][0] == grid[2][2] && grid[2][0] != '\0')	// bottom horizontal win
	{
		return true;
	}
	else if(grid[0][0] == grid[1][0] && grid[0][0] == grid[2][0] && grid[0][0] != '\0')	// left vertical win
	{
		return true;
	}
	else if(grid[0][1] == grid[1][1] && grid[0][1] == grid[2][1] && grid[0][1] != '\0')	// middle vertical win
	{
		return true;
	}
	else if(grid[0][2] == grid[1][2] && grid [0][2] == grid[2][2] && grid[0][2] != '\0')	// right vertical win
	{
		return true;
	}
	else if(grid[0][0] == grid[1][1] && grid[0][0] == grid[2][2] && grid[0][0] != '\0')	// diagonal win, top left to bottom right
	{
		return true;
	}
	else if(grid[2][0] == grid[1][1] && grid[2][0] == grid[0][2] && grid[2][0] != '\0')	// diagonal win, bottom left to top right
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
 * 	Handles logic for Tic Tac Toe game
 */
void TTT_Game(void)
{
	uint8_t turn = 0;			// tracks turn number
	uint8_t state = 0;			// tracks game state
	uint8_t player = 0;			// 0 = O player turn, 1 = X player turn
	bool winner = false;		// indicates a winner has been determined when TRUE
	uint16_t x_coordinate = 0;
	uint16_t y_coordinate = 0;
	uint8_t x_grid = 0;
	uint8_t y_grid = 0;
	char ttt_grid[3][3] = {'\0'};	// tracks tic tac toe grid

	LCD_Clear(TFT_RED);			// set the entire LCD Screen to red
	TTT_DrawGrid();				// draws 3x3 Tic Tac Toe Grid

	while(state < 3)
	{
		switch(state)
		{
			case 0:							// game start, all we've done is initalize game
				while(TouchScreenPressed() == 0);	// loop here until initial player input
				turn = 1;	// initialize game turn
				state = 1;	// initialize game state
				break;
			case 1:					// loop input mode,
				if(TouchScreenPressed())
				{
					// retrieve x and y
					x_coordinate = TS_SendReceive(TS_CMD_X);
					y_coordinate = TS_SendReceive(TS_CMD_Y);

					// convert to touch screen x and y
					x_coordinate = (x_coordinate - 160) / 7.25;	// accounts for offset and converts to 0 - 240
					y_coordinate = (y_coordinate - 190) / 5.4;	// accounts for offset and converts to 0 - 320

					// convert to grid
					x_grid = CoordToGridX(x_coordinate);
					y_grid = CoordToGridY(y_coordinate);

					// check if valid input
					if(ttt_grid[x_grid][y_grid] == '\0')	// if valid input
					{
						if(player)	// if player X
						{
							TTT_DrawX(x_grid, y_grid);			// draw X
							ttt_grid[x_grid][y_grid] = 'X';		// update grid
						}
						else		// else player O
						{
							TTT_DrawO(x_grid, y_grid);			// draw O
							ttt_grid[x_grid][y_grid] = 'O';		// update grid
						}

						DELAY_ms(1000);		// 1 second delay between viable inputs

						if(turn >= 5)
						{
							winner = TTT_CheckWinner(ttt_grid);
						}

						if(winner || turn >= 9)	// if there's a winner (TRUE), move on
						{
							state = 2;
							// do not update player turn, current player won or no one did
						}
						else		// else, update and go through another turn
						{
							player ^= 1;	// toggle player turn
							turn++;			// update game turn
						}
					}
					DELAY_ms(800);	// wait 1 second for a viable input
				}
				break;
			case 2:
				if(winner)
				{
					if(player)	// if player X won
					{
						for(int r = 0; r < 100; r++)
						{
							LCD_DrawLine(60+r, 60, 180+r, 260);
							LCD_DrawLine(180+r, 60, 60+r, 260);
						}
					}
					else		// player O won
					{
						for(int i = 0; i < 100; i++)
						{
							LCD_DrawFillCircle(120, 160, 100 + i, false, TFT_BLACK);
						}
					}
					LCD_Clear(TFT_WHITE);
					DELAY_ms(2000);
				}
				else	// 9 turns played out and no one won
				{
					// output no winner
					LCD_Clear(TFT_ORANGE);			// set the entire LCD Screen to red
					DELAY_ms(2000);					// delay for 2 seconds on orange screen
				}
				state++;
				break;
			default:
				// some manner of error handling here
				break;
		}
	}
}


















