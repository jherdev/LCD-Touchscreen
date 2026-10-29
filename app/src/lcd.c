/*
 * lcd.c
 *
 *  Created on: Oct 28, 2026
 *      Author: Jorge
 */

#include "lcd.h"
#include "delay.h"

/*
 * Reset LCD Screen to default setings
 */
void LCD_Reset(void)
{
	// Clear RESET signal LO (active low) - PC8
	GPIOC->BSRR |= GPIO_BSRR_BR_8;

	// delay 100ms
	DELAY_MS(100);

	// Set RESET signal HI (active low) - PC8
	GPIOC->BSRR |= GPIO_BSRR_BS_8;

	//delay 50ms
	DELAY_MS(50);
}

/*
 * Write an 8-bit command to the LCD screen
 */
void LCD_WR_CMD(uint8_t data)
{
	// Clear (PC7) Chip Select (CS) - active low
	GPIOC->BSRR |= GPIO_BSRR_BR_7;

	// Clear (PC9) D/C (Data/Command) - sending command (1 = data, 0 = command)
	GPIOC->BSRR |= GPIO_BSRR_BR_9;

	// Write to SPI1 Buffer
	SPI1->DR = data;

	// make sure TXE is set before continuing // TXE = 1, indicating Tx buffer empty
	while ((SPI1->SR & SPI_SR_TXE_Msk) == 0);

	// Set (PC7) Chip Select (CS) - active low // we are done sending command
	GPIOC->BSRR |= GPIO_BSRR_BS_7;
}

/*
 * Write 8-bit data to the LCD screen
 */
void LCD_WR_DATA(uint8_t data)
{
	// Clear (PC7) Chip Select (CS) - active low
	GPIOC->BSRR |= GPIO_BSRR_BR_7;

	// Set (PC9) D/C (Data/Command) - sending data (1 = data, 0 = command)
	GPIOC->BSRR |= GPIO_BSRR_BS_9;

	// Write to SPI1 Buffer
	SPI1->DR = data;

	// make sure TXE is set before continuing // TXE = 1, indicating Tx buffer empty
	while ((SPI1->SR & SPI_SR_TXE_Msk) == 0);

	// Set (PC7) Chip Select (CS) - active low // we are done sending command
	GPIOC->BSRR |= GPIO_BSRR_BS_7;
}

/*
 * Write 16-bit data to the LCD screen
 */
void LCD_WR_DATA_16Bit(uint16_t data)
{
	// Clear (PC7) Chip Select (CS) - active low
	GPIOC->BSRR |= GPIO_BSRR_BR_7;

	// Set (PC9) D/C (Data/Command) - sending data (1 = data, 0 = command)
	GPIOC->BSRR |= GPIO_BSRR_BS_9;

	// Write to SPI1 Buffer
	SPI1->DR = (data >> 8);
	SPI1->DR = data;

	//wait until full transmisssion before continuing
	// make sure TXE is set before continuing // TXE = 1, indicating Tx buffer empty
	while ((SPI1->SR & SPI_SR_TXE_Msk) == 0);

	// Set (PC7) Chip Select (CS) - active low // we are done sending command
	GPIOC->BSRR |= GPIO_BSRR_BS_7;
}

/*
 * Initialize LCD Screen as per User Manual
 */
