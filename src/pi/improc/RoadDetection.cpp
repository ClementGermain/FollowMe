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
const cv::Vec3b RoadDetection::blue{255, 0, 0};


RoadDetection::RoadDetection() :m_displayedImage{ROADMATROW, ROADMATCOL, CV_8UC3},
				m_thresholdedImage{ROADMATROW, ROADMATCOL, CV_8UC3}
				
{	
	//! Calculate the camera's focal distances
	m_dfy = Camera::getFrameHeight() / (2.f*tan(Camera::verticalFOV / 2.f));
	m_dfx = Camera::getFrameWidth() / (2.f*tan(Camera::horizontalFOV / 2.f));

	//! Set real point distance from car
	float py = 1.f;
	float px = 0.8f;
	
	//! Set points coordinates	
	m_forwardRect.push_back(project2D(cv::Point_<float>(-1*px, py)));	
	m_forwardRect.push_back(project2D(cv::Point_<float>(px, py)));

	//! Set real point distance from car
	py = 3.f;
	px = 0.8f;		
	
	//! Set points coordinates
	m_forwardRect.push_back(project2D(cv::Point_<float>(px, py)));
	m_forwardRect.push_back(project2D(cv::Point_<float>(-1*px, py)));

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
	//polylines(m_cameraImage,  m_forwardRect, true, 0x6CB8FD);
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
	
	//! Copy img to buffer !//
	image.copyTo(m_cameraImage);
	drawForwardRect();
	roadInQuad(m_forwardRect[3], m_forwardRect[2], m_forwardRect[1], m_forwardRect[0]);
}

int RoadDetection::roadInQuad(cv::Point topLeft, cv::Point topRight, cv::Point bottomRight, cv::Point bottomLeft)
{
	//! Create the bounding box of the quad !//
	int bMinX = min(topLeft.x, 	bottomLeft.x);
	int bMaxX = max(topRight.x, 	bottomRight.x);
	int bMinY = min(topLeft.y, 	topRight.y);
	int bMaxY = max(bottomLeft.y, 	bottomRight.y);
	
	//! Display the bounding box !//
	/*vector<cv::Point> shape;
	shape.push_back(cv::Point(bMinX, bMinY));
	shape.push_back(cv::Point(bMaxX, bMinY));
	shape.push_back(cv::Point(bMaxX, bMaxY));
	shape.push_back(cv::Point(bMinX, bMaxY));
	polylines(m_cameraImage,  shape, true, 0x198423);*/
	
	//! Scale points to the threshold image !//
	float scaleX = (float)ROADMATCOL / (float)Camera::getFrameWidth();
	float scaleY = (float)ROADMATROW / (float)Camera::getFrameHeight();
	bMinX *= scaleX;
	bMaxX *= scaleX;
	bMinY *= scaleY;
	bMaxY *= scaleY;

	//! Test each pixel in the bounding box !//
	for (int x=bMinX ; x<=bMaxX ; x++)
	{
		float xAdv = (float)(x-bMinX) / (float)(bMaxX - bMinX);
		for (int y=bMinY; y<=bMaxY ; y++)
		{
			float yAdv 	= (float)(y-bMinY) / (float)(bMaxY - bMinY);
			float left 	= scaleX * (topLeft.x * (1-yAdv) + bottomLeft.x * yAdv);
			float right 	= scaleX * (topRight.x * (1-yAdv) + bottomRight.x * yAdv);
			float top	= scaleY * (topLeft.y * (1-xAdv) + topRight.y * xAdv);
			float bottom	= scaleY * (bottomLeft.y * (1-xAdv) + bottomRight.y * xAdv);
			
			//! Check that the pixel is inside the image		
			if (x>=0 and x<ROADMATCOL and y>=0 and y<ROADMATROW)
			{
				//! Check that the pixel is inside the quad
				if (x>left and x<right and y>top and y<bottom)
				{
					m_displayedImage.at<Vec3b>(Point(x,y)) = blue;
				}
			}			
			
		}
	}
	
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
	float dtetax = atan2(0.5f * relativePoint.x, distx);	
	float x = m_dfx * tan(dtetax);

	//! Set points coordinates
	x += Camera::getFrameWidth()/2;	
	y += Camera::getFrameHeight()/2;	
	return cv::Point(x, y);
}


Mat & RoadDetection::getImage() {
	return m_displayedImage;	
}

Mat & RoadDetection::getCameraImage() {
	return m_cameraImage;	
}
