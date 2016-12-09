#include "IA.hpp"
#include "improc/UserPatternDetection.hpp"
#include "improc/UserPatternDetectionTest.hpp"
#include "car/Obstacle.hpp"
#include "utils/Log.hpp"
#include "car/Car.hpp"
#include <chrono>
#include <thread>

using namespace std;

float IA::Dist = 0.0;
float IA::Speed = 0.0;
bool IA::ObstacleDetected = false;
bool IA::UserDetected = false;

thread * IA::threadTest = NULL;
bool IA::endThread = true;
float IA::RealDistance = 0.0;
float IA::ActualSpeed = 0.0;
float IA::TargetSpeed = 0.0;

// ---Linar function for speed control---- //
void IA::SpeedControl (float distance){
	IA::RealDistance = distance-Car::CarSize;
	if (IA::RealDistance <= 3) {
		TargetSpeed=(IA::RealDistance)*(1.0/3.0);
		if (TargetSpeed - ActualSpeed > 0){
			ActualSpeed = ActualSpeed + 0.1;
		}
		else if (TargetSpeed - ActualSpeed > 0){
			ActualSpeed = ActualSpeed - 0.1;
		}
	IA::Speed = IA::ActualSpeed;
	}
	else {
	IA::Speed = 1.0;
	}
}
// --------------------------------------- //

// ---------Back motors management-------- //
void IA::IAMotorBack(float distance) {
	IA::UserDetected = UserDetectionTest.detector.isDetected();
	IA::SpeedControl(distance);
	IA::ObstacleDetected = ObstacleDetection::isGlobalDetected();
	if (UserDetected and !ObstacleDetected) {
		Car::writeControlMotor(Car::MoveForward, IA::Speed);
	}
	else {
		Car::writeControlMotor(Car::Stop, IA::Speed);	
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
