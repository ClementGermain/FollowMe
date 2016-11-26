#include "RoadDetection.hpp"

using namespace cv;

RoadDetection::RoadDetection() : m_thresholdedImage{ROADMATROW, ROADMATCOL, 0}
{
}


RoadDetection::~RoadDetection()
{
}


void RoadDetection::applyRoadThreshold(Mat image)
{
	int iLowH = 0;
	int iHighH = 35;

	int iLowS = 0;
	int iHighS = 110;

	int iLowV = 0;
	int iHighV = 255;

	Mat imageHSV;
	cvtColor(image, imageHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
	
	Mat threshold, threshold1, threshold2;
	inRange(imageHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), threshold1 /*m_thresholdedImage*/); //Threshold the image
	iLowH = 85;
	iHighH = 180;
	inRange(imageHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), threshold2);

	threshold = threshold1 + threshold2;

	Mat thresholdedImage;
	resize(threshold, thresholdedImage, Size(ROADMATCOL, ROADMATROW), 0, 0, INTER_AREA);


	thresholdedImage = thresholdedImage * OBLIVIOUSNESS;
	m_thresholdedImage = m_thresholdedImage * (1 - OBLIVIOUSNESS) + thresholdedImage;

}

Mat & RoadDetection::getImage() {
	return m_thresholdedImage;
}

