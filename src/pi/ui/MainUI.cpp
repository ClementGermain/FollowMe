#include <string>
#include <opencv2/opencv.hpp>
#include "MainUI.hpp"
#include "Camera.hpp"
#include "CommandLine.hpp"

using namespace std;
int main() {
	runUI();
	return 0;
}

int keyboardControl(istream & input, vector<int> i, vector<string> s);
int exitInterpreter(istream & input, vector<int> i, vector<string> s);
int commandMotor(istream & input, vector<int> i, vector<string> s);

CommandInterpreter interpreter;

void runUI() {
	/// Initialize ///
	// Camera
	Camera camera("Camera preview");
	camera.openPreview();

	// Motor trackbars
	initializeMotorWindow();

	// Command lines
	Menu options("", 0, 0,
		new Menu("motor", 0, 0, 
			new Menu("pwm", 0, commandMotor,
				new Menu("left", 1, commandMotor),
				new Menu("right", 2, commandMotor),
				new Menu("back", 3, commandMotor),
				new Menu("front", 4, commandMotor),
				new Menu("all", 7, commandMotor),
				NULL
			),
			NULL
		),
		new Menu("keyboard", 0, keyboardControl),
		new Menu("exit", 0, exitInterpreter),
		NULL
	);
	interpreter.setMenu(&options);

	/// Main loop ///
	// read & execute commands
	interpreter.readCommandLines();

	/// Ending ///
	// Camera
	camera.closePreview();
}

int exitInterpreter(istream & input, vector<int> i, vector<string> s) {
	interpreter.finish();
	return 0;
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
	cout << "TODO keyboard control"<<endl;
}
