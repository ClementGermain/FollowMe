#include "RoadDetection.hpp"

#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

/*RoadDetection::RoadDetection() : m_thresholdedImage{ROADMATROW, ROADMATCOL, 0}
{
}*/
RoadDetection::RoadDetection() :m_displayedImage{ROADMATROW, ROADMATCOL, CV_8UC3},
				m_thresholdedImage{ROADMATROW, ROADMATCOL, CV_8UC3}
				
{	
	//! Calculate the camera's focal distances
	float dfy = Camera::getFrameHeight() / (2.f*tan(Camera::verticalFOV / 2.f));
	float dfx = Camera::getFrameWidth() / (2.f*tan(Camera::horizontalFOV / 2.f));

	//! Set real point distance from car
	float py = 1.50f;
	float px = 1.f;		
	
	//! Calculate y coord
	float tetay = atan2(py, Camera::PosZ);
	float dtetay = M_PI*90.f/180.f + Camera::pitch - tetay;
	float y = tan(dtetay) * dfy;
	
	//! Calculate x coord
	float distx = sqrt(Camera::PosZ + pow(py,2));
	float dtetax = atan2(0.5f * px, distx);	
	float x = dfx * tan(dtetax);

	//! Set points coordinates
	m_forwardRect.push_back(cv::Point(Camera::getFrameWidth()/2 - x, Camera::getFrameHeight()/2 + y));
	m_forwardRect.push_back(cv::Point(Camera::getFrameWidth()/2 + x, Camera::getFrameHeight()/2 + y));

	//! Set real point distance from car
	py = 3.f;
	px = 1.f;		
	
	//! Calculate y coord
	tetay = atan2(py, Camera::PosZ);
	dtetay = M_PI*90.f/180.f + Camera::pitch - tetay;
	y = tan(dtetay) * dfy;
	
	//! Calculate x coord
	distx = sqrt(Camera::PosZ + pow(py,2));
	dtetax = atan2(0.5f * px, distx);	
	x = dfx * tan(dtetax);
	
	//! Set points coordinates	
	m_forwardRect.push_back(cv::Point(Camera::getFrameWidth()/2 + x, Camera::getFrameHeight()/2 + y));
	m_forwardRect.push_back(cv::Point(Camera::getFrameWidth()/2 - x, Camera::getFrameHeight()/2 + y));

}

RoadDetection::~RoadDetection()
{
}

int RoadDetection::canGoForward()
{
	return 0;	
}

void RoadDetection::drawForwardRect()
{
	polylines(m_cameraImage,  m_forwardRect, true, 0x6CB8FD);
}

void RoadDetection::applyRoadThreshold(Mat image)
{
	//! Init default detection parameters !//
	
	//! Convert the captured frame from BGR to HSV !//
	Mat imageHSV;
	cvtColor(image, imageHSV, COLOR_BGR2HSV); 

	//! Check image size !//
	if (imageHSV.rows%ROADMATROW != 0 or imageHSV.rows%ROADMATROW != 0)
	{
		return;
	}

	int rowsPerRect = image.rows / ROADMATROW;
	int colsPerRect = imageHSV.cols / ROADMATCOL;
	int pxPerRect = rowsPerRect * colsPerRect;

	//! Iterate for each element of the result matrix !//
	for (int matRow = 0 ; matRow < ROADMATROW ; matRow ++)
	{
		for (int matCol = 0 ; matCol < ROADMATCOL ; matCol ++)
		{
			
			//! Iterate for each pixel of the image !//
			int avrgH = 0, avrgS = 0, avrgV = 0;			

			for (	int pxRow = matRow*rowsPerRect ;
				pxRow < (matRow+1)*rowsPerRect ;
				pxRow ++
				)
			{
				for (	int pxCol = matCol*colsPerRect ;
					pxCol < (matCol+1)*colsPerRect ;
					pxCol ++
					)	
				
				{
					Vec3b intensity = imageHSV.at<Vec3b>(pxRow, pxCol);
					avrgH += intensity.val[0];
					avrgS += intensity.val[1];
					avrgV += intensity.val[2];
				}
			}
			
			//! Calculates road presence probability
			avrgH /= pxPerRect;
			avrgS /= pxPerRect;
			avrgV /= pxPerRect;

			Vec3b color;			
			if (avrgV > 180) //! Brightness to high
			{
				color[0] = 255;
				color[1] = 255;
				color[2] = 255;
			}
			else if ((avrgH < 35 or avrgH>85) //! Road
				and avrgS < 30
				and avrgV < 180)
			{
				color[0] = 100;
				color[1] = 255;
				color[2] = 0;
			}
			else if(avrgS < 80 //!Maybe road
				and avrgV < 255)
			{
				color[0] = 0;
				color[1] = 255;
				color[2] = 255;			
			}
			else
			{
				color[0] = 0;
				color[1] = 0;
				color[2] = 255;
			}
			
			//! Write the results into the Matrix !//
			m_thresholdedImage.at<Vec3b>(Point(matCol,matRow)) = color;
		}
	}
	m_thresholdedImage.copyTo(m_displayedImage);
	//resize(image, m_displayedImage, Size(ROADMATCOL, ROADMATROW), 0, 0, INTER_AREA);
	
	//! Copy img to buffer !//
	image.copyTo(m_cameraImage);
	drawForwardRect();
}


/*void RoadDetection::applyRoadThreshold(Mat image)
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
	inRange(imageHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), threshold1); //Threshold the image
	
	iLowH = 85;
	iHighH = 180;
	inRange(imageHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), threshold2);

	threshold = threshold1 + threshold2;

	Mat thresholdedImage;
	resize(threshold, thresholdedImage, Size(ROADMATCOL, ROADMATROW), 0, 0, INTER_AREA);

	// Interpolate new image with previous result
	addWeighted(m_thresholdedImage, (1-OBLIVIOUSNESS), thresholdedImage, OBLIVIOUSNESS, 0, m_thresholdedImage);

	Mat front = m_thresholdedImage(Range(ROADMATCOL/4, ROADMATCOL*3/4), Range(ROADMATROW/2, ROADMATROW-1));// yolo
	int sumOfFront = sum(front)[0];
	grassDetected = sumOfFront >= (ROADMATROW/2 * ROADMATCOL/2) * 255 * 50/100; // detected if at least 50% of the middle down area of image is grass
	
}*/

Mat & RoadDetection::getImage() {
	return m_displayedImage;	
}

Mat & RoadDetection::getCameraImage() {
	return m_cameraImage;	
}
