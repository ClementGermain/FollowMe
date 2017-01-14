#include <thread>
#include <cstring>
#include "Car.hpp"
#include "LinkSTM32.hpp"
#include "SPI_Interface.hpp"
#include "utils/Log.hpp"

using namespace std;


LinkSTM32::LinkSTM32(int period_ms) :
	period_ms(period_ms),
	endThread(false),
	SPI{SPI_CHANNEL, SPI_SPEED},
	threadCom([this] {this->run(); })
{
	LogI << "Starting SPI thread..." << endl;
	SPI.Start();
}

LinkSTM32::~LinkSTM32() {
	LogI << "Joining SPI thread..." << endl;
	endThread = true;
	threadCom.join();
	LogI << "SPI thread terminated" << endl;
}

void LinkSTM32::run() {
	BarstowControl_Typedef control;
	BarstowModel_Typedef model;

	const int bufferSize =	max(sizeof(BarstowControl_Typedef),
								sizeof(BarstowModel_Typedef))
							/ sizeof(unsigned char)
							+ sizeof(unsigned char);

	unsigned char spiBuffer[bufferSize];

	while(!endThread) {
		// Fetch model structure from STM32 via SPI
		
		// Get 'control' from Car
		Car::getControlStructure(control);
		control.checkValue = CHECK_VALUE;

		// Exchange data with SPI
		std::memcpy(spiBuffer, &control, sizeof(BarstowControl_Typedef));
		SPI.Send(spiBuffer, bufferSize);
		std::memcpy(&model, spiBuffer, sizeof(BarstowModel_Typedef));

		// Update Car
		Car::updateModelStructure(model);

		// Wait a bit then repeat
		this_thread::sleep_for(chrono::milliseconds(period_ms));
	}
}

