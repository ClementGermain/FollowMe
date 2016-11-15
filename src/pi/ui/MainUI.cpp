#include <string>
#include <opencv2/opencv.hpp>
#include <thread>
#include <chrono>
#include <unistd.h>
#include "MainUI.hpp"
#include "MainView.hpp"
#include "car/Camera.hpp"
#include "CommandLine.hpp"
#include "utils/Log.hpp"

using namespace std;


int exitInterpreter(istream & input, vector<int> i, vector<string> s);
int commandMotor(istream & input, vector<int> i, vector<string> s);
int cameraPreview(istream & input, vector<int> i, vector<string> s);
int openGUI(istream & input, vector<int> i, vector<string> s);
int writeLog(istream & input, vector<int> i, vector<string> s);

Camera camera("Camera preview");
MainView view(camera);

void runUI() {
	/// Initialize ///
	CommandInterpreter interpreter;

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
		new Menu("gui", 0, openGUI, NULL),
		new Menu("camera", 0, 0, 
			new Menu("preview", 0, 0,
				new Menu("open", 1, cameraPreview, NULL),
				new Menu("close", 2, cameraPreview, NULL),
				NULL
			),
			NULL
		),
		new Menu("log", 0, 0,
			new Menu("write", 0, 0,
				new Menu("I", 1, writeLog, NULL),
				new Menu("D", 2, writeLog, NULL),
				new Menu("W", 3, writeLog, NULL),
				new Menu("E", 4, writeLog, NULL),
				NULL
			),
			NULL
		),
		new Menu("exit", 0, exitInterpreter, NULL),
		NULL
	);
	interpreter.setMenu(&options);

	/// Main loop ///
	// read & execute commands
	interpreter.readCommandLines();

	/// Ending ///
	if(view.isOpen()) {
		cout << "Waiting for closure of the GUI..." << endl;
	}

	// Camera
	camera.closePreview();
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

int openGUI(istream & input, vector<int> i, vector<string> s) {
	view.open();
	return 0;
}

int writeLog(istream & input, vector<int> i, vector<string> s) {
	string text;
	input >> text;

	switch(i.back()) {
		case 1:
			LogI << text;
			break;
		case 2:
			LogD << text;
			break;
		case 3:
			LogW << text;
			break;
		case 4:
			LogE << text;
			break;
	}
	while(input >> text)
		Log << " " << text;
	Log << endl;
	return 0;
}
