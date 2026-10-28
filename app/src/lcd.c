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
void LCD_Command(uint8_t data)
{
	// Clear (PC7) Chip Select (CS) - active low
	GPIOC->BSRR |= GPIO_BSRR_BR_7;

	// Clear (PC9) D/C (Data/Command) - sending command (1 = data, 0 = command)
	GPIOC->BSRR |= GPIO_BSRR_BR_9;

	//SPI_WriteByte(data);
	// Write to SPI1 Buffer
	SPI1->DR = data;

	//LCD_CS_SET;
	// Set (PC7) Chip Select (CS) - active low // we are done sending command
	GPIOC->BSRR |= GPIO_BSRR_BS_7;

}

void LCD_Data(uint8_t data)
{
	// Clear (PC7) Chip Select (CS) - active low
	GPIOC->BSRR |= GPIO_BSRR_BR_7;

	// Set (PC9) D/C (Data/Command) - sending data (1 = data, 0 = command)
	GPIOC->BSRR |= GPIO_BSRR_BS_9;

	//SPI_WriteByte(data);
	// Write to SPI1 Buffer
	SPI1->DR = data;

	//LCD_CS_SET;
	// Set (PC7) Chip Select (CS) - active low // we are done sending command
	GPIOC->BSRR |= GPIO_BSRR_BS_7;
}












