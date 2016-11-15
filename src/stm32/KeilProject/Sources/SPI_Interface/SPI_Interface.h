#ifndef __SPI_INTERFACE_H
#define __SPI_INTERFACE_H

// include stm32 library
#include "stm32f10x_spi.h"

// include local library
#include "../gpio.h"

/** @brief Initialize the SPI
	* @param receiveBuffer: Reception buffer
	* @param receiveLen: 		Reception buffer length
	* @param sendBuffer: 		Sending buffer
	* @param sendLen: 			Sending buffer length
	* @retval None
*/
void InitializeSPI2(unsigned char * receiveBuffer, int receiveLen, unsigned char * sendBuffer, int sendLen);

#endif
