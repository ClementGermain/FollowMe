#include <fstream>
#include <chrono>
#include <string>
#include <opencv2/opencv.hpp>
#include <thread>
#include <chrono>
#include <unistd.h>
#include "MainUI.hpp"
#include "MainView.hpp"
#include "car/Camera.hpp"
#include "sound/Sound.hpp"
#include "CommandLine.hpp"
#include "utils/Log.hpp"
#include "improc/UserPatternDetectionTest.hpp"
#include "improc/RoadDetectionTest.hpp"
#include "sound/Sound.hpp"

using namespace std;
using namespace std::chrono;

/** Menu function prototypes **/
int exitInterpreter(istream & input, vector<int> i, vector<string> s);
int commandMotor(istream & input, vector<int> i, vector<string> s);
int openGUI(istream & input, vector<int> i, vector<string> s);
int writeLog(istream & input, vector<int> i, vector<string> s);
int saveLog(istream & input, vector<int> i, vector<string> s);
int tailLog(istream & input, vector<int> i, vector<string> s);
int runSound(istream & input, vector<int> i, vector<string> s);
int userDetectionSettings(istream & input, vector<int> i, vector<string> s);

// Local global variable
MainView view;

void runUI() {
	/// Initialize ///
	CommandInterpreter interpreter;

	// Define the commands
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
		new Menu("user", 0, 0,
			new Menu("red", 1, userDetectionSettings, NULL),
			new Menu("yellow", 2, userDetectionSettings, NULL),
			NULL
		),
		new Menu("gui", 0, openGUI, NULL),
		new Menu("log", 0, 0,
			new Menu("write", 0, 0,
				new Menu("I", 1, writeLog, NULL),
				new Menu("D", 2, writeLog, NULL),
				new Menu("W", 3, writeLog, NULL),
				new Menu("E", 4, writeLog, NULL),
				NULL
			),
			new Menu("save", 0, saveLog, NULL),
			new Menu("tail", 0, tailLog, NULL),
			NULL
		),
		new Menu("music", 0, 0, 
			new Menu ("play", 1, runSound, NULL),
			new Menu ("stop", 2, runSound, NULL),
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
		view.close();
	}
}

int exitInterpreter(istream & input, vector<int> i, vector<string> s) {
	// Return -1 to close the program
	return -1;
}

int commandMotor(istream & input, vector<int> i, vector<string> s) {
	// Read pwm value
	int pwm;
	input >> pwm;
	if(input.fail()) {
		cout << "Missing PWM value" << endl;
		return 1;
	}
	
	// Read motor target
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

int openGUI(istream & input, vector<int> i, vector<string> s) {
	// Open the graphic window
	view.open();
	return 0;
}

int writeLog(istream & input, vector<int> i, vector<string> s) {
	// Write text to log
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

int saveLog(istream & input, vector<int> i, vector<string> s) {
	// Save log in a file
	
	// Create a filename with current date and time
	system_clock::time_point today = system_clock::now();
	std::time_t tt = system_clock::to_time_t ( today );
	
	string date(ctime(&tt));
	string dir("../../out/");
	fstream file((dir+"Log "+date+".txt").c_str(), std::fstream::out);

	// Write log content
	LogStream::Cursor cur(Log.getCursor(false));
	while(Log.hasPrevious(cur))
		file << Log.readPrevious(cur).formatedString() << endl;

	file.close();
	return 0;
}

int tailLog(istream & input, vector<int> i, vector<string> s) {
	// Print the last lines of the log

	// read the parameter: number of line to print, default value is 10
	int lineCount;
	if(!(input >> lineCount))
		lineCount = 10;

	// Move a cursor to 'end-lineCount'
	LogStream::Cursor cur(Log.getCursor(false));
	int realLineCount = 0;
	while(--lineCount >= 0 && Log.hasPrevious(cur)) {
		Log.readPrevious(cur);
		realLineCount++;
	}

	// Write colored lines in standard output
	while(realLineCount--) {
		cout << Log.readNext(cur).coloredString(true) << endl;
	}

	return 0;
}

int runSound(istream & input, vector<int> i, vector<string> s) {
	// Read a music, a file name can be given in parameter
	// filename of the music
	string musicName;
	
	switch (i.back()){
		case 1:
			if(input >> musicName)
				Sound::play(musicName);
			else
				Sound::play();
			break;
		case 2:
			Sound::stop();
			break;
	}
	return 0;
}

int userDetectionSettings(istream & input, vector<int> i, vector<string> s) {
	switch(i.back()) {
		case 1: // red
			UserPattern::hLo = 160;//35*180/255;
			UserPattern::sLo = 70;
			UserPattern::vLo = 50;
			UserPattern::hHi = 5;//58*180/255;
			UserPattern::sHi = 255;
			UserPattern::vHi = 255;
			break;
		case 2: // yellow
			UserPattern::hLo = 35*180/255;
			UserPattern::sLo = 70;
			UserPattern::vLo = 50;
			UserPattern::hHi = 58*180/255;
			UserPattern::sHi = 255;
			UserPattern::vHi = 255;
			break;
	}
	return 0;
}
