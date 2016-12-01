#include "IA.hpp"
#include "improc/UserPatternDetection.hpp"
#include "improc/UserPatternDetectionTest.hpp"
#include "utils/Log.hpp"
#include "car/Car.hpp"
#include <chrono>
#include <thread>

using namespace std;

float IA::Dist = 0.0;
bool IA::UserDetected = false;

thread * IA::threadTest = NULL;
bool IA::endThread = true;

// ---------Back motors management-------- //
void IA::IAMotorBack(float distance) {
	LogD << "IAMotorBack distance "<<distance<<endl;
	float speed = 0.5f;
	float fastspeed = 1.0f;
	IA::UserDetected = UserDetectionTest.detector.isDetected();
	if (UserDetected) {
		if (distance >= 0.0 and distance <0.5) {
				Car::writeControlMotor(Car::Stop, speed);	
				LogD << "IAMotorBack Stop "<<endl; 
		}
		else if (distance >= 0.5 and distance <1.5) {
				Car::writeControlMotor(Car::MoveForward, speed);	
				LogD << "IAMotorBack MediumSpeed "<<endl; 
		}
		else if (distance >= 1.5) {
				Car::writeControlMotor(Car::MoveForward, fastspeed);	
				LogD << "IAMotorBack FullSpeed "<<endl; 
		}
	}
	else {
		Car::writeControlMotor(Car::Stop, speed);	
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
