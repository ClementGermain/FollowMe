#ifndef __ROAD_DETECTION_HPP__
#define __ROAD_DETECTION_HPP__

#include "../car/Camera.hpp"
#include <opencv2/opencv.hpp>

#define ROADMATCOL 16
#define ROADMATROW 12

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
