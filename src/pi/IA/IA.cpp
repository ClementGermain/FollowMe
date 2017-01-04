#include "IA.hpp"
#include "improc/UserPatternDetection.hpp"
#include "improc/UserPatternDetectionTest.hpp"
#include "car/Obstacle.hpp"
#include "utils/Log.hpp"
#include <chrono>
#include <thread>
#include <algorithm>
#include <cmath>
#include "car/Car.hpp"

using namespace std;

float IA::Speed = 0.0;
float IA::directionSpeed=0.0;
Car::Turn IA::Direction=Car::NoTurn; 
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
		if(!isUserDetected || realDistance < distanceMin)
			targetSpeed = 0.0f;
		else if(realDistance > distanceMax)
			targetSpeed = 1.0f;
		else
			targetSpeed = (realDistance-distanceMin) / (distanceMax-distanceMin) * (speedMax-speedMin) + speedMin;

		// update command speed
		if(targetSpeed - IA::Speed > 0)
			IA::Speed = min(IA::Speed + acceleration, targetSpeed); //acceleration
		else if(targetSpeed - IA::Speed < 0)
			IA::Speed = max(IA::Speed - acceleration, targetSpeed); //desceleration
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
	// ignore obstacleDetected because not reliable
	if (!obstacleDetected || true) {
		Car::writeControlMotor(Car::MoveForward, IA::Speed);
	}
	// otherwise, emergency brake
	else {
		IA::Speed = 0;
		Car::writeControlMotor(Car::Stop, IA::Speed);	
	}
}
// --------------------------------------- //

// ---------function for direction control -------- //

void IA::DirectionControl(float angleUserToCamera, bool isUserDetected, bool endOfCourseLeft, bool endOfCourseRight){
	
	if (!isUserDetected){
		IA::Direction = Car::NoTurn;
		directionSpeed=0.0;
	}
	else {
		const float dirAcceleration = 0.1f;
		const float dirSpeedMin = 0.35f;
		const float dirSpeedMax = 0.6f;
		// hysteresis threshold: if car is moving then stop less than 2° else start if angle>4°.
		//const float angleMin = IA::directionSpeed > 0 ? 0.03f : 0.07f; //2° - 8°. To be adjusted
		const float angleMin = 5 * M_PI/180;
		const float angleMax = 30 * M_PI/180; //10°. To be adjusted

		// Get target direction speed : PI !!
		float targetDirSpeed;
		if(abs(angleUserToCamera) < angleMin)
			targetDirSpeed = 0.0f; //no turn
		else if(abs(angleUserToCamera) > angleMax)
			targetDirSpeed = dirSpeedMax;
		else {
			//a modifier
			targetDirSpeed = dirSpeedMin;
			//targetSpeed = (realDistance-distanceMin) / (distanceMax-distanceMin) * (speedMax-speedMin) + speedMin;
		}


		if (endOfCourseLeft || endOfCourseRight){
			directionSpeed-=dirAcceleration;
		}				
		else {
			//update direction
			if (angleUserToCamera<-angleMin) //user if left
				IA::Direction = Car::TurnLeft;
			else if (angleUserToCamera>angleMin)
				IA::Direction = Car::TurnRight;
			else
				IA::Direction = Car::NoTurn;

			// update direction speed : iterative command to be smoother
			if(targetDirSpeed - IA::directionSpeed > 0)
				IA::directionSpeed = min(IA::directionSpeed + dirAcceleration, targetDirSpeed); //smooth acceleration
			else if(targetDirSpeed - IA::directionSpeed < 0)
				IA::directionSpeed = max(IA::directionSpeed - dirAcceleration, targetDirSpeed); //smooth desceleration
			else
				IA::directionSpeed = targetDirSpeed;


		}
	}
}

// --------------------------------------- //

// ---------Direction motors management-------- //

void IA::IAMotorDirection(){
	float angleUserToCamera = UserDetectionTest.detector.getDirection();
	bool isUserDetected = UserDetectionTest.detector.isDetected();
	bool isEndOfCourseLeft = false; // Car :: ??
	bool isEndOfCourseRight = false; //Car :: ??
	IA::DirectionControl(angleUserToCamera, isUserDetected, isEndOfCourseLeft, isEndOfCourseRight);
	//send command to direction motor
	Car::writeControlMotor(IA::Direction, IA::directionSpeed);
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
		IA::IAMotorDirection();
		// sleep 
		this_thread::sleep_for(chrono::milliseconds(100));
	}
}
// --------------------------------------- //
