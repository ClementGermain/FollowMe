#include "SPI_Interface.hpp" 
#include <wiringPiSPI.h>
#include "../utils/Log.hpp"

using namespace std;

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
	LogI << "Send" << ((int*)data)[0]<<endl;
	int n = wiringPiSPIDataRW (m_channel, data, len) ;
	LogI << "Recv" << ((int*)data)[0]<<endl;
	return n;
}
