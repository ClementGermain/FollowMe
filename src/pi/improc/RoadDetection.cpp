#include "RoadDetection.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using namespace cv;

const cv::Vec3b RoadDetection::white{255, 255, 255};
const cv::Vec3b RoadDetection::green{100, 255, 0};
const cv::Vec3b RoadDetection::yellow{0, 255, 255};
const cv::Vec3b RoadDetection::red{0, 0, 255};


RoadDetection::RoadDetection() :m_displayedImage{ROADMATROW, ROADMATCOL, CV_8UC3},
				m_thresholdedImage{ROADMATROW, ROADMATCOL, CV_8UC3}
				
{	
	//! Calculate the camera's focal distances
	m_dfy = Camera::getFrameHeight() / (2.f*tan(Camera::verticalFOV / 2.f));
	m_dfx = Camera::getFrameWidth() / (2.f*tan(Camera::horizontalFOV / 2.f));

	//! Set real point distance from car
	float py = 1.50f;
	float px = 1.f;
	
	//! Calculate y coord
	float tetay = atan2(py, Camera::PosZ);
	float dtetay = M_PI*90.f/180.f + Camera::pitch - tetay;
	float y = tan(dtetay) * m_dfy;
	
	//! Calculate x coord
	float distx = sqrt(Camera::PosZ + pow(py,2));
	float dtetax = atan2(0.5f * px, distx);	
	float x = m_dfx * tan(dtetax);

	//! Set points coordinates
	//m_forwardRect.push_back(cv::Point(Camera::getFrameWidth()/2 - x, Camera::getFrameHeight()/2 + y));
	//m_forwardRect.push_back(cv::Point(Camera::getFrameWidth()/2 + x, Camera::getFrameHeight()/2 + y));	
	m_forwardRect.push_back(project2D(cv::Point(-1*px, py)));	
	m_forwardRect.push_back(project2D(cv::Point_<float>(px, py)));

	//! Set real point distance from car
	py = 3.f;
	px = 1.f;		
	
	//! Calculate y coord
	tetay = atan2(py, Camera::PosZ);
	dtetay = M_PI*90.f/180.f + Camera::pitch - tetay;
	y = tan(dtetay) * m_dfy;
	
	//! Calculate x coord
	distx = sqrt(Camera::PosZ + pow(py,2));
	dtetax = atan2(0.5f * px, distx);	
	x = m_dfx * tan(dtetax);
	
	//! Set points coordinates	
	m_forwardRect.push_back(cv::Point(Camera::getFrameWidth()/2 + x, Camera::getFrameHeight()/2 + y));
	m_forwardRect.push_back(cv::Point(Camera::getFrameWidth()/2 - x, Camera::getFrameHeight()/2 + y));

}

RoadDetection::~RoadDetection()
{
}

int RoadDetection::canGoForward()
{
	return m_forwardBool;
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
			if (avrgV > RD_NOIDEA_MAX_BRIGHTNESS) //! Brightness to high => NoIdea
			{
				color = white;
			}
			else if ((avrgH < RD_YES_H1 or avrgH>RD_YES_H2) //! => yes
				and avrgS < RD_YES_MAX_S
				and avrgV < RD_YES_MAX_V)
			{
				color = green;
			}
			else if(avrgS < RD_LIKELY_MAX_S 		//!Likely
				and avrgV < RD_LIKELY_MAX_V)
			{
				color = yellow;			
			}
			else				//!No
			{
				color = red;
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


int RoadDetection::roadInRect(std::vector<cv::Point> Rect)
{
	/*if (Rect.size()!=4)
		return 0;
	
	int minX0 = Rect[0].x;
	int maxX0 = Rect[1].x;

	int minX1 = Rect[2].x;
	int maxX1 = Rect[3].x;

	int minY = Rect[0].y;
	int maxY = Rect[2].y;

	if (minY > maxY)
		return 0;

	for (int y = minY ;  y<maxY ; y++)
	{
		float a = (float)(maxY - y) / (maxY - minY);
		minX = Rect[0].x*(1.f-a) + Rect[3].x * a;
		maxX = Rect[1].x*(1.f-a) + Rect[2].x * a;
		
		if (minX > maxX)
			return 0;

		for (int x = minX ; x<maxX ; x++)
		{
			if (	x>0 	and x<ROADMATCOL
				y>0	and y<ROADMATROW)
			{
				//TODO
			}
		}
	}*/
	return 0;
}


cv::Point RoadDetection::project2D(cv::Point_<float> relativePoint)
{
	//! Calculate y coord
	float tetay = atan2(relativePoint.y, Camera::PosZ);
	float dtetay = M_PI*90.f/180.f + Camera::pitch - tetay;
	float y = tan(dtetay) * m_dfy;
	
	//! Calculate x coord
	float distx = sqrt(Camera::PosZ + pow(relativePoint.y,2));
	float dtetax = atan2(0.5f * abs(relativePoint.x), distx);	
	float x = m_dfx * tan(dtetax);

	//! Set points coordinates
	x = relativePoint.x >= 0 ? Camera::getFrameWidth()/2 + x : Camera::getFrameWidth()/2 - x;
	y += Camera::getFrameHeight()/2;	
	return cv::Point(x, y);
}


Mat & RoadDetection::getImage() {
	return m_displayedImage;	
}

Mat & RoadDetection::getCameraImage() {
	return m_cameraImage;	
}
