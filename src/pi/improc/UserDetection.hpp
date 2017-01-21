#ifndef __USER_PATTERN_DETECTION_HPP__
#define __USER_PATTERN_DETECTION_HPP__

#include <vector>
#include <opencv2/opencv.hpp>
#include "KalmanFilterForUserDetection.hpp"

/**
 * This class contains global parameters conncerning the user circle to detect
 */
class UserPattern {
	public:
		//! Radius of the disk in meter
		static const float CircleRadius;
		//! Hue thresholds (0-180) of the circle color
		static int hLo, hHi;
		//! Saturation thresholds (0-255) of the circle color
		static int sLo, sHi;
		//! Value thresholds (0-255) of the circle color
		static int vLo, vHi;

		//! The maximum number of consecutive frames during which the user is considered as detected meanwhile it is not visible.
		static const int maxFrameUserUndetected;
		//! Duration of a frame
		static const int frameDurationMillis;
};

/**
 * Algorithm to get the position of the user relatively to the car only from the camera image.
 */
class UserDetection {
	public:
		UserDetection();
		//! Detected the circle on the image (position and size), with kalman filter.
		void findPattern(cv::Mat & bgr_image, bool drawResult=false);
		//! Compute user position relatively to car from the result of findPattern(cv::Mat, bool)
		void imageCirclesToPosition();
		bool hasImage();
		//! Camera image with the detected circle in green and the filtered circle in blue.
		cv::Mat & getResultImage();
		//! Black and white image that isolates the color of the user circle.
		cv::Mat & getFilterImage();
	
		/** The circle has been recently found but necessarly in the very last frames **/
		bool isDetected();
		/** A circle has been found in the last frame **/
		bool isVisible();
		//! return the direction of the user in radians
		float getDirection();
		//! return the distance of the user in meters
		float getDistance();
		float x_mes;
		float y_mes;
		float r_mes;
		float Get_x_mes();
		float Get_y_mes();
		float Get_r_mes();
		float Get_x_kalman();
		float Get_y_kalman();
		float Get_r_kalman();
		void toggleMode();
	private:
		bool resultImageCreated;
		cv::Mat resultImage;
		cv::Mat filterImage;
		
		int frameCountSinceUserUndetected;
		cv::Vec3f filteredCircle;
		bool isUserDetected;
		float detectedDirection, detectedDistance;
		Kalman_Filter_User kalmanFilter;
		bool useRevolutionnaryMode;
};

#endif

