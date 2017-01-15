#include "KalmanFilterForUserDetection.hpp"
#include <opencv2/opencv.hpp>
#include <thread>
#include <stdio.h>
#include <vector>
#include "car/Camera.hpp"
#include "utils/Log.hpp"
#include "UserDetection.hpp"

using namespace cv;
using namespace std;



Kalman_Filter_User::Kalman_Filter_User() : K_Filter(6,3,0)
{
	/**
	 * x, y, r, vx, vy, vr, ax, ay, ar
	 * A : 6,6
	 * H : 6,3
	 * Q : 6,6
	 * R : 3,3
	 */
	/*
	LogD << "transition "<<K_Filter.transitionMatrix.size().width << " " << K_Filter.transitionMatrix.size().height << endl;
	LogD << "measurement "<<K_Filter.measurementMatrix.size().width << " " << K_Filter.measurementMatrix.size().height << endl;
	LogD << "process noise "<<K_Filter.processNoiseCov.size().width << " " << K_Filter.processNoiseCov.size().height << endl;
	LogD << "measurement noise "<<K_Filter.measurementNoiseCov.size().width << " " << K_Filter.measurementNoiseCov.size().height << endl;
	*/
	resetState(0,0,0);

	// A, should take in account speed and acceleration
	setIdentity(K_Filter.transitionMatrix);
	K_Filter.transitionMatrix = *(Mat_<float>(6, 6) <<
								1,0,0, 1,0,0, 
	 							0,1,0, 0,1,0, 
	 							0,0,1, 0,0,1, 
	
	 							0,0,0, 1,0,0, 
	 							0,0,0, 0,1,0, 
	 							0,0,0, 0,0,1); // A
	// H
	setIdentity(K_Filter.measurementMatrix);
	
	//parameters:
	float d = 0.80f;// default distance
	// Q
	updateProcessNoise(d);
	// R
	updateMeasurementNoise(d);
	// P
	setIdentity(K_Filter.errorCovPost, Scalar::all(1)); // TODO test with 0.1

}

void Kalman_Filter_User::updateMeasurementNoise(float userDistance) {
	// select radius variance from table according to the current distance
	const int nbValues = 2;
	float distances[nbValues] = {0.8, 1.8}; //----+--->  values from excel
	float variancesR[nbValues] = {31, 65}; // (9.82,3) 
	float variancesX[nbValues] = {20.25,12.98};
	float variancesY[nbValues] = {57.05,136.13};

	float varX;
	float varY;
	float varR;

	if(userDistance <= distances[0]) {
		varR = variancesR[0];
		varX = variancesX[0];
		varY = variancesY[0];
	}
	else if(userDistance >= distances[nbValues-1]) {
		varR = variancesR[nbValues-1];
		varX = variancesX[nbValues-1];
		varY = variancesY[nbValues-1];
	}
	else {
		int i = 1;
		while(userDistance > distances[i])
			i++;
		float k = (userDistance - distances[i-1]) / (distances[i] - distances[i-1]);
		varR = k * variancesR[i-1] + (1-k) * variancesR[i];
		varX = k * variancesX[i-1] + (1-k) * variancesX[i];
		varY = k * variancesY[i-1] + (1-k) * variancesY[i];
	}
	
	K_Filter.measurementNoiseCov = *(Mat_<float>(3, 3) << varX, 0, 0,
														  0, varY, 0,
														  0, 0, varR);
}

void Kalman_Filter_User::resetState(float x, float y, float r) {
	K_Filter.statePre.at<float>(0) = x;
	K_Filter.statePre.at<float>(1) = y;
	K_Filter.statePre.at<float>(2) = r;
	K_Filter.statePre.at<float>(3) = 0;
	K_Filter.statePre.at<float>(4) = 0;
	K_Filter.statePre.at<float>(5) = 0;
}

void Kalman_Filter_User::updateProcessNoise(float userDistance) {
	// Parameters for calculations
	float halfWidth = Camera::getFrameWidth() * 0.5f;
	float focalLength = halfWidth / tan(Camera::horizontalFOV/2);

	float time = (float) UserPattern::frameDurationMillis / 1000; //seconds
	float v_max_user = 1.38f; // m/s (=> 5km/h)
	float moving = time * v_max_user;
	
	float R_cible = UserPattern::CircleRadius;
	
	// Maximum x and radius change between 2 frames
	float delta_x_max = focalLength / userDistance * moving;
	float delta_r_max = (userDistance - moving > 0.8f) ? focalLength * R_cible * (1.0f / (userDistance - moving) - 1.0f / userDistance) : 1; 

	//LogI << "d max: "<< moving << " Delta X max: " << delta_x_max << "   Delta R max: " << delta_r_max << endl;

	setIdentity(K_Filter.processNoiseCov, Scalar::all(1.0)); // Q 
	K_Filter.processNoiseCov.at<float>(0,0) = delta_x_max;
	K_Filter.processNoiseCov.at<float>(1,1) = 1;//100;//Camera::getFrameHeight() * 0.02;
	K_Filter.processNoiseCov.at<float>(2,2) = delta_r_max;

	K_Filter.processNoiseCov.at<float>(3,3) = 1; // vx
	K_Filter.processNoiseCov.at<float>(4,4) = 1; // vy
	K_Filter.processNoiseCov.at<float>(5,5) = 1; // vr
}

Mat Kalman_Filter_User::correctMeasurement(float x, float y, float r){
	Mat prediction = K_Filter.predict();

	//Get position x, y and radius r
	Mat measurement = Mat::zeros(3,1, CV_32F);
	measurement.at<float>(0) = x;
	measurement.at<float>(1) = y;
	measurement.at<float>(2) = r;
	
	//The "correct" phase that is going to  use the predicted value and our measurement
	return K_Filter.correct(measurement);
}

Mat Kalman_Filter_User::predictNoMeasurement() {
	//compute a predicted state
	return K_Filter.predict();
}

