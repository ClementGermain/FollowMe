#ifndef __ROAD_DETECTION_HPP__
#define __ROAD_DETECTION_HPP__

#include "../car/Camera.hpp"
#include <opencv2/opencv.hpp>

class RoadDetection
{
public:
	RoadDetection();
	~RoadDetection();

	applyRoadThreshold(cv::Mat image);
	cv::Mat & RoadDetection::getImage();

private:
	cv::Mat m_tresholdedImage
};

#endif
