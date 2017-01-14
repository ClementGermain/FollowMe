#include "RoadDetection.hpp"
#include "UserPatternDetection.hpp"
#include "UserPatternDetectionTest.hpp"
#include "car/Car.hpp"
#include "utils/Log.hpp"

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
const cv::Vec3b RoadDetection::orange{0, 127, 255};


RoadDetection::RoadDetection() :m_displayedImage{ROADMATROW, ROADMATCOL, CV_8UC3},
				m_thresholdedImage{ROADMATROW, ROADMATCOL, CV_8UC3}
				
{	

}

RoadDetection::~RoadDetection()
{
}

int RoadDetection::canGoForward()
{
	return m_forwardBool;
}

void RoadDetection::init()
{

	//! Calculate the camera's focal distances
	m_dfy = Camera::getFrameHeight() / (2.f*tan(Camera::verticalFOV / 2.f));
	m_dfx = Camera::getFrameWidth() / (2.f*tan(Camera::horizontalFOV / 2.f));

	//! Set real point distance from car
	float py = Car::CarSize*1.2f;
	float px = Car::CarWidth*0.4f;
	
	//! Set points coordinates	
	m_forwardRect.push_back(project2D(cv::Point_<float>(-1*px, py)));	
	m_forwardRect.push_back(project2D(cv::Point_<float>(px, py)));

	//! Set real point distance from car
	py = 3.f;
	px = Car::CarWidth;		
	
	//! Set points coordinates
	m_forwardRect.push_back(project2D(cv::Point_<float>(px, py)));
	m_forwardRect.push_back(project2D(cv::Point_<float>(-1*px, py)));
	return;
}

cv::Point RoadDetection::computeNextWayPoint(cv::Point_<float>  userPosition)
{
 
	//! Project user real position on road matrix !//
	cv::Point userCamPos = project2D(userPosition);

	//! Scale points to the threshold image !//
	float scaleX = (float)ROADMATCOL / (float)Camera::getFrameWidth();
	float scaleY = (float)ROADMATROW / (float)Camera::getFrameHeight();
	
	cv::Point userMatPos;
	userMatPos.x = scaleX * userCamPos.x;
	userMatPos.y = scaleY * userCamPos.y;

	//! Display the user on the interface !//
	if (userMatPos.x >=0 and userMatPos.x <ROADMATCOL and userMatPos.y>=0 and userMatPos.y <=ROADMATROW)
            m_displayedImage.at<Vec3b>(Point(userMatPos.x,userMatPos.y)) = blue;

        //! Get the best next point
        bool best = false;
        int minDist;
        
        cv::Point_<float> targetPoint = userPosition;
        
        cv::Point wayPointLeft = project2D(cv::Point_<float>{targetPoint.x - Car::CarWidth*0.8f, targetPoint.y});
        cv::Point wayPointRight = project2D(cv::Point_<float>{targetPoint.x + Car::CarWidth*0.8f, targetPoint.y});
            
        cv::Point bestPoint = maxDistInPath(wayPointLeft, wayPointRight, m_forwardRect[1], m_forwardRect[0]);
        minDist = pow(abs(bestPoint.x - userMatPos.x), 2) + pow(abs(bestPoint.y - userMatPos.y), 2);
            
        //! Best guess left
        float offset = -0.5f;
        int it =0;
        while(not best and it <10)
        {
            it ++;
            wayPointLeft = project2D(cv::Point_<float>{targetPoint.x - Car::CarWidth*0.8f + offset, targetPoint.y});
            wayPointRight = project2D(cv::Point_<float>{targetPoint.x + Car::CarWidth*0.8f + offset, targetPoint.y});
            
            cv::Point point = maxDistInPath(wayPointLeft, wayPointRight, m_forwardRect[1], m_forwardRect[0]);
            int dist = pow(abs(point.x - userMatPos.x), 2) + pow(abs(point.y - userMatPos.y), 2);
            
            if (dist <= minDist)
            {
                minDist = dist;
                bestPoint = point;
                best = false;
                offset -= 0.5f;
            }
            else if ((float)dist >= (float)minDist * 1.2f)
            {
                best = true;
            }
            else
            {
                offset -= 0.5f;
            }
        }
        //Best guess right
        offset = 0.5f;
        best = false;
        it = 0;
        while(not best and it<10)
        {
            it ++;
            wayPointLeft = project2D(cv::Point_<float>{targetPoint.x - Car::CarWidth*0.8f + offset, targetPoint.y});
            wayPointRight = project2D(cv::Point_<float>{targetPoint.x + Car::CarWidth*0.8f + offset, targetPoint.y});
            
            //m_displayedImage.at<Vec3b>(cv::Point(wayPointRight.x * scaleX, wayPointRight.y * scaleY)) = orange;
            
            cv::Point point = maxDistInPath(wayPointLeft, wayPointRight, m_forwardRect[1], m_forwardRect[0]);
            int dist = pow(abs(point.x - userMatPos.x), 2) + pow(abs(point.y - userMatPos.y), 2);
         
            if (dist <= minDist)
            {
                minDist = dist;
                bestPoint = point;
                best = false;
                offset += 0.5f;
            }
            else if ((float)dist >= (float)minDist * 1.2f)
            {
                best = true;
            }
            else
            {
                offset += 0.5f;
            }
        }

        return bestPoint;
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
	
	
	//roadInQuad(m_forwardRect[3], m_forwardRect[2], m_forwardRect[1], m_forwardRect[0]);
		//create next target and give it the distance and direction (relative point) 
		cv::Point_<float> relativeTarget;
		relativeTarget.x = sin(UserDetectionTest.detector.getDirection()) * UserDetectionTest.detector.getDistance();
		relativeTarget.y = cos(UserDetectionTest.detector.getDirection()) * UserDetectionTest.detector.getDistance();
		cv::Point computedPoint = computeNextWayPoint(relativeTarget);
        //cv::Point test = computeNextWayPoint(cv::Point_<float>{4.0f, 12.18f});
        if (computedPoint.x >=0 and computedPoint.x <ROADMATCOL and computedPoint.y>=0 and computedPoint.y <=ROADMATROW)
            m_displayedImage.at<Vec3b>(computedPoint) = orange;
        computedPoint.x = computedPoint.x * (float)Camera::getFrameWidth() / (float)ROADMATCOL;
        computedPoint.y = computedPoint.y * (float)Camera::getFrameHeight() / (float)ROADMATROW;
        cv::Point_<float> absolutPoint = unproject2D(computedPoint);
        float x = absolutPoint.x;
        float y = absolutPoint.y;
        Target.x = atan2(x, y);
        Target.y = sqrt(pow(x,2)+pow(y,2));
}


