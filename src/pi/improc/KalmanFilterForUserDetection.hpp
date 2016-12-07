#ifndef __KALMAN_FILTER_FOR_USER_DETECTION_HPP__
#define __KALMAN_FILTER_FOR_USER_DETECTION_HPP__

#include <opencv2/opencv.hpp>
#include "UserPatternDetection.hpp"
#include <vector>
#include <stdio.h>

class Kalman_Filter_User{
public:
			static int x; //lateral position of the user
			static int y; // longitudinal position
			static int r; // radius 

			Kalman_Filter_User() ; //constructor to initialize 
			Kalman_Filter_User_Detection();

};
