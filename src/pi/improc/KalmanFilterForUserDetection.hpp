#ifndef __KALMAN_FILTER_FOR_USER_DETECTION_HPP__
#define __KALMAN_FILTER_FOR_USER_DETECTION_HPP__

#include <opencv2/opencv.hpp>
#include <vector>
#include <stdio.h>

class Kalman_Filter_User{
	public:
		Kalman_Filter_User() ; //constructor to initialize 
		cv::Mat Kalman_Filter_User_Detection(float x, float y, float r);
	private:
		cv::KalmanFilter K_Filter;
};

#endif
