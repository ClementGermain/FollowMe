#ifndef __COMMUNICATION_STM32_HPP__
#define __COMMUNICATION_STM32_HPP__

#include <thread>

#define SPI_SPEED 500000
#define SPI_CHANNEL 0

class LinkSTM32 {
	public:
		LinkSTM32(int period_ms=50);
		~LinkSTM32();

	private:
		void run();

		const int period_ms;
		bool endThread;
		SPI_Interface SPI;
		std::thread threadCom;
};

#endif

