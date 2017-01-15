#include <opencv2/opencv.hpp>
#include <thread>
#include <chrono>
#include "RoadDetectionThread.hpp"
#include "car/Camera.hpp"
#include "utils/Log.hpp"

using namespace std;

RoadDetectionThread roadDetectionThread;

RoadDetectionThread::RoadDetectionThread() :
	PeriodicThread(100, "Road Detection")
{

}

void RoadDetectionThread::loop() {
	cv::Mat img;

#ifdef __NO_RASPI__
	img = cv::imread("../../res/img/route.bmp");
#else
	Camera::getImage(img);
#endif

	detector.applyRoadThreshold(img);
}

