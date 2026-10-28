/*
 * main.c
 *
 *  Created on: July 28, 2026
 *      Author: Jorge
 */
#include "stm32f0xx.h"
#include "bsp.h"
#include "delay.h"
#include "lcd.h"

static void SystemClock_Config (void);

int main()
{
	SystemClock_Config();		// calls the clock configuration function
	BSP_SPI1_Init();			// configure SPI1 for LCD Screen



	LCD_Reset();

	LCD_Command(0xCF);
	LCD_Data(0x00);
	LCD_Data(0xC9);		//C1
	LCD_Data(0x30);

	LCD_Command(0xED);
	LCD_Data(0x64);
	LCD_Data(0x03);
	LCD_Data(0X12);
	LCD_Data(0X81);

	LCD_Command(0xE8);
	LCD_Data(0x85);
	LCD_Data(0x10);
	LCD_Data(0x7A);

	LCD_Command(0xCB);
	LCD_Data(0x39);
	LCD_Data(0x2C);
	LCD_Data(0x00);
	LCD_Data(0x34);
	LCD_Data(0x02);

	LCD_Command(0xF7);
	LCD_Data(0x20);

	LCD_Command(0xEA);
	LCD_Data(0x00);
	LCD_Data(0x00);

	LCD_Command(0xC0);    	// Power control
	LCD_Data(0x1B);   		// VRH[5:0]

	LCD_Command(0xC1);    	// Power control
	LCD_Data(0x00);   		// SAP[2:0];BT[3:0] 01

	LCD_Command(0xC5);    	// VCM control
	LCD_Data(0x30); 	 	// 3F
	LCD_Data(0x30); 	 	// 3C

	LCD_Command(0xC7);    	// VCM control2
	LCD_Data(0XB7);

	LCD_Command(0x36);    // Memory Access Control
	LCD_Data(0x08);

	LCD_Command(0x3A);
	LCD_Data(0x55);

	LCD_Command(0xB1);
	LCD_Data(0x00);
	LCD_Data(0x1A);

	LCD_Command(0xB6);    // Display Function Control
	LCD_Data(0x0A);
	LCD_Data(0xA2);

	LCD_Command(0xF2);    // 3Gamma Function Disable
	LCD_Data(0x00);

	LCD_Command(0x26);    //Gamma curve selected
	LCD_Data(0x01);

	LCD_Command(0xE0);    //Set Gamma
	LCD_Data(0x0F);
	LCD_Data(0x2A);
	LCD_Data(0x28);
	LCD_Data(0x08);
	LCD_Data(0x0E);
	LCD_Data(0x08);
	LCD_Data(0x54);
	LCD_Data(0XA9);
	LCD_Data(0x43);
	LCD_Data(0x0A);
	LCD_Data(0x0F);
	LCD_Data(0x00);
	LCD_Data(0x00);
	LCD_Data(0x00);
	LCD_Data(0x00);

	LCD_Command(0XE1);    //Set Gamma
	LCD_Data(0x00);
	LCD_Data(0x15);
	LCD_Data(0x17);
	LCD_Data(0x07);
	LCD_Data(0x11);
	LCD_Data(0x06);
	LCD_Data(0x2B);
	LCD_Data(0x56);
	LCD_Data(0x3C);
	LCD_Data(0x05);
	LCD_Data(0x10);
	LCD_Data(0x0F);
	LCD_Data(0x3F);
	LCD_Data(0x3F);
	LCD_Data(0x0F);

	LCD_Command(0x2B);
	LCD_Data(0x00);
	LCD_Data(0x00);
	LCD_Data(0x01);
	LCD_Data(0x3f);

	LCD_Command(0x2A);
	LCD_Data(0x00);
	LCD_Data(0x00);
	LCD_Data(0x00);
	LCD_Data(0xef);

	LCD_Command(0x11); //Exit Sleep

	DELAY_MS(120);

	LCD_Command(0x29); //display on

/////////////////////////////////////////////////////////////////////////////////////////



	while(1)
	{

	}
}

/*
 * 	Clock configuration for the Nucleo STM32F072RB board
 * 	HSE input Bypass Mode 				-> 8MHz
 * 	SYSCLK, AHB, APB1 					-> 48MHz
 *  	PA8 as MCO with /16 prescaler 	-> 3MHz
 *
 *  Laurent Latorre - 05/08/2017
 */

static void SystemClock_Config()
{
	uint32_t	HSE_Status;
	uint32_t	PLL_Status;
	uint32_t	SW_Status;
	uint32_t	timeout = 0;
	timeout = 1000000;
	// Start HSE in Bypass Mode
	RCC->CR |= RCC_CR_HSEBYP;
	RCC->CR |= RCC_CR_HSEON;
	// Wait until HSE is ready
	do
	{
		HSE_Status = RCC->CR & RCC_CR_HSERDY_Msk;
		timeout--;
	} while ((HSE_Status == 0) && (timeout > 0));
	// Select HSE as PLL input source
	RCC->CFGR &= ~RCC_CFGR_PLLSRC_Msk;
	RCC->CFGR |= (0x02 <<RCC_CFGR_PLLSRC_Pos);
	// Set PLL PREDIV to /1
	RCC->CFGR2 = 0x00000000;
	// Set PLL MUL to x6
	RCC->CFGR &= ~RCC_CFGR_PLLMUL_Msk;
	RCC->CFGR |= (0x04 <<RCC_CFGR_PLLMUL_Pos);
	// Enable the main PLL
	RCC-> CR |= RCC_CR_PLLON;
	// Wait until PLL is ready
	do
	{
		PLL_Status = RCC->CR & RCC_CR_PLLRDY_Msk;
		timeout--;
	} while ((PLL_Status == 0) && (timeout > 0));
        // Set AHB prescaler to /1
	RCC->CFGR &= ~RCC_CFGR_HPRE_Msk;
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
	//Set APB1 prescaler to /1
	RCC->CFGR &= ~RCC_CFGR_PPRE_Msk;
	RCC->CFGR |= RCC_CFGR_PPRE_DIV1;
	// Enable FLASH Prefetch Buffer and set Flash Latency
	FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;
	/* --- Until this point, MCU was still clocked by HSI at 8MHz ---*/
	/* --- Switching to PLL at 48MHz Now!  Fasten your seat belt! ---*/
	// Select the main PLL as system clock source
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	// Wait until PLL becomes main switch input
	do
	{
		SW_Status = (RCC->CFGR & RCC_CFGR_SWS_Msk);
		timeout--;
	} while ((SW_Status != RCC_CFGR_SWS_PLL) && (timeout > 0));
	/* --- Here we go! ---*/


	/*--- Use PA8 as MCO output at 48/16 = 3MHz ---*/
	// Set MCO source as SYSCLK (48MHz)
	RCC->CFGR &= ~RCC_CFGR_MCO_Msk;
	RCC->CFGR |=  RCC_CFGR_MCOSEL_SYSCLK;
	// Set MCO prescaler to /16 -> 3MHz
	RCC->CFGR &= ~RCC_CFGR_MCOPRE_Msk;
	RCC->CFGR |=  RCC_CFGR_MCOPRE_DIV16;
	// Enable GPIOA clock
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	// Configure PA8 as Alternate function
	GPIOA->MODER &= ~GPIO_MODER_MODER8_Msk;
	GPIOA->MODER |= (0x02 <<GPIO_MODER_MODER8_Pos);
	// Set to AF0 (MCO output)
	GPIOA->AFR[1] &= ~(0x0000000F);
	GPIOA->AFR[1] |=  (0x00000000);
	// Update SystemCoreClock global variable
	SystemCoreClockUpdate();
}
