#include <thread>
#include <cstring>
#include "Car.hpp"
#include "LinkSTM32.hpp"
#include "SPI_Interface.hpp"
#include "utils/Log.hpp"

using namespace std;


LinkSTM32::LinkSTM32(int period_ms) :
	PeriodicThread(period_ms, "SPI"),
	bufferSize(max(sizeof(BarstowControl_Typedef), sizeof(BarstowModel_Typedef))
			/ sizeof(unsigned char) + sizeof(unsigned char)),
	SPI{SPI_CHANNEL, SPI_SPEED}
{
}

void LinkSTM32::begin() {
	SPI.Start();
	spiBuffer = new unsigned char[bufferSize];
}

void LinkSTM32::loop() {
	BarstowControl_Typedef control;
	BarstowModel_Typedef model;

	// Get 'control' from Car
	Car::getControlStructure(control);
	control.checkValue = CHECK_VALUE;

	// Exchange data with SPI
	std::memcpy(spiBuffer, &control, sizeof(BarstowControl_Typedef));
	SPI.Send(spiBuffer, bufferSize);
	std::memcpy(&model, spiBuffer, sizeof(BarstowModel_Typedef));

	// Fetch model structure from STM32 via SPI
	Car::updateModelStructure(model);
}

void LinkSTM32::end() {
	delete spiBuffer;
	spiBuffer = NULL;
}

