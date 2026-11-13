/*
 * tictactoe.h
 *
 *  Created on: Oct 29, 2026
 *      Author: Jorge
 */

#ifndef INC_TICTACTOE_H_
#define INC_TICTACTOE_H_

/*
 * tic tac toe functions
 */
void 		TTT_DrawX		(uint8_t row, uint8_t column);
void 		TTT_DrawO		(uint8_t row, uint8_t column);
void 		TTT_DrawGrid	(void);
uint8_t 	CoordToGridX	(uint16_t x);
uint8_t 	CoordToGridY	(uint16_t y);
bool 		TTT_CheckWinner	(char grid[3][3]);

void 		TTT_Game		(void);

#endif /* INC_TICTACTOE_H_ */