void LCD_Init(void)
{
	LCD_WR_CMD(0xCF);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xC9);		//C1
	LCD_WR_DATA(0x30);

	LCD_WR_CMD(0xED);
	LCD_WR_DATA(0x64);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0X12);
	LCD_WR_DATA(0X81);

	LCD_WR_CMD(0xE8);
	LCD_WR_DATA(0x85);
	LCD_WR_DATA(0x10);
	LCD_WR_DATA(0x7A);

	LCD_WR_CMD(0xCB);
	LCD_WR_DATA(0x39);
	LCD_WR_DATA(0x2C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x34);
	LCD_WR_DATA(0x02);

	LCD_WR_CMD(0xF7);
	LCD_WR_DATA(0x20);

	LCD_WR_CMD(0xEA);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);

	LCD_WR_CMD(0xC0);    	// Power control
	LCD_WR_DATA(0x1B);   	// VRH[5:0]

	LCD_WR_CMD(0xC1);    	// Power control
	LCD_WR_DATA(0x00);   	// SAP[2:0];BT[3:0] 01

	LCD_WR_CMD(0xC5);    	// VCM control
	LCD_WR_DATA(0x30); 	 	// 3F
	LCD_WR_DATA(0x30); 	 	// 3C

	LCD_WR_CMD(0xC7);    	// VCM control2
	LCD_WR_DATA(0XB7);

	LCD_WR_CMD(0x36);    	// Memory Access Control
	LCD_WR_DATA(0x08);

	LCD_WR_CMD(0x3A);
	LCD_WR_DATA(0x55);

	LCD_WR_CMD(0xB1);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x1A);

	LCD_WR_CMD(0xB6);    	// Display Function Control
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0xA2);

	LCD_WR_CMD(0xF2);    	// 3Gamma Function Disable
	LCD_WR_DATA(0x00);

	LCD_WR_CMD(0x26);    	// Gamma curve selected
	LCD_WR_DATA(0x01);

	LCD_WR_CMD(0xE0);    	// Set Gamma
	LCD_WR_DATA(0x0F);
	LCD_WR_DATA(0x2A);
	LCD_WR_DATA(0x28);
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x0E);
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x54);
	LCD_WR_DATA(0XA9);
	LCD_WR_DATA(0x43);
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0x0F);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);

	LCD_WR_CMD(0XE1);    	//Set Gamma
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x15);
	LCD_WR_DATA(0x17);
	LCD_WR_DATA(0x07);
	LCD_WR_DATA(0x11);
	LCD_WR_DATA(0x06);
	LCD_WR_DATA(0x2B);
	LCD_WR_DATA(0x56);
	LCD_WR_DATA(0x3C);
	LCD_WR_DATA(0x05);
	LCD_WR_DATA(0x10);
	LCD_WR_DATA(0x0F);
	LCD_WR_DATA(0x3F);
	LCD_WR_DATA(0x3F);
	LCD_WR_DATA(0x0F);

	LCD_WR_CMD(0x2B);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x3f);

	LCD_WR_CMD(0x2A);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xef);

	LCD_WR_CMD(0x11); 		//Exit Sleep

	DELAY_MS(120);

	LCD_WR_CMD(0x29); 		//display on
}

/*
 * Setting LCD display window
 * xStart: beginning of x coordinate of the LCD display window
 * yStart: beginning of y coordinate of the LCD display window
 * xEnd: ending of the x coordinate of the LCD Display window
 * yEnd: ending of the y coodinate of the LCD Display window
 */
void LCD_SetWindows(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd)
{
	// Set X coordinate start and end
	LCD_WR_CMD(TFT_CASET);			// Send Command [Set Column Address]
	LCD_WR_DATA(xStart >> 8);		//
	LCD_WR_DATA(0x00FF & xStart);	//
	LCD_WR_DATA(xEnd >> 8);			//
	LCD_WR_DATA(0x00FF & xEnd);		//

	// Set Y coordinate start and end
	LCD_WR_CMD(TFT_PASET);			// Send Command [Page Address Set]
	LCD_WR_DATA(yStart >> 8);		//
	LCD_WR_DATA(0x00FF & yStart);	//
	LCD_WR_DATA(yEnd >> 8);			//
	LCD_WR_DATA(0x00FF & yEnd);		//

	// LCD Write RAM Prepare
	LCD_WR_CMD(TFT_RAMWR);			// Send Command [Memory Write] - wramcmd
}

/*
 * full screen filled with Color parameter
 */
void LCD_Clear(uint16_t color)
{
	uint16_t i,m;
	LCD_SetWindows(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);

	// Clear (PC7) Chip Select (CS) - active low
	GPIOC->BSRR |= GPIO_BSRR_BR_7;

	// Set (PC9) D/C (Data/Command) - sending data (1 = data, 0 = command)
	GPIOC->BSRR |= GPIO_BSRR_BS_9;

	for(i = 0; i < LCD_HEIGHT; i++)
	{
		for(m = 0; m < LCD_WIDTH; m++)
		{
			LCD_WR_DATA_16Bit(color);
		}
	}

	// Set (PC7) Chip Select (CS) - active low // we are done sending command
	GPIOC->BSRR |= GPIO_BSRR_BS_7;
}






