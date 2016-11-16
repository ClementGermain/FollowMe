#ifndef __USER_PATTERN_DETECTION_HPP__
#define __USER_PATTERN_DETECTION_HPP__


class UserPatternDetection {
	public:
		UserPatternDetection();
		void findPattern(cv::Mat & bgr_image, bool drawResult=false);
		cv::Mat & getImage();
	private:
		cv::Mat resultImage;
};

#endif

