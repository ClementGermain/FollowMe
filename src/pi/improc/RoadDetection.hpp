
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

typedef struct{
	//! each of the following values represent the fuzzy value of the 4 classes
	//! They all must be from 0 to 1
	float Yes;
	float Likely;
	float Unlikely;
	float No;
	float NoIdea;
}  fuzzyModel_TypeDef;

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

	/*! Compute next way point !*/
	//@Param  : user position relative to the car (x,y) in meters
	//@Return : next accessible way point to follow the user 
	cv::Point computeNextWayPoint(cv::Point_<float>  userPosition);

	/*! Return if the car can go forward without going into the grass !*/
	int canGoForward();

	/*! Return a new direction with a safe road */
	float getGrassFreeDirection(void);

	/*! return True if the algorithm successed to find a safe path !*/
	bool pathFound(void);

	/*Shoudn't be there, should be replace by canGoForward function ! !*/
	bool grassDetected;

	//! Project a 3D point on the camera
	//@Param : relative distance from the car (see car documentation)
	cv::Point project2D(cv::Point_<float> relativePoint);

	//! Get the real distance from a point on the camera
	//@Param : coordinates x and y
	cv::Point unproject2D(cv::Point_<float> Point);


	enum{Yes, Likely, Unlikely, No, NoIdea};

private:
	/*! **** Methods **** !*/	
	
	/*! Detect the road beetween 4 points !*/
	int roadInQuad(cv::Point topLeft, cv::Point topRight, cv::Point bottomRight, cv::Point bottomLeft);
	cv::Point maxDistInPath(cv::Point topLeft, cv::Point topRight, cv::Point bottomRight, cv::Point bottomLeft);
	
	//! Detect road in front of the car
	std::vector<cv::Point> m_forwardRect;
	std::vector<cv::Point> m_path;
	void drawForwardRect();
	void drawPath();

	//! This function as to determine if pts is in Path
	bool isInPath(cv::Point pts);

	fuzzyModel_TypeDef m_fuzzyPath;

	//! Do a fuzzy analyse of the content of the path */
	//! This function as to update the FuzzyState struct
	void pathContentAnalyse(void);

	//! determine if the path is ok to go or not based on the fuzzyPath struct
	bool pathIsOk(void);

	//! Create a new path by rotating the current path
	//	@ param : angle in degres
	void calculNewPath(int angle);

	bool m_forwardBool;

	//! Color constant for display
	static const cv::Vec3b white, green, yellow, red, blue, orange;



	//! **** Attributes ***** !//
	cv::Mat m_displayedImage;
	cv::Mat m_thresholdedImage;
	cv::Mat m_cameraImage;

	//! Camera focal distance
	float m_dfx;
	float m_dfy;

};

#endif
