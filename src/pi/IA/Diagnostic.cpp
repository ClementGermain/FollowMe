#include "Diagnostic.hpp"
#include <cmath>

using namespace std;

/*
thread * Diagnostic::threadTest = NULL;
bool Diagnostic::endThread = true;

bool Diagnostic::failure = false;
bool Diagnostic::failureMotor = false;

MotorModel Diagnostic::Model;

float Diagnostic::delta_voltage = 0.5; // TODO set real value
float Diagnostic::delta_current = 200;// TODO set real value
float Diagnostic::delta_speed = 0.1; // TODO set real value

void Diagnostic::init(){
    Model.load("model_propulsion");
}

void Diagnostic::diagnostic(){
    BarstowControl_Typedef BarstowControl;
    BarstowModel_Typedef BarstowModel;

	// Get data from car
	Car::getControlStructure(BarstowControl);
	Car::getModelStructure(BarstowModel);

	// Get data from expectation
	MotorModel_Typedef MotorM;
    Model.getState(BarstowControl.propulsionMotor.speed, MotorM);
    
	// compare data
	// TODO comparison for all the motors
    if(abs(MotorM.speed - BarstowModel.leftWheelMotor.speed) > delta_voltage) {
        failure = true;
        failureMotor = true;
    }
    else {
        failure = false;
        failureMotor = false;
    }
}

// ------------ Thread management -------- //
void Diagnostic::start() {
	if(threadTest == NULL) {
		endThread = false;
		threadTest = new thread(Diagnostic::run);
	}
}

void Diagnostic::stop() {
	if(threadTest != NULL) {
		endThread = true;
		threadTest->join();
		delete threadTest;
		threadTest = NULL;
	}
}

void Diagnostic::run() {
	init();

	while(!Diagnostic::endThread) {
        Diagnostic::diagnostic();
		// sleep 
		this_thread::sleep_for(chrono::milliseconds(100));
	}
}
// --------------------------------------- //

*/
