#ifndef __USER_PATTERN_DETECTION_HPP__
#define __USER_PATTERN_DETECTION_HPP__


class UserPatternDetection {
	public:
		UserPatternDetection();
		void findPattern(cv::Mat & bgr_image, bool drawResult=false);
		bool hasImage();
		cv::Mat & getResultImage();
		cv::Mat & getFilterImage();
	private:
		bool resultImageCreated;
		cv::Mat resultImage;
		cv::Mat filterImage;
};

#endif

