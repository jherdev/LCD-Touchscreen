/*
 * touch.c
 *
 *  Created on: Nov 5, 2026
 *      Author: Jorge
 */

#include "touch.h"

/*
 * 	ReadCoordinate - reads coordinate x or y depending on parameter
 */
uint16_t TS_SendReceive(uint8_t tx_byte)
{
	uint8_t discard = 0;
	uint8_t rx_byte = 0;
	uint8_t temp = 0;
	uint16_t result = 0;

	// Select Slave (CS -> Low)
	GPIOB->BSRR |= GPIO_BSRR_BR_12;

	// make sure TXE (Transmit Buffer Empty) is set before sending data
	while((SPI2->SR & SPI_SR_TXE_Msk) == 0);

	// send tx_byte
	*(__IO uint8_t *)&SPI2->DR = tx_byte;

	// make sure TXE (Transmit Buffer Empty) is set before taking in data
	while((SPI2->SR & SPI_SR_TXE_Msk) == 0);

	// Wait until incoming data has arrived
	while((SPI2->SR & SPI_SR_RXNE_Msk) == 0);

	// Read data
	discard = *(__IO uint8_t *)&SPI2->DR;		// dummy read, not important


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// make sure TXE (Transmit Buffer Empty) is set before sending data
	while((SPI2->SR & SPI_SR_TXE_Msk) == 0);

	// send dummy bytes
	*(__IO uint8_t *)&SPI2->DR = 0x00;

	// make sure TXE (Transmit Buffer Empty) is set before sending data
	while((SPI2->SR & SPI_SR_TXE_Msk) == 0);

	// Wait until incoming data has arrived
	while((SPI2->SR & SPI_SR_RXNE_Msk) == 0);

	// Read data
	rx_byte = *(__IO uint8_t *)&SPI2->DR;		// actual read, return this data

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// make sure TXE (Transmit Buffer Empty) is set before sending data
	while((SPI2->SR & SPI_SR_TXE_Msk) == 0);

	// send dummy bytes
	*(__IO uint8_t *)&SPI2->DR = 0x00;

	// make sure TXE (Transmit Buffer Empty) is set before sending data
	while((SPI2->SR & SPI_SR_TXE_Msk) == 0);

	// Wait until incoming data has arrived
	while((SPI2->SR & SPI_SR_RXNE_Msk) == 0);

	// Read data
	temp = *(__IO uint8_t *)&SPI2->DR;		// actual read, return this data

	//rx_byte += temp;

	// Release slave (CS -> High)
	GPIOB->BSRR |= GPIO_BSRR_BS_12;

	result = rx_byte << 4;
	result |= (temp >> 4);

	return result;
}

/*
 * 	TouchScreenPressed - returns TRUE if touch screen is pressed, FALSE if not
 * 							uses T_IRQ line (active low) to check
 */
bool TouchScreenPressed(void)
{
	if((GPIOB->IDR & GPIO_IDR_11) == 0)
	{
		return true;
	}
	return false;
}
