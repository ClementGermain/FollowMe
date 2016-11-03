#include "MainUI.hpp"
#include "Camera.hpp"
#include "CommandLine.hpp"

using namespace std;

void runUI() {
	/// Initialize ///
	// Camera
	Camera camera("Camera preview");
	camera.startPreview();

	// Motor trackbars
	initializeMotorWindow();

	/// Main loop ///
	// read & execute commands
	readCommandsLines();

	/// Ending ///
	// Camera
	camera.stopPreview();
}

const char * motorTrackbarNames[6] = {
	"Left PWM",
	"Left Speed",
	"Right PWM",
	"Right Speed",
	"Front PWM",
	"Front Speed"
}

// Define button and trackbar of the control panel
// Control panel can be open with <ctrl+p>
void initializeMotorWindow() {
	cv::namedWindow("Motors", CV_WINDOW_NORMAL);
	int initValue = 0;
	
	for(int i = 0; i < 6; i++)
		cv::createTrackbar(motorTrackbarNames+i, "Motors", &initValue, 100);
}

// Update trackbar position
// motorPos : MOTOR_LEFT, MOTOR_RIGHT or MOTOR_FRONT
// type : MOTOR_PWM or MOTOR_SPEED
// value : 0-100
void updateMotorTrackbar(int motorPos, int type, int value) {
	cv::setTrackbarPos(motorTrackbarNames[motorPos*2+type], "Motors", value);
}


