#include <opencv2/opencv.hpp>
#include <thread>
#include <chrono>
#include "utils/Timer.hpp"
#include "utils/Log.hpp"
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
	while(!endThread) {
		cv::Mat img;
#ifdef __NO_RASPI__
		img = cv::imread("../../res/img/red_circle3.png");
#else
		Camera::getImage(img);
#endif

		Timer t;
		if(img.data)
			detector.findPattern(img, true);
		detector.imageCirclesToPosition();
		
		int sleep_ms = max(0, UserPattern::frameDurationMillis - (int) (t.elapsed() * 1000));
		// sleep 
		for(int s = 0; s < sleep_ms && !endThread; s++)
			this_thread::sleep_for(chrono::milliseconds(1));
	}
}