cv::Point RoadDetection::maxDistInPath(cv::Point topLeft, cv::Point topRight, cv::Point bottomRight, cv::Point bottomLeft)
{
	//! Last achivable point before reaching grass
	cv::Point lPoint;
	
	//! Create the bounding box of the path !//
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

	lPoint.x = ROADMATCOL /2;
	lPoint.y = bMaxY;

	bool grass = false;
	int y=bMaxY;	
	while (y>=bMinY and not grass)
	{
		float yAdv = (float)(y-bMinY) / (float)(bMaxY - bMinY);
		int moyX = 0;
		int countX = 0;

		for (int x=bMinX ; x<=bMaxX ; x++)
		{
			float xAdv = (float)(x-bMinX) / (float)(bMaxX - bMinX);

			float left 	= scaleX * (topLeft.x * (1-yAdv) + bottomLeft.x * yAdv);
			float right = scaleX * (topRight.x * (1-yAdv) + bottomRight.x * yAdv);
			float top	= scaleY * (topLeft.y * (1-xAdv) + topRight.y * xAdv);
			float bottom= scaleY * (bottomLeft.y * (1-xAdv) + bottomRight.y * xAdv);

			//! Check that the pixel is inside the image		
			if (x>=0 and x<ROADMATCOL and y>=0 and y<ROADMATROW)
			{
				//! Check that the pixel is inside the quad
				if (x>left and x<right and y>top and y<bottom)
				{
					if (m_thresholdedImage.at<Vec3b>(Point(x,y)) == red
							or m_thresholdedImage.at<Vec3b>(Point(x,y)) == white)
					{
						grass = true;
					}
					else
					{
						//m_displayedImage.at<Vec3b>(Point(x,y)) = blue;
						moyX += x;
						countX ++;
					}				
				}

			}
			else
			{
				grass = true;
			}			
		}

		if (not grass)
		{
			lPoint.x = countX != 0 ? moyX / countX : lPoint.x;
			lPoint.y = y;
		}
		y--;
	}

	/*if (lPoint.x >=0 and lPoint.x <ROADMATCOL and lPoint.y>=0 and lPoint.y <=ROADMATROW)
	  m_displayedImage.at<Vec3b>(lPoint) = orange;*/

	return lPoint;
}


cv::Point RoadDetection::project2D(cv::Point_<float> relativePoint)
{
	//! Calculate y coord
	float tetay = atan2(relativePoint.y, Camera::PosZ);
	float dtetay = M_PI*90.f/180.f + Camera::pitch - tetay;
	float y = tan(dtetay) * m_dfy;

	//! Calculate x coord
	float distx = sqrt(Camera::PosZ + pow(relativePoint.y,2));
	//float dtetax = atan2(relativePoint.x, distx);	
	//float x = m_dfx * tan(dtetax);
	float x = m_dfx * relativePoint.x / distx;

	//! Set points coordinates
	x += Camera::getFrameWidth()/2;	
	y += Camera::getFrameHeight()/2;	
	return cv::Point(x, y);
}

cv::Point RoadDetection::unproject2D(cv::Point_<float> Point)
{
	cv::Point_<float> relativePoint;

	//! Set points coordinates
	Point.x -= Camera::getFrameWidth()/2;	
	Point.y -= Camera::getFrameHeight()/2;

	//! Calculate y from relativePoint
	float dtetay = atan2(Point.y, m_dfy);
	float tetay = M_PI*90.f/180.f + Camera::pitch - dtetay;	
	relativePoint.y = tan(tetay)*Camera::PosZ;

	//! Calculate x from relativePoint
	float distx = sqrt(Camera::PosZ + pow(relativePoint.y,2));
	relativePoint.x = Point.x * distx / m_dfx;
	return relativePoint;
}

Mat & RoadDetection::getImage() {
	return m_displayedImage;	
}

Mat & RoadDetection::getCameraImage() {
	return m_cameraImage;	
}
