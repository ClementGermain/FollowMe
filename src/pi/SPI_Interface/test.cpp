#include "SPI_Interface.hpp"
#include <unistd.h>
#include <iostream>

#define BUFFER_SIZE 10

using namespace std;

int main()
{
	SPI_Interface SPI{0, 500000};
	
	std::cout << "Start : " << SPI.Start() << std::endl;
	
	unsigned char buffer[10];
	
	for (int i=0 ; i<=BUFFER_SIZE ; i++)
	{
		buffer[i] = i;
	}


	while(1)
	{
		std::cout << "Send :" << SPI.Send(buffer, sizeof(char)*BUFFER_SIZE) << std::endl;
		sleep(1);

		for (int i=0 ; i<BUFFER_SIZE ; i++)
		{
			cout << ((int)buffer[i]) << " ; ";
		}
		cout << endl;
	}
	
	return 0;
}
