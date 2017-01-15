#include <fstream>
#include <chrono>
#include <string>
#include <opencv2/opencv.hpp>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <cstdlib>
#include "MainUI.hpp"
#include "MainView.hpp"
#include "car/Camera.hpp"
#include "car/MotorModel.hpp"
#include "car/MotorDiagnostic.hpp"
#include "car/StateRecorder.hpp"
#include "sound/Sound.hpp"
#include "CommandLine.hpp"
#include "utils/Log.hpp"
#include "improc/UserDetectionThread.hpp"
#include "improc/RoadDetectionThread.hpp"
#include "sound/Sound.hpp"
#include "IA/IA.hpp"

using namespace std;
using namespace std::chrono;

/** Menu function prototypes **/
int exitInterpreter(istream & input, vector<int> i, vector<string> s);
int printLogoFollowMe(istream & input, vector<int> i, vector<string> s);
int openGUI(istream & input, vector<int> i, vector<string> s);
int writeLog(istream & input, vector<int> i, vector<string> s);
int saveLog(istream & input, vector<int> i, vector<string> s);
int tailLog(istream & input, vector<int> i, vector<string> s);
int runSound(istream & input, vector<int> i, vector<string> s);
int userDetectionSettings(istream & input, vector<int> i, vector<string> s);
int runIA(istream & input, vector<int> i, vector<string> s);
int toggleRoadDetectionIA(istream & input, vector<int> i, vector<string> s);
int runModelAcquire(istream & input, vector<int> i, vector<string> s);
int runDiag(istream & input, vector<int> i, vector<string> s);
int saveUserData(istream & input, vector<int> i, vector<string> s);
int printUserData(istream & input, vector<int> i, vector<string> s);
int resetUserData(istream & input, vector<int> i, vector<string> s);
int toggleModeUserDetection(istream & input, vector<int> i, vector<string> s);
int saveStateRecord(istream & input, vector<int> i, vector<string> s);

// Local global variable
MainView view;

void runUI() {
	LogI << "Opening UI (command prompt)..." << endl; 

	/// Initialize ///
	CommandInterpreter interpreter;

	// Define the commands
	Menu options("", 0, 0,
		new Menu("user", 0, 0,
			new Menu("red", 1, userDetectionSettings, NULL),
			new Menu("yellow", 2, userDetectionSettings, NULL),
			new Menu("save", 0, saveUserData, NULL),
			new Menu("print", 0, printUserData, NULL),
			new Menu("reset", 0, resetUserData, NULL),
			new Menu("mode", 0, toggleModeUserDetection, NULL),
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
		new Menu("ai", 0, 0, 
			new Menu ("start", 1, runIA, NULL),
			new Menu ("stop", 2, runIA, NULL),
			new Menu("road", 0, toggleRoadDetectionIA, NULL),
			NULL
		),
		new Menu("record", 0, saveStateRecord, NULL),
		new Menu("ModelAcquire", 0, runModelAcquire, NULL),
		new Menu("logo", 0, printLogoFollowMe, NULL),
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


int printLogoFollowMe(istream & input, vector<int> i, vector<string> s) {
	// Print pretty logo
	system("head -15 ../../res/ascii/logo.txt");
	return 0;
}

int openGUI(istream & input, vector<int> i, vector<string> s) {
	// Open the graphic window
	view.open();
	cout << "Open GUI." << endl;
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
	date.pop_back(); // remove '\n' at the end
	string dir("../../out/");
	string filename(dir+"Log "+date+".txt");
	cout << "Write log in \"" << filename << "\"" << endl;
	ofstream file(filename);

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
			cout << "Let's play some music!" << endl;
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

int runIA(istream & input, vector<int> i, vector<string> s) {
	// Start the IA for following an user

	switch (i.back()){
		case 1:
			cout << "Start AI" << endl;
			IA::start();
			break;
		case 2:
			cout << "Stop AI" << endl;
			IA::stop();
			break;
	}
	return 0;
}

int runModelAcquire(istream & input, vector<int> i, vector<string> s){
  cout << "Acquiring model for diagnosis, please wait..." << endl;
  
  MotorModel model;
  model.create(-1.0, 1.0, 10000);
  model.save("model_propulsion");

  cout << "Saved in \"model_propulsion.bin\"" << endl;
  
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

int saveUserData(istream & input, vector<int> i, vector<string> s) {
	cout << "Save user detection measures" << endl;
	userDetectionThread.Get_measures();
	return 0;
}

int printUserData(istream & input, vector<int> i, vector<string> s) {
	userDetectionThread.printMeasures();
	return 0;
}

int resetUserData(istream & input, vector<int> i, vector<string> s) {
	cout << "Reset user detection measures" << endl;
	userDetectionThread.resetMeasures();
	return 0;
}

int toggleModeUserDetection(istream & input, vector<int> i, vector<string> s) {
	cout << "Toggle user detection mode" << endl;
	userDetectionThread.detector.toggleMode();
	return 0;
}

int toggleRoadDetectionIA(istream & input, vector<int> i, vector<string> s) {
	IA::toggleRoadDetection();
	cout << "Road detection is now " << (IA::enableRoadDetection ? "enabled" : "disabled") << endl;
	return 0;
}

int saveStateRecord(istream & input, vector<int> i, vector<string> s) {
	string filename;
	if(input >> filename)
		stateRecorder.save(filename);
	else
		stateRecorder.save();
	return 0;
}
