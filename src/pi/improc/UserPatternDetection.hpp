#ifndef __USER_PATTERN_DETECTION_HPP__
#define __USER_PATTERN_DETECTION_HPP__

#include <vector>

class UserPattern {
	public:
		// Radius in meter
		static const float CircleRadius;
		// HSV thresholds
		static const int hLo, hHi; // in range 0-180
		static const int sLo, sHi; // in range 0-255
		static const int vLo, vHi; // in range 0-255
};

class UserPatternDetection {
	public:
		UserPatternDetection();
		void findPattern(cv::Mat & bgr_image, bool drawResult=false);
		void imageCirclesToPosition();
		bool hasImage();
		cv::Mat & getResultImage();
		cv::Mat & getFilterImage();
	
	private:
		bool resultImageCreated;
		cv::Mat resultImage;
		cv::Mat filterImage;
		std::vector<cv::Vec3f> imageCircles;
};

#endif

