#include "RoadDetection.h"

using namespace cv;

RoadDetection::RoadDetection()
{
}


RoadDetection::~RoadDetection()
{
}


void RoadDetection::applyRoadThreshold(Mat image)
{
	int iLowH = 200;
	int iHighH = 250;

	int iLowS = 0;
	int iHighS = 100;

	int iLowV = 0;
	int iHighV = 210;

	Mat imageHSV;
	cvtColor(image, imageHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

	inRange(imageHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), m_thresholdedImage); //Threshold the image

}

Mat & RoadDetection::getImage() {
	return m_thresholdedImage;
}