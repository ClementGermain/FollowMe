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
		endThread = false;
		threadTest->join();
		delete threadTest;
	}
}

void UserPatternDetectionTest::run() {
	while(!endThread) {
		cv::Mat img;
		RaspiCam.getImage(img);
		detector.findPattern(img, true);
		// sleep 100ms
		this_thread::sleep_for(chrono::milliseconds(100));
	}
}

