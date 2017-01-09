#ifndef __USER_PATTERN_DETECTION_HPP__
#define __USER_PATTERN_DETECTION_HPP__

#include <vector>
#include <opencv2/opencv.hpp>
#include "KalmanFilterForUserDetection.hpp"

class UserPattern {
	public:
		// Radius in meter
		static const float CircleRadius;
		// HSV thresholds
		static int hLo, hHi; // in range 0-180
		static int sLo, sHi; // in range 0-255
		static int vLo, vHi; // in range 0-255

		static const int maxFrameUserUndetected;
		static const int frameDurationMillis;
};

class UserPatternDetection {
	public:
		UserPatternDetection();
		void findPattern(cv::Mat & bgr_image, bool drawResult=false);
		void imageCirclesToPosition();
		bool hasImage();
		cv::Mat & getResultImage();
		cv::Mat & getFilterImage();
	
		/** The circle has been recently found but necessarly in the very last frames **/
		bool isDetected();
		/** A circle has been found in the last frame **/
		bool isVisible();
		float getDirection();
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

