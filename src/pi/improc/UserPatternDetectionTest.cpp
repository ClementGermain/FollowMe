#include <opencv2/opencv.hpp>
#include <thread>
#include <chrono>
#include "utils/Timer.hpp"
#include "utils/Log.hpp"
#include "UserPatternDetectionTest.hpp"
#include "car/Camera.hpp"
#include <stdio.h>

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

float* UserPatternDetectionTest::Get_x_mes_1000(){ return x_mes_1000;}
float* UserPatternDetectionTest::Get_y_mes_1000(){ return y_mes_1000;}
float* UserPatternDetectionTest::Get_r_mes_1000(){ return r_mes_1000;}

void UserPatternDetectionTest::run() {
		int num_mesure=0;
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
			if (num_mesure>=1000)
				num_mesure = 0;
			if((detector.Get_x_mes() != x_mes_1000[num_mesure - 1]) || (detector.Get_y_mes() != y_mes_1000[num_mesure - 1]) || (detector.Get_r_mes() != r_mes_1000[num_mesure - 1])) {

			x_mes_1000[num_mesure] = detector.Get_x_mes();
			y_mes_1000[num_mesure] = detector.Get_y_mes();
			r_mes_1000[num_mesure] = detector.Get_r_mes();
			num_mesure++;
}

		
		detector.imageCirclesToPosition();
		
		int sleep_ms = max(0, UserPattern::frameDurationMillis - (int) (t.elapsed() * 1000));
		// sleep 
		for(int s = 0; s < sleep_ms && !endThread; s++)
			this_thread::sleep_for(chrono::milliseconds(1));
	}
	UserPatternDetectionTest::Get_measures();
}


void UserPatternDetectionTest::Get_measures(){
	FILE * pFile;
	pFile = fopen ("mymeasure.csv","w");
		for(int i=0;i<1000; i++) {
			fprintf (pFile, "%f,%f, %f \n",x_mes_1000[i],y_mes_1000[i], r_mes_1000[i] );
			}
fclose(pFile);

}

