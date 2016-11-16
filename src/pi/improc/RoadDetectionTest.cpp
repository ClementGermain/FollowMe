#include <opencv2/opencv.hpp>
#include <thread>
#include <chrono>
#include "RoadDetectionTest.hpp"
#include "car/Camera.hpp"

using namespace std;

RoadDetectionTest RoadDetectionTest;

RoadDetectionTest::RoadDetectionTest() :
	endThread(true),
	threadTest(NULL)
{

}

RoadDetectionTest::~RoadDetectionTest() {
	stop();
}

void RoadDetectionTest::start() {
	if(threadTest == NULL) {
		endThread = false;
		threadTest = new thread([this] { this->run(); });
	}
}

void RoadDetectionTest::stop() {
	if(threadTest != NULL) {
		endThread = false;
		threadTest->join();
		delete threadTest;
	}
}

void RoadDetectionTest::run() {
	while(!endThread) {
		cv::Mat img;
		RaspiCam.getImage(img);
		detector.applyRoadThreshold(img, true);
		
		// sleep 100ms
		this_thread::sleep_for(chrono::milliseconds(100));
	}
}

