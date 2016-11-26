#ifndef __ROAD_DETECTION_HPP__
#define __ROAD_DETECTION_HPP__

#include "../car/Camera.hpp"
#include <opencv2/opencv.hpp>

#define ROADMATCOL 32
#define ROADMATROW 24

#define OBLIVIOUSNESS 0.8

class RoadDetection
{
public:
	RoadDetection();
	~RoadDetection();

	void applyRoadThreshold(cv::Mat image);
	cv::Mat & getImage();

private:
	cv::Mat m_thresholdedImage;
};

#endif
