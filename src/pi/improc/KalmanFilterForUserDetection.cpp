#include "KalmanFilterForUserDetection.hpp"
#include <opencv2/opencv.hpp>
#include <thread>
#include <stdio.h>
#include <vector>
#include "car/Camera.hpp"

using namespace cv;
using namespace std;



Kalman_Filter_User::Kalman_Filter_User() : K_Filter(3,3,0){

	//init
	K_Filter.statePre.at<float>(0) = 0;
	K_Filter.statePre.at<float>(1) = 0;
	K_Filter.statePre.at<float>(2) = 0;
	K_Filter.transitionMatrix = *(Mat_<float>(3, 3) << 1,0,0,   0,1,0,  0,0,1); //A
	setIdentity(K_Filter.measurementMatrix);// H = I
	
	//parameters:
	float halfWidth = Camera::getFrameWidth() * 0.5f;
	float focalLength = halfWidth / tan(Camera::horizontalFOV/2);
	float v_max_user=5; //m per seconds
	float time =0.1; //seconds
	float d=1.80;
	float delta_x_max = focalLength * 1/(d * v_max_user * time); 
	float delta_r_max =focalLength * 1/(d * v_max_user * time); 
	setIdentity(K_Filter.processNoiseCov, Scalar::all(5.0));//Q 
	K_Filter.processNoiseCov.at<float>(2,2) = 0.0;
	K_Filter.processNoiseCov.at<float>(1,1) = delta_x_max;
	K_Filter.processNoiseCov.at<float>(3,3) = delta_r_max;
	K_Filter.processNoiseCov.at<float>(4,4) = 1;
	K_Filter.processNoiseCov.at<float>(5,5) = 1;
	
	//setIdentity(K_Filter.measurementNoiseCov, Scalar::all(1.0));  //R
	//K_Filter.measurementNoiseCov.at<float>(2,2) = 5.0;
	K_Filter.measurementNoiseCov = *(Mat_<float>(3, 3) << 	50,0,0,   0,50,0,  0,0,65); //TAB EXCEL
	setIdentity(K_Filter.errorCovPost, Scalar::all(1));//P

	
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




