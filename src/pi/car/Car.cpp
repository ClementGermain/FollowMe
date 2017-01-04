#include <mutex>
#include <algorithm>
#include "Car.hpp"

using namespace std;

// static members
mutex Car::controlMutex;
mutex Car::modelMutex;
BarstowControl_Typedef Car::controlStructure;
BarstowModel_Typedef Car::modelStructure;
const float Car::CarSize = 0.80f; // 80cm from back wheel axis to front of the car
const float Car::CarWidth = 0.52f;

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

void Car::writeControlGyro(bool isGyrophareOn) {
	controlMutex.lock();

	if (isGyrophareOn){
		controlStructure.gyro = 1;
	}	
	else {
		controlStructure.gyro = 0;
	}

	controlMutex.unlock();
}


void Car::writeControlMotor(Car::Motor target, MotorControl_Typedef & control) {
	controlMutex.lock();

	switch(target) {
		case Car::DirectionMotor:
			controlStructure.directionMotor = control;
			break;
		case Car::LeftWheelMotor:
		case Car::BothWheelMotors:
		case Car::RightWheelMotor:
			controlStructure.propulsionMotor = control;
			break;
	}

	controlMutex.unlock();
}

// @param speed: value in range [0, 1]
void Car::writeControlMotor(Car::Moving action, float speed) {
	MotorControl_Typedef control;

	control.speed = min(max(0.0f, speed), 1.0f);

	switch(action) {
		case Car::MoveForward:
			control.direction = MOTOR_DIRECTION_FORWARD;
			break;
		case Car::MoveBackward:
			control.direction = MOTOR_DIRECTION_BACKWARD;
			break;
		case Car::Stop:
			control.direction = MOTOR_DIRECTION_STOP;
			break;
	}
	
	writeControlMotor(BothWheelMotors, control);
}

// @param speed: value in range [0, 1]
void Car::writeControlMotor(Car::Turn action, float speed) {
	MotorControl_Typedef control;

	control.speed = min(max(0.0f, speed), 1.0f);

	switch(action) {
		case Car::TurnLeft:
			control.direction = MOTOR_DIRECTION_LEFT;
			break;
		case Car::TurnRight:
			control.direction = MOTOR_DIRECTION_RIGHT;
			break;
		case Car::NoTurn:
			control.direction = MOTOR_DIRECTION_STOP;
			break;
	}

	writeControlMotor(DirectionMotor, control);
}

void Car::updateModelStructure(BarstowModel_Typedef & model) {
	modelMutex.lock();

	modelStructure = model;

	modelMutex.unlock();
}

