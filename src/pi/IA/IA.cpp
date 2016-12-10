#include "IA.hpp"
#include "improc/UserPatternDetection.hpp"
#include "improc/UserPatternDetectionTest.hpp"
#include "car/Obstacle.hpp"
#include "utils/Log.hpp"
#include "car/Car.hpp"
#include <chrono>
#include <thread>
#include <algorithm>

using namespace std;

float IA::Dist = 0.0;
float IA::Speed = 0.0;
bool IA::ObstacleDetected = false;
bool IA::UserDetected = false;

thread * IA::threadTest = NULL;
bool IA::endThread = true;
float IA::RealDistance = 0.0;
float IA::TargetSpeed = 0.0;

// ---Linar function for speed control---- //
void IA::SpeedControl (float distance, bool isUserDetected){
	IA::RealDistance = distance-Car::CarSize;
	if (IA::RealDistance <= 0.5) {
		IA::Speed = 0.0;
	}
	else {
		TargetSpeed=isUserDetected * min((IA::RealDistance)*(1.0f/3.0f),1.0f);
		if (TargetSpeed - IA::Speed  > 0){
			IA::Speed = min(IA::Speed + 0.1f,IA::TargetSpeed);
		}
		else if (TargetSpeed - IA::Speed  < 0){
			IA::Speed  = max(IA::Speed - 0.1f,IA::TargetSpeed);
		}
	}
}
// --------------------------------------- //

// ---------Back motors management-------- //
void IA::IAMotorBack(float distance) {
	IA::UserDetected = UserDetectionTest.detector.isDetected();
	IA::SpeedControl(distance, IA::UserDetected);
	IA::ObstacleDetected = ObstacleDetection::isGlobalDetected();
	if (!ObstacleDetected) {
		Car::writeControlMotor(Car::MoveForward, IA::Speed);
	}
	else {
		IA::Speed = 0;
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
