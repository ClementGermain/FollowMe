#ifndef __ROAD_DETECTION_HPP__
#define __ROAD_DETECTION_HPP__

#include "../car/Camera.hpp"
#include <opencv2/opencv.hpp>
#include <vector>


//! Size of the road matrix
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

	/*! Start an iteration of te road detection algoritm !*/
	//@Param : camera image on which the algorithm will be applied
	void applyRoadThreshold(cv::Mat image);

	/*! Return the image calculated by the road detection algorithm !*/
	cv::Mat & getImage();

	/*! Return the camera image with HUD displayed !*/
	cv::Mat & getCameraImage();
	
	/*! Return if the car can go forward without going into the grass !*/
	int canGoForward();

	//Shoudn't be there, should be replace by canGoForward function !
	bool grassDetected;

	enum{Yes, Likely, Unlikely, No, NoIdea};

private:
	/*! **** Methods **** !*/	
	
	/*! Detect the road in the rectangle !*/
	//! Caution : can only work with trapezes

	// ********************* not fully implemented yet TODO ***************	
	int roadInRect(std::vector<cv::Point> Rect);
	
	//! Detect road in front of the car
	std::vector<cv::Point> m_forwardRect;
	void drawForwardRect();	
	bool m_forwardBool;
	
	//! Color constant for display
	static const cv::Vec3b white, green, yellow, red;

	//! Project a 3D point on the camera
	//@Param : relative distance from the car (see car documentation)
	cv::Point project2D(cv::Point_<float> relativePoint);


	//! **** Attributes ***** !//
	cv::Mat m_displayedImage;
	cv::Mat m_thresholdedImage;
	cv::Mat m_cameraImage;

	//! Camera focal distance
	float m_dfx;
	float m_dfy;

};

#endif
