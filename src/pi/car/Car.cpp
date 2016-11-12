#include <mutex>
#include "Car.hpp"

// static members
std::mutex Car::controlMutex;
std::mutex Car::modelMutex;
BarstowControl_Typedef Car::controlStructure;
BarstowModel_Typedef Car::modelStructure;

void Car::getControlStructure(BarstowControl_Typedef & out) {
	controlMutex.lock();
	
	out = controlStructure;

	controlMutex.unlock();
}

void Car::getModelStructure(BarstowModel_Typedef & out) {
	modelMutex.lock();

	out = modelStructure;

	modelMutex.unlock();
}

void Car::writeControlMotor(Car::Motor target, MotorControl_Typedef & control) {
	controlMutex.lock();

	switch(target) {
		case Car::DirectionMotor:
			controlStructure.directionMotor = control;
			break;
		case Car::LeftWheelMotor:
			controlStructure.leftWheelMotor = control;
			break;
		case Car::RightWheelMotor:
			controlStructure.rightWheelMotor = control;
			break;
	}

	controlMutex.unlock();
}

void Car::updateModelStructure(BarstowModel_Typedef & model) {
	modelMutex.lock();

	modelStructure = model;

	modelMutex.unlock();
}
