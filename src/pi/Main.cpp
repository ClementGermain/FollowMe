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
#include "IA/IA.hpp"
#include "sound/Sound.hpp"
#include "utils/Log.hpp"
#include "improc/UserPatternDetectionTest.hpp"
#include "improc/RoadDetectionTest.hpp"

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
	system_clock::time_point today = system_clock::now();
	std::time_t tt = system_clock::to_time_t ( today );
	
	string date(ctime(&tt));
	string dir("../../out/");
	mkdir(dir.c_str(), 0700);
	fstream file((dir+"Log "+date+"(after segfault).txt").c_str(), std::fstream::out);

	LogStream::Cursor cur(Log.getCursor(false));
	while(Log.hasPrevious(cur))
		file << Log.readPrevious(cur).formatedString() << endl;

	file.close();

	// Release camera and sound
	Sound::stop();
	Camera::destroy();

	// Exit program
	exit(1);
}

int main() {
	// Initializations
	signal(SIGSEGV, handler);   // install our segfault handler 
	LinkSTM32 link(100);
	Camera::init();
	ObstacleDetection::start();
	IA::start();

	// start Image Processing threads
	UserDetectionTest.start();
	roadDetectionTest.start();

	// Main loop
	runUI();

	// Destroying
	Camera::destroy();
	Sound::stop();
	UserDetectionTest.stop();
	roadDetectionTest.stop();
	ObstacleDetection::stop();
	IA::stop();
	return 0;
}

