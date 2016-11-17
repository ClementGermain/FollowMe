#include "RoadDetection.hpp"

using namespace cv;

RoadDetection::RoadDetection()
{
}


RoadDetection::~RoadDetection()
{
}


void RoadDetection::applyRoadThreshold(Mat image)
{
	int iLowH = 0;
	int iHighH = 255;

	int iLowS = 0;
	int iHighS = 30;

	int iLowV = 40;
	int iHighV = 180;

	Mat imageHSV;
	cvtColor(image, imageHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
	//cvtColor(imageHSV, m_thresholdedImage, COLOR_HSV2RGB);
	inRange(imageHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), m_thresholdedImage); //Threshold the image

}

Mat & RoadDetection::getImage() {
	return m_thresholdedImage;
}
