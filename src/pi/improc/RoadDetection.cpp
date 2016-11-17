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

	int iLowV = 5;
	int iHighV = 180;

	Mat imageHSV;
	cvtColor(image, imageHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
	//cvtColor(imageHSV, m_thresholdedImage, COLOR_HSV2RGB);
	
	Mat threshold;
	inRange(imageHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), threshold); //Threshold the image

/*	//morphological opening (remove small objects from the foreground)
	erode(m_thresholdedImage, m_thresholdedImage, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	dilate(m_thresholdedImage, m_thresholdedImage, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	//morphological closing (fill small holes in the foreground)
	dilate(m_thresholdedImage, m_thresholdedImage, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	erode(m_thresholdedImage, m_thresholdedImage, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
*/
	resize(threshold, m_thresholdedImage, Size(64, 48), 0, 0, INTER_AREA);
}

Mat & RoadDetection::getImage() {
	return m_thresholdedImage;
}
