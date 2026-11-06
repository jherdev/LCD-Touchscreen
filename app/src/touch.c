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
uint8_t TS_ReadCoordinate(uint8_t tx_byte)
{
	uint8_t rx_byte = 0;

	// Select Slave (CS -> Low)
	GPIOB->BSRR |= GPIO_BSRR_BR_12;

	// make sure TXE (Transmit Buffer Empty) is set before sending data
	while((SPI1->SR & SPI_SR_TXE_Msk) == 0);

	// send tx_byte
	*(__IO uint8_t *)&SPI1->DR = tx_byte;

	// Wait until incoming data has arrived
	while((SPI1->SR & SPI_SR_RXNE_Msk) == 0);

	// Read data
	rx_byte = *(__IO uint8_t *)&SPI1->DR;

	// Release slave (CS -> High)
	GPIOB->BSRR |= GPIO_BSRR_BS_12;

	return rx_byte;
}
