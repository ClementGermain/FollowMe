#ifndef __COMMUNICATION_STM32_HPP__
#define __COMMUNICATION_STM32_HPP__

#include "SPI_Interface.hpp"
#include "utils/PeriodicThread.hpp"

#define SPI_SPEED 500000
#define SPI_CHANNEL 0

/**
 * Thread that periodically sends commands and receives sensor data.
 * It uses SPI_Interface to communicate with the STM32.
 */
class LinkSTM32 : public PeriodicThread {
	public:
		LinkSTM32(int period_ms);

	private:
		void begin();
		void loop();
		void end();

		const int bufferSize;
		unsigned char * spiBuffer;

		SPI_Interface SPI;
};

#endif

