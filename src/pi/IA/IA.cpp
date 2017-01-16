#include "IA.hpp"
#include "improc/RoadDetection.hpp"
#include "improc/RoadDetectionThread.hpp"
#include "improc/UserDetection.hpp"
#include "improc/UserDetectionThread.hpp"
#include "car/MotorDiagnostic.hpp"
#include "car/Obstacle.hpp"
#include "sound/Sound.hpp"
#include "utils/Log.hpp"
#include <chrono>
#include <thread>
#include <algorithm>
#include <cmath>
#include "car/Car.hpp"
#include <iostream>
#include <math.h>

using namespace std;

float IA::Speed = 0.0;
float IA::directionSpeed=0.0;
Car::Turn IA::Direction=Car::NoTurn; 
thread * IA::threadTest = NULL;
bool IA::endThread = true;
float IA::previousAngle = 0.f;
float IA::uAngleT1 =0.f;
float IA::uAngleT2 =0.f;
bool IA::enableRoadDetection = false;

//Direction = roadDetectionThread.detector.Target[0];
//Distance = roadDetectionThread.detector.Target[1];

// ---Linar function for speed control---- //
void IA::SpeedControl (float distanceUserToCamera, bool isUserDetected){
	float realDistance = distanceUserToCamera - Car::CarSize;
	const float minCriticalDistance = 0.1f; // Requirement: car must not get closer than 0.1m
	const float acceleration = 0.05f;

	// Stop instantly the car if closer than the minimal critical distance
	if(realDistance <= minCriticalDistance) {
		IA::Speed = 0.0f;
	}
	else {
		const float speedMin = 0.35f;
		const float speedMax = 1.0f;
		const float distanceMin = 0.3f;
		const float distanceMax = 1.7f;
		// hysteresis threshold: if car is moving then stop at 0.3m else start at 0.7m.
		const float distanceMinStop = IA::Speed > 0 ? distanceMin : 0.7f;

		// Get target speed
		float targetSpeed;
		if(!isUserDetected || realDistance < distanceMinStop)
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

	bool isUserDetected = userDetectionThread.detector.isDetected();
	bool isFailureLeftDetected = Diag_Prop_Left.isFailureDetected();
	bool isFailureRightDetected = Diag_Prop_Right.isFailureDetected();
	float distance;
	if (enableRoadDetection)
		distance = roadDetectionThread.detector.Target.y;
	else
		distance = userDetectionThread.detector.getDistance();
	
	IA::SpeedControl(distance, isUserDetected);

	if (!isFailureLeftDetected && !isFailureRightDetected) { 
		Car::writeControlMotor(Car::MoveForward, IA::Speed);
	}
	// otherwise, emergency brake
	else {
		IA::Speed = 0;
		Car::writeControlMotor(Car::Stop, IA::Speed);
		Car::writeControlGyro(true);
		Sound::play("../../res/music/nils.mp3");
		LogW << "Thread ended beacause of moto failure" << endl;
		endThread = true;
	}
}
// --------------------------------------- //

// ---------function for direction control -------- //

void IA::DirectionControl(float angleUserToCamera, bool isUserDetected, bool endOfCourseLeft, bool endOfCourseRight){
    if (!isUserDetected){
        directionSpeed=0.0;
    }
    else 
    {
        directionSpeed = angleUserToCamera * M_PI / 180.f;
cout << directionSpeed << endl;
    }
}

// ---------Direction motors management-------- //

void IA::IAMotorDirection(){

	float angleUserToCamera;
	if(enableRoadDetection)
		angleUserToCamera = roadDetectionThread.detector.Target.x;
	else
		angleUserToCamera = userDetectionThread.detector.getDirection();
	
	bool isUserDetected = userDetectionThread.detector.isDetected();
	bool isEndOfCourseLeft = false; // Car :: ??
	bool isEndOfCourseRight = false; //Car :: ??
	IA::DirectionControl(angleUserToCamera, isUserDetected, isEndOfCourseLeft, isEndOfCourseRight);
	//send command to direction motor
	Car::writeControlMotor(IA::Direction, IA::directionSpeed);
}

void IA::toggleRoadDetection() {
	enableRoadDetection = !enableRoadDetection;
}


// --------------------------------------- //

// ------------ Thread management -------- //
void IA::start() {
	LogI << "Starting AI..." << endl;
	Car::writeControlGyro(false);
	if(endThread) {
		if(threadTest != NULL) {
			delete threadTest;
		}
		endThread = false;
		threadTest = new thread(IA::run);
	}
}

void IA::stop() {
	if(threadTest != NULL) {
		LogI << "Joining AI thread..." << endl;
		endThread = true;
		IA::Speed=0.0f;
		IA::directionSpeed=0.0f;
		Car::writeControlGyro(false);
		Car::writeControlMotor(Car::Stop, IA::Speed);
		Car::writeControlMotor(IA::Direction, IA::directionSpeed);
		threadTest->join();
		delete threadTest;
		threadTest = NULL;
		LogI << "AI thread terminated" << endl;
	}
}

void IA::run() {
	while(!IA::endThread) {
		IA::IAMotorBack();
		IA::IAMotorDirection();
		// sleep 
		this_thread::sleep_for(chrono::milliseconds(IA_PERIOD));
	}
}
// --------------------------------------- //
