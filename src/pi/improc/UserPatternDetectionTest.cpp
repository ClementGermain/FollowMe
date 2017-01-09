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

void UserPatternDetectionTest::run() {
	int num_mesure = 0, num_kalman = 0;
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

		if(detector.isVisible())
		{
			x_mes_1000[0][num_mesure] = detector.Get_x_mes();
			y_mes_1000[0][num_mesure] = detector.Get_y_mes();
			r_mes_1000[0][num_mesure] = detector.Get_r_mes();
			if(++num_mesure >= 1000)
				num_mesure = 0;
		}

		if(detector.isDetected()) {
			x_mes_1000[1][num_kalman] = detector.Get_x_kalman();
			y_mes_1000[1][num_kalman] = detector.Get_y_kalman();
			r_mes_1000[1][num_kalman] = detector.Get_r_kalman();
			if(++num_kalman >= 1000)
				num_kalman = 0;
		}

		detector.imageCirclesToPosition();

		int sleep_ms = max(0, UserPattern::frameDurationMillis - (int) (t.elapsed() * 1000));
		// sleep 
		for(int s = 0; s < sleep_ms && !endThread; s++)
			this_thread::sleep_for(chrono::milliseconds(1));
	}
}


void UserPatternDetectionTest::Get_measures(){
	FILE * pFile;
	pFile = fopen ("mymeasure.csv","w");
	fprintf(pFile, "mes_x mes_y mes_r km_x km_y km_r\n");
	for(int i = 0; i < 1000; i++) {
		fprintf (pFile, "%f %f %f %f %f %f\n", x_mes_1000[0][i], y_mes_1000[0][i], r_mes_1000[0][i], x_mes_1000[1][i], y_mes_1000[1][i], r_mes_1000[1][i] );
	}
	fclose(pFile);
}

void UserPatternDetectionTest::printMeasures() {
	for(int K = 0; K < 2; K++)
	{
		// Compute means and variances (with Konig-Huygens theorem)
		float meanX = 0, meanY = 0, meanR = 0;
		float varX = 0, varY = 0, varR = 0;
		int count = 0;
		for(int i = 0; i < 1000; i++)
		{
			if(x_mes_1000[K][i] != 0.0f || y_mes_1000[K][i] != 0.0f || r_mes_1000[K][i] != 0.0f)
			{
				count++;
				meanX += x_mes_1000[K][i];
				meanY += y_mes_1000[K][i];
				meanR += r_mes_1000[K][i];
				varX += x_mes_1000[K][i] * x_mes_1000[K][i];
				varY += y_mes_1000[K][i] * y_mes_1000[K][i];
				varR += r_mes_1000[K][i] * r_mes_1000[K][i];
			}
		}
		meanX /= count;
		meanY /= count;
		meanR /= count;
		varX = varX / count - meanX * meanX;
		varY = varY / count - meanY * meanY;
		varR = varR / count - meanR * meanR;

		cout << "-- " << (K == 0 ? "Measured" : "Filtered") << "(" << count << ") --" << endl;
		cout << "Means:" << endl;
		cout << "\tx: " << meanX << endl;
		cout << "\ty: " << meanY << endl;
		cout << "\tr: " << meanR << endl;
		cout << "Variances:" << endl;
		cout << "\tx: " << varX << endl;
		cout << "\ty: " << varY << endl;
		cout << "\tr: " << varR << endl;
	}
}
