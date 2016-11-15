#ifndef __SPI_INTERFACE_H
#define __SPI_INTERFACE_H

// include stm32 library
#include "stm32f10x_spi.h"

// include local library
#include "../gpio.h"

#define BUFFER_SIZE 10

/** @brief Initialize the SPI
	* @param receiveBuffer: ??
	* @param receiveLen: ??
	* @param sendBuffer: ??
	* @param sendLen: ??
	* @retval None
*/
void InitializeSPI2(unsigned char * receiveBuffer, int receiveLen, unsigned char * sendBuffer, int sendLen);

#endif
