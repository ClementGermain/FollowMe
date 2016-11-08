#include "SPI_Interface.hpp" 
#include <wiringPiSPI.h>


SPI_Interface::SPI_Interface(int channel, int speed)
{
	m_channel = channel;
	m_speed = speed;
}

SPI_Interface::~SPI_Interface()
{
	
}

int SPI_Interface::Start()
{
	return  wiringPiSPISetup (m_channel, m_speed);
}

int SPI_Interface::Send(unsigned char* data, int len)
{
	return wiringPiSPIDataRW (m_channel, data, len) ;
}

