#include "KalmanFilterForUserDetection.hpp"
#include <opencv2/opencv.hpp>
#include <thread>
#include <stdio.h>
#include <vector>

using namespace cv;
using namespace std;



Kalman_Filter_User::Kalman_Filter_User() : K_Filter(3,3,0){

	//init
	K_Filter.statePre.at<float>(0) = 0;
	K_Filter.statePre.at<float>(1) = 0;
	K_Filter.statePre.at<float>(2) = 0;
	K_Filter.transitionMatrix = *(Mat_<float>(3, 3) << 1,0,0,   0,1,0,  0,0,1);
	setIdentity(K_Filter.measurementMatrix);
	setIdentity(K_Filter.processNoiseCov, Scalar::all(1e-5));
	setIdentity(K_Filter.measurementNoiseCov, Scalar::all(1.0));
	setIdentity(K_Filter.errorCovPost, Scalar::all(1));
}


Mat Kalman_Filter_User::Kalman_Filter_User_Detection(float x, float y, float r){

	// First predict, to update the internal statePre variable
	Mat prediction = K_Filter.predict(); //compute a predicted state
	//Get position x, y and radius r
	Mat measurement = Mat::zeros(3,1, CV_32F);
	measurement.at<float>(0) = x;
	measurement.at<float>(1) = y;
	measurement.at<float>(2) = r;
	//The "correct" phase that is going to  use the predicted value and our measurement
	return K_Filter.correct(measurement);

}

