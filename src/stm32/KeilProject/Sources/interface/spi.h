#ifndef __SPI__
#define __SPI__


typedef void (*dmaEventFunc)(void);

/** @brief Initialize the SPI
	* @param receiveBuffer: 	Reception buffer
	* @param receiveLen: 			Reception buffer length
	* @param sendBuffer: 			Sending buffer
	* @param sendLen: 				Sending buffer length
	* @param receiveEventFunc	Function called when dma reception is finished
	* @param sendEventFunc		Function called when dma sending is finished
	* @retval None
*/
void InitializeSPI2(unsigned char * receiveBuffer, int receiveLen, unsigned char * sendBuffer, int sendLenvoid, void receiveEventFunc(void), void sendEventFunc(void));

#endif
