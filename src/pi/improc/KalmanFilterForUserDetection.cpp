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
	K_Filter.transitionMatrix = *(Mat_<float>(3, 3) << 1,0,0,   0,1,0,  0,0,1); //A
	setIdentity(K_Filter.measurementMatrix);// H = I
	
	setIdentity(K_Filter.processNoiseCov, Scalar::all(5.0));//Q 
	K_Filter.processNoiseCov.at<float>(2,2) = 1.0;
	
	setIdentity(K_Filter.measurementNoiseCov, Scalar::all(1.0));  //R
	K_Filter.measurementNoiseCov.at<float>(2,2) = 5.0;
		
	//K_Filter.measurementNoiseCov = *(Mat_<float>(3, 3) << 	1,0,0,   1,1,0,  			0,0,1); 
	
	setIdentity(K_Filter.errorCovPost, Scalar::all(1));//P


//init

		var_x=0.0;
		var_y=0.0;
		var_r=0.0;
	
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


void Kalman_Filter_User::Mesure_parameters_calcul_KF(float xm, float ym, float rm, float x_mes[1000], float y_mes[1000], float r_mes[1000]){

	for(int i=0; i<1000; i++){ 
		var_x= var_x +(x_mes[i] - xm);
		var_y =var_y +(y_mes[i] - ym); 
		var_r =var_r + (r_mes[i] -rm);
		}

	var_x= var_x/1000;
	var_y =var_y/1000; 
	var_r =var_r/1000;




}

