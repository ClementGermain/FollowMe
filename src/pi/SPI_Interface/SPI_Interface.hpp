#ifndef __SPI_INTERFACE_HPP__
#define __SPI_INTERFACE_HPP__

class SPI_Interface 
{
	public:
		SPI_Interface(int channel, int speed);
		~SPI_Interface();
		int Start();
		int Send(unsigned char * data, int len);
	private:
		int m_speed;
		int m_channel;
};

#endif

