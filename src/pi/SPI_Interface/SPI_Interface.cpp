#include "SPI_Interface.hpp"

SPI_Interface::SPI_Interface(int channel, int speed)
{
	m_channel = channel;
	m_speed = speed;
}

SPI_Interface::~SPI_Interface()
{
	
}