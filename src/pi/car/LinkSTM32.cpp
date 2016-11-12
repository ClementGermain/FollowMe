#include <thread>
#include "Car.hpp"
#include "LinkSTM32.hpp"

using namespace std;


LinkSTM32::LinkSTM32(int period_ms) :
	period_ms(period_ms),
	endThread(false),
	threadCom([this] {this->run(); })
{

}

LinkSTM32::~LinkSTM32() {
	endThread = true;
	threadCom.join();
}

void LinkSTM32::run() {
	BarstowControl_Typedef control;
	BarstowModel_Typedef model;

	while(!endThread) {
		// Fetch model structure from STM32 via SPI
		// TODO write in 'model'
		
		// Update Car
		Car::updateModelStructure(model);

		// Get 'control' from Car
		Car::getControlStructure(control);

		// Send control structure to STM32 via SPI
		// TODO read 'control'
		
		// Wait a bit then repeat
		this_thread::sleep_for(chrono::milliseconds(period_ms));
	}
}

