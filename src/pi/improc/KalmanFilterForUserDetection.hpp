#ifndef __KALMAN_FILTER_FOR_USER_DETECTION_HPP__
#define __KALMAN_FILTER_FOR_USER_DETECTION_HPP__

#include <opencv2/opencv.hpp>
#include <vector>
#include <stdio.h>

class Kalman_Filter_User{
	public:
		Kalman_Filter_User() ; //constructor to initialize 
		cv::Mat Kalman_Filter_User_Detection(float x, float y, float r);
		//void Mesure_parameters_calcul_KF(float xm, float ym, float rm, float x_mes[1000], float y_mes[1000], float r_mes[1000]);
		
	private:
		cv::KalmanFilter K_Filter;
};

#endif
