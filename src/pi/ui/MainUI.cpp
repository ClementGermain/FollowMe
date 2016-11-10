#include <string>
#include <opencv2/opencv.hpp>
#include <thread>
#include <unistd.h>
#include "MainUI.hpp"
#include "Camera.hpp"
#include "CommandLine.hpp"
#include "KeyboardInput.hpp"

using namespace std;

int main() {
	runUI();
	return 0;
}

void startWaitKeyLoop_OpenCV();
void stopWaitKeyLoop_OpenCV();
int keyboardControl(istream & input, vector<int> i, vector<string> s);
int exitInterpreter(istream & input, vector<int> i, vector<string> s);
int commandMotor(istream & input, vector<int> i, vector<string> s);
int cameraPreview(istream & input, vector<int> i, vector<string> s);

Camera camera("Camera preview");

void runUI() {
	/// Initialize ///
	CommandInterpreter interpreter;
	startWaitKeyLoop_OpenCV();

	// Motor trackbars
	initializeMotorWindow();

	// Command lines
	Menu options("", 0, 0,
		new Menu("motor", 0, 0, 
			new Menu("pwm", 0, commandMotor,
				new Menu("left", 1, commandMotor, NULL),
				new Menu("right", 2, commandMotor, NULL),
				new Menu("back", 3, commandMotor, NULL),
				new Menu("front", 4, commandMotor, NULL),
				new Menu("all", 7, commandMotor, NULL),
				NULL
			),
			NULL
		),
		new Menu("keyboard", 0, keyboardControl, NULL),
		new Menu("camera", 0, 0, 
			new Menu("preview", 0, 0,
				new Menu("open", 1, cameraPreview, NULL),
				new Menu("close", 2, cameraPreview, NULL),
				NULL
			),
			NULL
		),
		new Menu("exit", 0, exitInterpreter, NULL),
		NULL
	);
	//options.print();
	interpreter.setMenu(&options);

	/// Main loop ///
	// read & execute commands
	interpreter.readCommandLines();

	/// Ending ///
	// Camera
	camera.closePreview();
	sleep(1);
	stopWaitKeyLoop_OpenCV();
}

int exitInterpreter(istream & input, vector<int> i, vector<string> s) {
	return -1;
}

int commandMotor(istream & input, vector<int> i, vector<string> s) {
	int pwm;
	input >> pwm;
	if(input.fail()) {
		cout << "Missing PWM value" << endl;
		return 1;
	}
	
	cout << "TODO send pwm to motor {";
	if(i.back() & 1)
		cout << "l";
	if(i.back() & 2)
		cout << "r";
	if(i.back() & 4)
		cout << "f";
	cout <<"} with value "<< pwm <<endl;

	return 0;
}

int cameraPreview(istream & input, vector<int> i, vector<string> s) {
	if(i.back() == 1) {
		camera.openPreview();
	}
	else if(i.back() == 2) {
		camera.closePreview();
	}
	return 0;
}

string motorTrackbarNames[6] = {
	"Left PWM",
	"Left Speed",
	"Right PWM",
	"Right Speed",
	"Front PWM",
	"Front Speed"
};

// Define button and trackbar of the control panel
// Control panel can be open with <ctrl+p>
void initializeMotorWindow() {
	cv::namedWindow("Motors", CV_WINDOW_NORMAL);
	int initValue = 0;
	
	for(int i = 0; i < 6; i++)
		cv::createTrackbar(motorTrackbarNames[i], "Motors", &initValue, 100);
}

// Update trackbar position
// motorPos : MOTOR_LEFT, MOTOR_RIGHT or MOTOR_FRONT
// type : MOTOR_PWM or MOTOR_SPEED
// value : 0-100
void updateMotorTrackbar(int motorPos, int type, int value) {
	cv::setTrackbarPos(motorTrackbarNames[motorPos*2+type], "Motors", value);
}

int keyboardControl(istream & input, vector<int> i, vector<string> s) {
	runKeyboardControl();
	return 0;
}


thread * threadWaitKey;
bool threadWaitKeyRunning = false;
void loopWaitKey() {
	while(threadWaitKeyRunning) {
		cv::waitKey(100);
	}
}

void startWaitKeyLoop_OpenCV() {
	if(!threadWaitKeyRunning) {
		threadWaitKeyRunning = true;
		threadWaitKey = new thread(loopWaitKey);
	}
}

void stopWaitKeyLoop_OpenCV() {
	if(threadWaitKeyRunning) {
		threadWaitKeyRunning = false;
		threadWaitKey->join();
		delete threadWaitKey;
	}
}
