/*
 * bsp.c
 *
 *  Created on: Oct 27, 2026
 *      Author: Jorge
 */

#include "bsp.h"

/*
 * BSP_SPI1_Init()
 * Initialization for LCD Screen
 * Initalize SPI1 on PA5, PA6, PA7 @ 375 kHz
 * Initalize GPIO PC7, PC9 for Chip Select and Data/Command
 *
 */
void BSP_SPI1_Init()
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
	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7 | GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);	// AND with NOT register position, clears it
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
	//SPI1->CR1 |= 0x06 << SPI_CR1_BR_Pos;
	SPI1->CR1 |= 0x00 << SPI_CR1_BR_Pos;		// 48MHz / 2 = 24MHz clock (slow wasn't working)

	// Set data size to 8-bit
	SPI1->CR2 |= 0x07 <<SPI_CR2_DS_Pos;

	// Set as master (SSI must be high), with software managed NSS
	SPI1->CR1 |= SPI_CR1_MSTR | SPI_CR1_SSI | SPI_CR1_SSM;

	// Enable SPI1
	SPI1->CR1 |= SPI_CR1_SPE;
}

/*
 *	BSP_SPI2_Init()
 *	Initalization for Touch Screen SPI
 *	Initialize SPI2 on PB12, PB13, PB14, PB15
 */
void BSP_SPI2_Init()
{
/*
 * 	SPI2 CS 	-> PB12	// Configure as GPIO
 * 	SPI2 SCK	-> PB13
 * 	SPI2 MISO	-> PB14
 * 	SPI2 MOSI	-> PB15
 */

	// Configure GPIO Chip Select PB12
	// Start GPIOB clock
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

	// Configure PB12 as GPIO Register
	GPIOB->MODER &= ~GPIO_MODER_MODER12;

	// Left Shift 0x01 (General Purpose Output Mode)
	GPIOB->MODER |= (0x01 << GPIO_MODER_MODER12_Pos);

	// Configure PB12 as output PUSH-PULL
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT_12;

	// Configure PB12 for High Speed I/O output
	GPIOB->OSPEEDR &= ~GPIO_OSPEEDR_OSPEEDR12;
	// Left shift 0x03(11) for High Speed I/O Output
	GPIOB->OSPEEDR |= (0x03 << GPIO_OSPEEDR_OSPEEDR12_Pos);

	// Configure PB12 for Pull Up / Pull Down Registers
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR12;

	// Atomic Set/Reset // Set CS Idle to High (Signal is active LOW)
	GPIOB->BSRR |= GPIO_BSRR_BS_12;

	// Configure GPIO IRQ for PB11	///////////////////////////////////////////////////////////////////////////////////////

	// Configure PB11 as a GPIO Register
	GPIOB->MODER &= ~GPIO_MODER_MODER11;

	// Left Shift 0x00 (Input Mode (reset state))
	GPIOB->MODER |= (0x00 << GPIO_MODER_MODER11_Pos);

	// Configure PB11 for Pull Up Register
	GPIOB->PUPDR |= (0x01 << GPIO_PUPDR_PUPDR11_Pos);

	// Configure PB13, PB14, PB15 as SPI2 pins (AF0)	///////////////////////////////////////////////////////////////////

	// Configure PB13, PB14 and PB15 as AF mode
	GPIOB->MODER &= ~(GPIO_MODER_MODER13 | GPIO_MODER_MODER14 | GPIO_MODER_MODER15);
	GPIOB->MODER |= (0x02 << GPIO_MODER_MODER13_Pos) | (0x02 << GPIO_MODER_MODER14_Pos) | (0x02 << GPIO_MODER_MODER15_Pos);

	// Connect to SPI2 (AF0)
	// There are no GPIOx_AFRL or GPIOx_AFRH registers, they are seperated into GPIOx_AFR[0] for 0-7 and GPIOx_FR[1] for 8-15
	GPIOB->AFR[1] &= ~(GPIO_AFRH_AFSEL13 | GPIO_AFRH_AFSEL14 | GPIO_AFRH_AFSEL15);
	GPIOB->AFR[1] |= (0x00 << GPIO_AFRH_AFSEL13_Pos) | (0x00 << GPIO_AFRH_AFSEL14_Pos) | (0x00 << GPIO_AFRH_AFSEL15_Pos);

	// Enable SPI2 Clock
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;

	// Configure SPI with default config
	// 4-wire = full-duplex (MISO/MOSI on separated lines)
	// 8-bit, no CRC, MSB first
	// CPHA = 0, CPOL = 0 -> SPI mode 0 (first clock rising edge)
	SPI2->CR1 = 0x0000;
	SPI2->CR2 = 0x0000;

	// Set the baudrate to 48MHz / 2 = 24MHz clock
	// SPI2->CR1 |= 0x00 << SPI_CR1_BR_Pos;
	// Set the baudrate to 48MHz /256 = 187KHz (slow, but easy to debug)
	SPI2->CR1 |= 0x07 <<SPI_CR1_BR_Pos;

	// Set data size to 8-bit
	SPI2->CR2 |= 0x07 <<SPI_CR2_DS_Pos;

	// Set FIFO threshold to 1-byte
	SPI2->CR2 |= SPI_CR2_FRXTH;		// 1: event generated if FIFO is >= 8 bit
	// 									// 0: event generated if FIFO is >= 16 bit

	// Set as master (SSI must be high), with software managed NSS
	SPI2->CR1 |= SPI_CR1_MSTR | SPI_CR1_SSI | SPI_CR1_SSM;

	// Enable SPI2
	SPI2->CR1 |= SPI_CR1_SPE;
}

/*
 * BSP_Console_Init()
 * USART2 @ 115200 Full Duplex
 * 1 start - 8-bit - 1 stop
 * TX -> PA2 (AF1)
 * RX -> PA3 (AF1)
 */
void BSP_Console_Init()
{
	// Enable GPIOA clock
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	// Configure PA2 and PA3 as Alternate function
	GPIOA->MODER &= ~(GPIO_MODER_MODER2_Msk | GPIO_MODER_MODER3_Msk);
	GPIOA->MODER |= (0x02 <<GPIO_MODER_MODER2_Pos) | (0x02 <<GPIO_MODER_MODER3_Pos);

	// Set PA2 and PA3 to AF1 (USART2)
	GPIOA->AFR[0] &= ~(0x0000FF00);
	GPIOA->AFR[0] |=  (0x00001100);

	// Enable USART2 clock
	RCC -> APB1ENR |= RCC_APB1ENR_USART2EN;

	// Clear USART2 configuration (reset state)
	// 8-bit, 1 start, 1 stop, CTS/RTS disabled
	USART2->CR1 = 0x00000000;
	USART2->CR2 = 0x00000000;
	USART2->CR3 = 0x00000000;

	// Select PCLK (APB1) as clock source
	// PCLK -> 48 MHz
	RCC->CFGR3 &= ~RCC_CFGR3_USART2SW_Msk;

	// Baud Rate = 115200
	// With OVER8=0 and Fck=48MHz, USARTDIV =   48E6/115200 = 416.6666
	// BRR = 417 -> Baud Rate = 115107.9137 -> 0.08% error
	//
	// With OVER8=1 and Fck=48MHz, USARTDIV = 2*48E6/115200 = 833.3333
	// BRR = 833 -> Baud Rate = 115246.0984 -> 0.04% error (better)
	USART2->CR1 |= USART_CR1_OVER8;
	USART2->BRR = 833;

	// Enable both Transmitter and Receiver
	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;

	// Enable USART2
	USART2->CR1 |= USART_CR1_UE;
}















