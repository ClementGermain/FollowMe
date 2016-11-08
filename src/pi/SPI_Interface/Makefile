all:
	g++ -c test.cpp -Wall -std=c++11
	g++ -c SPI_Interface.cpp -Wall -std=c++11
	g++ test.o SPI_Interface.o -Wall -lwiringPi

clean:
	rm -f *.o
	rm a.out
