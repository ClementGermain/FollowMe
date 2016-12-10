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

float IA::Speed = 0.0;
thread * IA::threadTest = NULL;
bool IA::endThread = true;

// ---Linar function for speed control---- //
void IA::SpeedControl (float distanceUserToCamera, bool isUserDetected){
	float realDistance = distanceUserToCamera - Car::CarSize;
	const float minCriticalDistance = 0.5f; // Requirement: car must not get closer than 0.5m
	const float acceleration = 0.1f;

	// Stop instantly the car if closer than 0.5m
	if(realDistance <= minCriticalDistance) {
		IA::Speed = 0.0f;
	}
	else {
		const float speedMin = 0.35f;
		const float speedMax = 1.0f;
		// hysteresis threshold: if car is moving then stop at 1m else start at 2m.
		const float distanceMin = IA::Speed > 0 ? 1.0f : 2.0f;
		const float distanceMax = 2.5f;

		// Get target speed
		float targetSpeed;
		if(isUserDetected || realDistance < distanceMin)
			targetSpeed = 0.0f;
		else if(realDistance > distanceMax)
			targetSpeed = 1.0f;
		else
			targetSpeed = (realDistance-distanceMin) / (speedMax-speedMin) + speedMin;

		// update command speed
		if(targetSpeed - IA::Speed > 0)
			IA::Speed = min(IA::Speed + acceleration, targetSpeed);
		else if(targetSpeed - IA::Speed < 0)
			IA::Speed = max(IA::Speed - acceleration, targetSpeed);
		else
			IA::Speed = targetSpeed;
	}
}
// --------------------------------------- //

// ---------Back motors management-------- //
void IA::IAMotorBack() {
	// Update speed value
	float distance = UserDetectionTest.detector.getDistance();
	bool isUserDetected = UserDetectionTest.detector.isDetected();
	IA::SpeedControl(distance, isUserDetected);

	// Send speed command if no obstacle detected
	bool obstacleDetected = ObstacleDetection::isGlobalDetected();
	if (!obstacleDetected) {
		Car::writeControlMotor(Car::MoveForward, IA::Speed);
	}
	// otherwise, emergency brake
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
		IA::IAMotorBack();
		// sleep 
		this_thread::sleep_for(chrono::milliseconds(100));
	}
}
// --------------------------------------- //
