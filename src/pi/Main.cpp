#include <fstream>
#include <cstdio>
#include <unistd.h>
#include <csignal>
#include <chrono>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include "ui/MainUI.hpp"
#include "car/LinkSTM32.hpp"
#include "car/Car.hpp"
#include "car/Camera.hpp"
#include "car/Obstacle.hpp"
#include "car/MotorModel.hpp"
#include "car/MotorDiagnostic.hpp"
#include "car/StateRecorder.hpp"
#include "IA/IA.hpp"
#include "sound/Sound.hpp"
#include "utils/Log.hpp"
#include "improc/UserDetectionThread.hpp"
#include "improc/RoadDetectionThread.hpp"

using namespace std;
using namespace std::chrono;

void handler(int sig) {
	// lolilol
	fprintf(stderr, "\n\"Ah bah c'est bien Nils, super le segfault ! Génial, j'te remercie.\n");
	fprintf(stderr, "Tout est foutu, c'est pas grave hein ! Oh, t'es vraiment qu'un sale\n");
	fprintf(stderr, "petit con hein ! Putain mais c'est dingue !\"\n");

	// Print fatal error in logs
	LogE << "SEGFAULT" << endl;

	// Save logs
	LogStream::Cursor cur(Log.getCursor(false));
	if(Log.hasPrevious(cur)) {
		// generate file name with date
		system_clock::time_point today = system_clock::now();
		std::time_t tt = system_clock::to_time_t ( today );
		string date(ctime(&tt));
		date.pop_back(); // remove '\n' at the end

		// create directory and file
		string dir("../../out/");
		mkdir(dir.c_str(), 0700);
		fstream file((dir+"Log "+date+" (segfault).txt").c_str(), std::fstream::out);

		// write log in file
		while(Log.hasPrevious(cur))
			file << Log.readPrevious(cur).formatedString() << endl;

		file.close();
	}

	// Release camera and sound
	Sound::stop();
	Camera::destroyAndStop();

	// Exit program
	exit(1);
}

int main() {
	// Initializations
	signal(SIGSEGV, handler);   // install our segfault handler 
	LinkSTM32 link(100);
	link.start();
	Camera::initAndStart();
     	//ObstacleDetection::start();

	// start Image Processing threads
	userDetectionThread.start();
	roadDetectionThread.detector.init();
	roadDetectionThread.start();  
	// start diagnosis thread
	Diag_Prop_Left.start();
	Diag_Prop_Right.start();
	// start recorder thread
	stateRecorder.start();
	
	// Main loop
	runUI();

	// Destroying
	stateRecorder.stop();
	IA::stop();
	Sound::stop();
	userDetectionThread.stop();
	roadDetectionThread.stop();
	ObstacleDetection::stop();
	Camera::destroyAndStop();
	link.stop();
	return 0;
}

