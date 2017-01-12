#include <opencv2/opencv.hpp>
#include <thread>
#include <chrono>
#include "RoadDetectionTest.hpp"
#include "car/Camera.hpp"

using namespace std;

RoadDetectionTest roadDetectionTest;

RoadDetectionTest::RoadDetectionTest() :
	endThread(true),
	threadTest(NULL)
{

}

RoadDetectionTest::~RoadDetectionTest() {
}

void RoadDetectionTest::start() {
	if(threadTest == NULL) {
		endThread = false;
		threadTest = new thread([this] { this->run(); });
	}
}

void RoadDetectionTest::stop() {
	if(threadTest != NULL) {
		endThread = true;
		threadTest->join();
		delete threadTest;
		threadTest = NULL;
	}
}

void RoadDetectionTest::run() {
	while(!endThread) {
		cv::Mat img;

#ifdef __NO_RASPI__
		img = cv::imread("../../res/img/route.bmp");
#else
		Camera::getImage(img);
#endif

		detector.applyRoadThreshold(img);
		
		// sleep 100ms
		this_thread::sleep_for(chrono::milliseconds(100));
	}
}

