#ifndef __ROAD_DETECTION_HPP__
#define __ROAD_DETECTION_HPP__

#include "../car/Camera.hpp"
#include <opencv2/opencv.hpp>
#include <vector>

#define ROADMATCOL 32
#define ROADMATROW 24


//! Road detection parameters
#define RD_NOIDEA_MAX_BRIGHTNESS 180
#define RD_YES_H1 35
#define RD_YES_H2 85
#define RD_YES_MAX_S 30
#define RD_YES_MAX_V 180
#define RD_LIKELY_MAX_S 80
#define RD_LIKELY_MAX_V 255

class RoadDetection
{
public:
	RoadDetection();
	~RoadDetection();

	void applyRoadThreshold(cv::Mat image);
	cv::Mat & getImage();
	cv::Mat & getCameraImage();
	
	int canGoForward();

	bool grassDetected;

	enum{Yes, Likely, Unlikely, No, NoIdea};
private:
	
	cv::Mat m_displayedImage;
	cv::Mat m_thresholdedImage;
	cv::Mat m_cameraImage;

	std::vector<cv::Point> m_forwardRect;

	void drawForwardRect();
	bool m_forwardBool;
	
	//! Color constant for display
	static const cv::Vec3b white, green, yellow, red;
};

#endif
