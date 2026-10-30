/*
 * lcd.h
 *
 *  Created on: Oct 28, 2026
 *      Author: Jorge
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "stm32f0xx.h"

// Parameters
#define LCD_WIDTH		240
#define LCD_HEIGHT		320

#define POINT_COLOR		0x0000		// black

// Color definitions
#define TFT_BLACK       0x0000      /*   0,   0,   0 */
#define TFT_NAVY        0x000F      /*   0,   0, 128 */
#define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define TFT_MAROON      0x7800      /* 128,   0,   0 */
#define TFT_PURPLE      0x780F      /* 128,   0, 128 */
#define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
#define TFT_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define TFT_BLUE        0x001F      /*   0,   0, 255 */
#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
#define TFT_RED         0xF800      /* 255,   0,   0 */
#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
#define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
#define TFT_ORANGE      0xFD20      /* 255, 165,   0 */
#define TFT_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define TFT_PINK        0xF81F

// Delay between some initialisation commands
#define TFT_INIT_DELAY 0x80 		// Not used unless commandlist invoked

// Generic commands
#define TFT_NOP     	0x00		// NOP
#define TFT_SWRST   	0x01		// Software Reset
#define TFT_RDDID		0x04		// Read Display Identification Information
#define TFT_RDDST		0x09		// Read Display Status

#define TFT_SLEEPIN		0x10		// Enter Sleep Mode
#define TFT_SLEEPOUT	0x11		// Sleep Out
#define TFT_PTLON		0x12		// Partial Mode ON
#define TFT_NORON		0x13		// Normal Display Mode ON

#define TFT_RDMODE  	0x0A		// Read Display Power Mode
#define TFT_RDMADCTL  	0x0B		// Read Display MADCTL
#define TFT_RDPIXFMT  	0x0C		// Read Display Pixel Format
#define TFT_RDIMGFMT  	0x0D		// Read Image Format
//#define TFT_RDDSM		0x0E		// Read Display Signal Mode
#define TFT_RDSELFDIAG  0x0F		// Read Display Self-Diagnostic Result

#define TFT_INVOFF  	0x20		// Display Inversion OFF
#define TFT_INVON   	0x21		// Display Inversion ON
#define TFT_GAMMASET	0x26		// Gamma Set
#define TFT_DISPOFF 	0x28		// Display OFF
#define TFT_DISPON  	0x29		// Display ON

#define TFT_CASET   	0x2A		// Column Address Set	// setxcmd
#define TFT_PASET  		0x2B		// Page Address Set		// setycmd
#define TFT_RAMWR   	0x2C		// Memory Write
#define TFT_RAMRD   	0x2E		// Memory Read


#define TFT_PTLAR   	0x30		// Partial Area
#define TFT_VSCRDEF 	0x33		// Vertical Scrolling Definition
//#define TFT_MADCTL  	0x36		// Memory Access Control
#define TFT_VSCRSADD 	0x37		// Vertical Scrolling Start Address
#define TFT_PIXFMT  	0x3A		// COLMOD: Pixel Format Set

#define TFT_WRDISBV  	0x51		// Write Display Brightness
#define TFT_RDDISBV  	0x52		// Read Display Brightness
#define TFT_WRCTRLD  	0x53		// Write CTRL Display

#define TFT_FRMCTR1 	0xB1		// Frame Rate Control (In Normal Mode/Full Colors)
#define TFT_FRMCTR2 	0xB2		// Frame Rate Control (In Idle Mode/8 Colors)
#define TFT_FRMCTR3 	0xB3		// Frame Rate Contorl (In Partial Mode/Full Colors)
#define TFT_INVCTR  	0xB4		// Display Inversion Control
#define TFT_DFUNCTR 	0xB6		// Display Function Control

#define TFT_PWCTR1  	0xC0		// Power Control 1
#define TFT_PWCTR2  	0xC1		// Power Control 2
#define TFT_PWCTR3  	0xC2		//
#define TFT_PWCTR4  	0xC3
#define TFT_PWCTR5  	0xC4
#define TFT_VMCTR1  	0xC5		// VCOM Control 1
#define TFT_VMCTR2  	0xC7		// VCOM Control 2

#define TFT_RDID4   	0xD3		// Read ID4
#define TFT_RDINDEX 	0xD9		//
#define TFT_RDID1   	0xDA
#define TFT_RDID2   	0xDB
#define TFT_RDID3   	0xDC

// the following command is not listed on the ILI9341 Datasheet - (or atleast not that I can find)
#define TFT_RDIDX   	0xDD 		// TBC
#define TFT_IDXRD   	0xDD 		// ILI9341 only, indexed control register read

#define TFT_GMCTRP1 	0xE0		// Positive Gamma Correction
#define TFT_GMCTRN1 	0xE1		// Negative Gamma Correction


#define TFT_MADCTL  	0x36		// Memory Access Control
#define TFT_MAD_MY  	0x80		//
#define TFT_MAD_MX  	0x40
#define TFT_MAD_MV  	0x20
#define TFT_MAD_ML  	0x10
#define TFT_MAD_BGR 	0x08		// Blue|Green|Red
#define TFT_MAD_MH  	0x04
#define TFT_MAD_RGB 	0x00		// Red|Green|Blue

/*
 * LCD Functions
 */
void LCD_Reset			(void);
void LCD_WR_CMD			(uint8_t data);
void LCD_WR_DATA		(uint8_t data);
void LCD_WR_DATA_16Bit	(uint16_t data);
void LCD_Init			(void);
void LCD_SetWindows		(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
void LCD_Clear			(uint16_t color);
void LCD_DrawPoint		(uint16_t x, uint16_t y);
void LCD_SetCursor		(uint16_t x_position, uint16_t y_position);
void LCD_DrawLine		(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);


#endif /* INC_LCD_H_ */




