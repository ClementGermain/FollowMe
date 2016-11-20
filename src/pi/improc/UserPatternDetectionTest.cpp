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

		Timer t;
		detector.findPattern(img, true);
		LogI << "Processed image for user pattern detection in "<<t.elapsed()<<" seconds"<<endl;
		detector.imageCirclesToPosition();

		// sleep 
		for(int s = 0; s < 100 && !endThread; s++)
			this_thread::sleep_for(chrono::milliseconds(100));
		i++;
	}
}

