#include "IA.hpp"
#include "improc/UserPatternDetection.hpp"
#include "improc/UserPatternDetectionTest.hpp"
#include "utils/Log.hpp"
#include "car/Car.hpp"
#include <chrono>
#include <thread>

using namespace std;

float IA::Dist = 0.0;
float IA::Speed = 0.0;
bool IA::UserDetected = false;

thread * IA::threadTest = NULL;
bool IA::endThread = true;

// ---Linar function for speed control---- //
void IA::SpeedControl (float distance){
	if (distance <= 3) {
	IA::Speed = distance*(1.0/3.0);
	}
	else {
	IA::Speed = 1.0;
	}
}
// --------------------------------------- //

// ---------Back motors management-------- //
void IA::IAMotorBack(float distance) {
	LogD << "IAMotorBack distance "<<distance<<endl;
	IA::UserDetected = UserDetectionTest.detector.isDetected();
	IA::SpeedControl(distance);
	if (UserDetected) {
		Car::writeControlMotor(Car::MoveForward, IA::Speed);
		LogD << "IAMotorBack speed "<<IA::Speed<<endl; 
	}
	else {
		Car::writeControlMotor(Car::Stop, IA::Speed);	
		LogD << "IAMotorBack No user detected "<<endl; 
	}
}
// --------------------------------------- //

// ------------ Thread management -------- //
void IA::start() {
	if(threadTest == NULL) {
		endThread = false;
		threadTest = new thread(IA::run);
	}
}

void IA::stop() {
	if(threadTest != NULL) {
		endThread = true;
		threadTest->join();
		delete threadTest;
		threadTest = NULL;
	}
}

void IA::run() {
	while(!IA::endThread) {
	IA::Dist = UserDetectionTest.detector.getDistance();
	IA::IAMotorBack(Dist);
		// sleep 
		this_thread::sleep_for(chrono::milliseconds(100));
	}
}
// --------------------------------------- //
