#include <opencv2/opencv.hpp>
#include <thread>
#include <chrono>
#include "UserPatternDetectionTest.hpp"
#include "car/Camera.hpp"

using namespace std;

UserPatternDetectionTest UserDetectionTest;

UserPatternDetectionTest::UserPatternDetectionTest() :
	endThread(true),
	threadTest(NULL)
{

}

UserPatternDetectionTest::~UserPatternDetectionTest() {
	stop();
}

void UserPatternDetectionTest::start() {
	if(threadTest == NULL) {
		endThread = false;
		threadTest = new thread([this] { this->run(); });
	}
}

void UserPatternDetectionTest::stop() {
	if(threadTest != NULL) {
		endThread = true;
		threadTest->join();
		delete threadTest;
		threadTest = NULL;
	}
}

void UserPatternDetectionTest::run() {
	int i = 0;
	const char * names[6] = {
		"../../res/img/u1.png",
		"../../res/img/u2.png",
		"../../res/img/u3a.png",
		"../../res/img/u4.png",
		"../../res/img/u5.png",
		"../../res/img/u6.png"
	};
	while(!endThread) {
		cv::Mat img;
#ifdef __NO_RASPI__
		img = cv::imread(names[i%6]);
#else
		RaspiCam.getImage(img);
#endif
		detector.findPattern(img, true);
		
		// sleep 
		for(int s = 0; s < 50 && !endThread; s++)
			this_thread::sleep_for(chrono::milliseconds(100));
		i++;
	}
}

