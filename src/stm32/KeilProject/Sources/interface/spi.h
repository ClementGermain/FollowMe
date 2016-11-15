#ifndef __SPI__
#define __SPI__

// include stm32 library
#include "stm32f10x_spi.h"

// include local library
#include "../library/gpio.h"

/** @brief Initialize the SPI
	* @param receiveBuffer: Reception buffer
	* @param receiveLen: 		Reception buffer length
	* @param sendBuffer: 		Sending buffer
	* @param sendLen: 			Sending buffer length
	* @retval None
*/
void InitializeSPI2(unsigned char * receiveBuffer, int receiveLen, unsigned char * sendBuffer, int sendLen);

#endif
