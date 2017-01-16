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
		IA::Direction = Car::NoTurn;
		directionSpeed=0.0;
	}
	else {
		const float dirAcceleration = 0.4f;	//car direction can not move MORE than 0.4
	//TO BE ADJUSTED
		const float dirGain = 2.0f; 		//if angle & pre_angle far, move faster
		const float dirSpeedMin = 0.3f;
		const float dirSpeedMax = 1.0f;
		// hysteresis threshold: if car is moving then stop less than 2° else start if angle>4°.
		//const float angleMin = IA::directionSpeed > 0 ? 0.03f : 0.07f; //2° - 8°. To be adjusted
		const float angleMin = 1 * M_PI/180;
		const float angleMax = 30 * M_PI/180; //10°. To be adjusted

		float targetDirSpeed;
		if(abs(angleUserToCamera) < angleMin)
			targetDirSpeed = 0.0f; //no turn
		else if(abs(angleUserToCamera) > angleMax)
			targetDirSpeed = dirSpeedMax;
		else {
			//targetDirSpeed = dirSpeedMin;
			targetDirSpeed = (angleUserToCamera-angleMin) / (angleMax-angleMin) * (dirSpeedMax-dirSpeedMin) + dirSpeedMin;
		}

		if (endOfCourseLeft || endOfCourseRight){
			directionSpeed-=dirAcceleration;
		}				
		else {
			//update direction
			if (angleUserToCamera<-angleMin){ //user if left
				IA::Direction = Car::TurnLeft;
				targetDirSpeed=-targetDirSpeed;
			}
			else if (angleUserToCamera>angleMin)
				IA::Direction = Car::TurnRight;
			else
				IA::Direction = Car::NoTurn;

			targetDirSpeed = abs(targetDirSpeed);
			cout<<"target speed: " << targetDirSpeed <<endl;
			if (((angleUserToCamera>IA::previousAngle && IA::Direction==Car::TurnRight)||(angleUserToCamera<IA::previousAngle && IA::Direction==Car::TurnLeft)) && targetDirSpeed>dirSpeedMin)
				targetDirSpeed-=abs(abs(angleUserToCamera)-abs(IA::previousAngle))/(angleMax-angleMin)*dirGain;
			if (((angleUserToCamera<IA::previousAngle && IA::Direction==Car::TurnRight)||(angleUserToCamera>IA::previousAngle && IA::Direction==Car::TurnLeft))&& targetDirSpeed<dirSpeedMax)
				targetDirSpeed+=abs(abs(angleUserToCamera)-abs(IA::previousAngle))/(angleMax-angleMin)*dirGain;

			cout << "target speed2 : "<< targetDirSpeed <<endl;

			// update direction speed : iterative command to be smoother
			if(targetDirSpeed - IA::directionSpeed > 0)
				IA::directionSpeed = abs(min(IA::directionSpeed + dirAcceleration, targetDirSpeed)); //smooth acceleration
			else if(targetDirSpeed - IA::directionSpeed < 0)
				IA::directionSpeed = abs(max(IA::directionSpeed - dirAcceleration, targetDirSpeed)); //smooth desceleration
			else
				IA::directionSpeed = targetDirSpeed;
			cout << "real cmd : "<<directionSpeed <<endl;


		}
	}
	IA::previousAngle = angleUserToCamera;
}


void IA::DirectionControl2(float angleUserToCamera, bool isUserDetected, bool endOfCourseLeft, bool endOfCourseRight){
    const float maxSpeed = 0.5f;
    const float minSpeed = 0.1f;
    
    if (!isUserDetected){
        IA::Direction = Car::NoTurn;
        directionSpeed=0.0;
    }
    else {
        //float angularSpeed = (angleUserToCamera + uAngleT1 + uAngleT2) / (0.002f * IA_PERIOD);
        float angularSpeed = (angleUserToCamera - uAngleT2) / (0.002f * IA_PERIOD);
		
		float aST1 = (angleUserToCamera - uAngleT1) / (0.001f * IA_PERIOD);
		cout << "Predicted angular delta :" << angularSpeed << endl;
		cout << "aST1:" << aST1 << endl;
		//float angularTarget = angleUserToCamera * Speed * 1.f;
		float angularTarget = angleUserToCamera / (0.001f * IA_PERIOD);
        
		cout << "Angular delta targeted :" << angularTarget << endl;
        float deltaAngularSpeed = angularTarget - angularSpeed;
        IA::directionSpeed = deltaAngularSpeed * 0.15f;       
     
        /* Set direction*/
        IA::Direction = IA::directionSpeed <= 0.f ? Car::TurnLeft : Car::TurnRight; 
      
        /* Clamp direction speed */
        
        cout << IA::directionSpeed << endl;
        IA::directionSpeed = abs(IA::directionSpeed);
        IA::directionSpeed = min (IA::directionSpeed, maxSpeed);
        IA::directionSpeed = IA::directionSpeed < minSpeed ? 0 : IA::directionSpeed ;
        cout << IA::directionSpeed << endl << endl;
        IA::directionSpeed = 0.f; 
    }
    
    uAngleT2 = uAngleT1;
    uAngleT1 = angleUserToCamera;
}

void IA::DirectionControl3(float angleUserToCamera, bool isUserDetected, bool endOfCourseLeft, bool endOfCourseRight){
    const float dAngle = 0.15f;
    const float hist = 0.15f /2.f;

    //cout << angleUserToCamera << endl;

    if (!isUserDetected){
        IA::Direction = Car::NoTurn;
        directionSpeed=0.0;
    }
    else {
        if (IA::directionSpeed != 0)
        {
            IA::directionSpeed = 0;
        }
        else
        {
			if (uAngleT1 <= angleUserToCamera - hist
					and uAngleT1 < 0.3f)
			{
				IA::Direction = Car::TurnRight;
				//cout << "LEFT" << endl;
				uAngleT1 += dAngle;
				directionSpeed=1.0;
			}
			else if (uAngleT1 > angleUserToCamera + hist 
					and uAngleT1 > -0.3f)
			{
				IA::Direction = Car::TurnLeft;
				//cout << "Right" << endl;
				uAngleT1 -=dAngle;		
				directionSpeed=1.0;
			}
		}
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
	IA::DirectionControl3(angleUserToCamera, isUserDetected, isEndOfCourseLeft, isEndOfCourseRight);
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
