#ifndef __USER_PATTERN_DETECTION_HPP__
#define __USER_PATTERN_DETECTION_HPP__


class UserPatternDetection {
	public:
		UserPatternDetection();
		void findPattern(cv::Mat & bgr_image, bool drawResult=false);
		bool hasResultImage();
		cv::Mat & getResultImage();
	private:
		bool resultImageCreated;
		cv::Mat resultImage;
};

#endif

