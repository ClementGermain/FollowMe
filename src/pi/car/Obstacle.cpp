#include "Obstacle.hpp"
#include <chrono>
#include <thread>

using namespace std;

bool ObstacleDetection::Left = false;
bool ObstacleDetection::Center = false;
bool ObstacleDetection::Right = false;
bool ObstacleDetection::Global = false;

// ------------- US Left ----------------- //
void ObstacleDetection::ObstacleDetectionLeft() {
	BarstowModel_Typedef model;
	Car::getModelStructure(model);
	if (model.frontLeftUSensor.distance < 100) {
		Left = true;
	}
	else {
		Left = false;
	}
};
bool ObstacleDetection::IsLeftDetected() {
	return Left;
};
// --------------------------------------- //

// ------------- US Center --------------- //			
void ObstacleDetection::ObstacleDetectionCenter() {
	BarstowModel_Typedef model;
	Car::getModelStructure(model);
	if (model.frontCenterUSensor.distance < 100) {
		Center = true;
	}
	else {
		Center = false;
	}
};
bool ObstacleDetection::IsCenterDetected() {
	return Center;
};
// --------------------------------------- //

// ------------- US Right ---------------- //			
void ObstacleDetection::ObstacleDetectionRigth() {
	BarstowModel_Typedef model;
	Car::getModelStructure(model);
	if (model.frontRightUSensor.distance < 100) {
		Right = true;
	}
	else {
		Right = false;
	}
};
bool ObstacleDetection::IsRightDetected() {
	return Right;
};
// --------------------------------------- //

// ------------- US Global --------------- //			
void ObstacleDetection::ObstacleDetectionGlobal() {
	if (Left or Center or Right){
		Global = true;
	}
	else {
		Global = false;
	}
};
bool ObstacleDetection::IsGlobalDetected(){
	return Global;
}
// --------------------------------------- //

// ------------ Thread management -------- //
ObstacleDetection::ObstacleDetection() :
	endThread(true),
	threadTest(NULL)
{

}

ObstacleDetection::~ObstacleDetection() {
	stop();
}

void ObstacleDetection::start() {
	if(threadTest == NULL) {
		endThread = false;
		threadTest = new thread([this] { this->run(); });
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
	while(!endThread) {
ObstacleDetection::ObstacleDetectionLeft();
ObstacleDetection::ObstacleDetectionCenter();
ObstacleDetection::ObstacleDetectionRigth(); 
ObstacleDetection::ObstacleDetectionGlobal();
		// sleep 
			this_thread::sleep_for(chrono::milliseconds(100));
	}
}
