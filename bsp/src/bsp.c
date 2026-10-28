/*
 * bsp.c
 *
 *  Created on: Oct 27, 2026
 *      Author: Jorge
 */

#include "bsp.h"

/*
 * LCD_SPI1_Init()
 * Initalize SPI1 on PA5, PA6, PA7 @ 375 kHz
 * Initalize GPIO PC7, PC9 for Chip Select and Data/Command
 *
 */
void LCD_SPI1_Init()
{
	// SPI_SCK		-> PA5
	// SPI_MISO		-> PA6
	// SPI_MOSI		-> PA7
	// CS Pin		-> PC7 (GPIO)
	// Data/Command	-> PC9 (GPIO)

	// Configure PC7 as Chip Select (CS) pin // Configure PC9 as Data/Command (DCX) pin ------------------------------------------------------
	// Configure PC8 as RESET pin // Configure PC6 as LED Pin --------------------------------------------------------------------------------

	// Start GPIOC clock
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;					// this seems to be universal first step, can't "clock" changes if clock isn't running

	// Configure PC7 and PC9 as General Purpose OUTPUT mode register
	GPIOC->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7 | GPIO_MODER_MODER8 | GPIO_MODER_MODER9);	// AND with NOT register name, clears the register
	// LEFT SHIFT 0x01 (General Purpose Output Mode)
	GPIOC->MODER |= (0x01 << GPIO_MODER_MODER6_Pos) | (0x01 << GPIO_MODER_MODER7_Pos) | (0x01 << GPIO_MODER_MODER8_Pos) | (0x01 << GPIO_MODER_MODER9_Pos);

	// Configure PC6, PC7, PC8, and PC9 as output PUSH-PULL
	GPIOC->OTYPER &= ~(GPIO_OTYHPER_OT_6 | GPIO_OTYPER_OT_7 | GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);	// AND with NOT register position, clears it
	// no need to set it to anything, we want bit set to 0 for Output PUSH-PULL

	// Configure PC6, PC7, PC8, and PC9 for High Speed I/O output
	// Low and Medium speeds seem to be for low power modes, and only High Speed can reach 50Mhz clock we're using (i think)
	GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEEDR6 | GPIO_OSPEEDR_OSPEEDR7 | GPIO_OSPEEDR_OSPEEDR8 | GPIO_OSPEEDR_OSPEEDR9);	// AND with NOT register name, clears it
	// LEFT SHIFT 0x03 (11) for HIGH SPEED I/O output
	GPIOC->OSPEEDR |= (0x03 << GPIO_OSPEEDR_OSPEEDR6_Pos) | (0x03 << GPIO_OSPEEDR_OSPEEDR7_Pos) | (0x03 << GPIO_OSPEEDR_OSPEEDR8_Pos) | (0x03 << GPIO_OSPEEDR_OSPEEDR9_Pos);

	// Configure PC6, PC7, PC8, and PC9 for Pull-Up / Pull-Down Registers
	// as an OUTPUT pin, we do NOT need pull-up or pull-down registers
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7 | GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);	// AND with NOT [register name], clears the register
	// not need to set it to anything, 00 gives us No pull-up, pull-down setting

	// Accessing GPIO output pins using BSRR register performs an atomic bit set/reset
	// Set Chip Select (PC7) to Idle as HIGH (signal is active low)
	GPIOC->BSRR |= GPIO_BSRR_BS_7;
	// Set RESET (PC8) to Idle as HIGH (signal is active low)
	GPIOC->BSRR |= GPIO_BSRR_BS_8;
	// Set LED (PC6) to Idle as HIGH (LED ON, can turn off backlight later or connect to PWM to adjust brightness)
	GPIOC->BSRR |= GPIO_BSRR_BS_6;

	// Configure PA5, PA6, PA7 as SPI1 pins (AF0) // ----------------------------------------------------------------------------------------

	// Start GPIOA clock
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	// Configure PA5, PA6, and PA7 as AF mode
	GPIOA->MODER &= ~(GPIO_MODER_MODER5 | GPIO_MODER_MODER6 | GPIO_MODER_MODER7);
	GPIOA->MODER |= (0x02 << GPIO_MODER_MODER5_Pos) | (0x02 << GPIO_MODER_MODER6_Pos) | (0x02 << GPIO_MODER_MODER7_Pos);

	// Connect to SPI1 (AF0)
	// There are no GPIOx_AFRL or GPIOx_AFRH registers, they are seperated into GPIOx_AFR[0] for 0-7 and GPIOx_FR[1] for 8-15
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL5 | GPIO_AFRL_AFSEL6 | GPIO_AFRL_AFSEL7);
	GPIOA->AFR[0] |= (0x00 << GPIO_AFRL_AFSEL5_Pos) | (0x00 << GPIO_AFRL_AFSEL6_Pos) | (0x00 << GPIO_AFRL_AFSEL7_Pos);

	// Configure SPI1 // --------------------------------------------------------------------------------------------------------------------
	// Enable SPI1 Clock
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	// Configure SPI with default config
	// 4-wire = full-duplex (MISO/MOSI on separated lines)
	// 8-bit, no CRC, MSB first
	// CPHA = 0, CPOL = 0 -> SPI mode 0 (first clock rising edge)
	SPI1->CR1 = 0x0000;
	SPI1->CR2 = 0x0000;

	// Set the baudrate to 48MHz /128 = 375kHz (slow, but easy to debug)
	SPI1->CR1 |= 0x06 <<SPI_CR1_BR_Pos;

	// Set data size to 8-bit
	SPI1->CR2 |= 0x07 <<SPI_CR2_DS_Pos;

	// Set as master (SSI must be high), with software managed NSS
	SPI1->CR1 |= SPI_CR1_MSTR | SPI_CR1_SSI | SPI_CR1_SSM;

	// Enable SPI1
	SPI1->CR1 |= SPI_CR1_SPE;
}

















