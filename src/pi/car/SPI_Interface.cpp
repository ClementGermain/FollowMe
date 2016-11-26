#include "SPI_Interface.hpp" 
#include <errno.h>
#include <cstring>
#ifndef __NO_RASPI__
#include <wiringPiSPI.h>
#endif

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
#ifndef __NO_RASPI__
	int res = wiringPiSPISetup (m_channel, m_speed);
	if(res == -1) {
		LogE << "WiringPi SPI Setup failed: " << strerror(errno) << endl;
	}
	return res;
#else
	return 0;
#endif
}

int SPI_Interface::Send(unsigned char* data, int len)
{
#ifndef __NO_RASPI__
	int n = wiringPiSPIDataRW (m_channel, data, len) ;
	return n;
#else
	return 0;
#endif
}
