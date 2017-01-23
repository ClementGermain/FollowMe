#ifndef __SPI_INTERFACE_HPP__
#define __SPI_INTERFACE_HPP__

/**
 * This interface communicates to the STM32 via SPI with wiringPiSPI library. It sends and receives data through a common buffer.
 */
class SPI_Interface 
{
	public:
		SPI_Interface(int channel, int speed);
		~SPI_Interface();
		//! initialize SPI communication
		int Start();
		//! Send the data buffer and put response in data buffer. 'data' is the input and the output.
		int Send(unsigned char * data, int len);
	private:
		int m_speed;
		int m_channel;
};

#endif

