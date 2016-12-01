#include "Obstacle.hpp"
#include "car/Car.hpp"
#include "../../stm32/KeilProject/Sources/Barstow/Control.h"
#include <ctime>
#include <chrono>
#include <thread>

using namespace std;

bool ObstacleDetection::Left = false;
bool ObstacleDetection::Center = false;
bool ObstacleDetection::Right = false;
bool ObstacleDetection::Global = false;
time_t ObstacleDetection::Timer = time(0);
time_t ObstacleDetection::Delta = time(0);  
thread * ObstacleDetection::threadTest = NULL;
bool ObstacleDetection::endThread = true;

// ------------- US Left ----------------- //
void ObstacleDetection::obstacleDetectionLeft() {
	BarstowModel_Typedef model;
	Car::getModelStructure(model);
	if (model.frontLeftUSensor.distance < 20) {
		Left = true;
	}
	else {
		Left = false;
	}
};
bool ObstacleDetection::isLeftDetected() {
	return Left;
};
// --------------------------------------- //

// ------------- US Center --------------- //			
void ObstacleDetection::obstacleDetectionCenter() {
	BarstowModel_Typedef model;
	Car::getModelStructure(model);
	if (model.frontCenterUSensor.distance < 20) {
		Center = true;
	}
	else {
		Center = false;
	}
};
bool ObstacleDetection::isCenterDetected() {
	return Center;
};
// --------------------------------------- //

// ------------- US Right ---------------- //			
void ObstacleDetection::obstacleDetectionRight() {
	BarstowModel_Typedef model;
	Car::getModelStructure(model);
	if (model.frontRightUSensor.distance < 20) {
		Right = true;
	}
	else {
		Right = false;
	}
};
bool ObstacleDetection::isRightDetected() {
	return Right;
};
// --------------------------------------- //

// ------------- US Global --------------- //			
void ObstacleDetection::obstacleDetectionGlobal() {
	/*BarstowControl_Typedef control;
	Car::getControlStructure(control);*/
	if (Left or Center or Right){
		Global = true;
		ObstacleDetection::Delta = time(0);
		//control.gyro = 1;
	}
	else {
		ObstacleDetection::obstacleDetectionGlobalTimed();
	}
	/*Car::updateControlStructure(control);*/
};
bool ObstacleDetection::isGlobalDetected(){
	return Global;
}
// --------------------------------------- //

// ---------US Global time related-------- //	
void ObstacleDetection::obstacleDetectionGlobalTimed() {
	/*BarstowControl_Typedef control;
	Car::getControlStructure(control);*/
	ObstacleDetection::Timer = time(0);	
	if (difftime(ObstacleDetection::Timer,ObstacleDetection::Delta) < 1){ 
		Global = true;
		//control.gyro = 1;
	}
	else {
		Global = false;
		//control.gyro = 0;
	}	
	/*Car::updateControlStructure(control);*/
}
// --------------------------------------- //

// ------------ Thread management -------- //
void ObstacleDetection::start() {
	if(threadTest == NULL) {
		endThread = false;
		threadTest = new thread(ObstacleDetection::run);
	}
}

void ObstacleDetection::stop() {
	if(threadTest != NULL) {
		endThread = true;
		threadTest->join();
		delete threadTest;
		threadTest = NULL;
	}
}

void ObstacleDetection::run() {
	while(!ObstacleDetection::endThread) {
		ObstacleDetection::obstacleDetectionLeft();
		ObstacleDetection::obstacleDetectionCenter();
		ObstacleDetection::obstacleDetectionRight(); 
		ObstacleDetection::obstacleDetectionGlobal();
		// sleep 
		this_thread::sleep_for(chrono::milliseconds(100));
	}
}
