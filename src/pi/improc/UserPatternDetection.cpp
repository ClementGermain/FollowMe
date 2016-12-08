#include <opencv2/opencv.hpp>
#include "UserPatternDetection.hpp"
#include <vector>
#include "utils/Log.hpp"
#include <iostream>
#include "car/Camera.hpp"

using namespace std;

const float UserPattern::CircleRadius = 0.07f; // diameter = 14 cm
int UserPattern::hLo = 160;//35*180/255;
int UserPattern::sLo = 70;
int UserPattern::vLo = 50;
int UserPattern::hHi = 5;//58*180/255;
int UserPattern::sHi = 255;
int UserPattern::vHi = 255;


UserPatternDetection::UserPatternDetection() : resultImageCreated(false) {

}

void UserPatternDetection::findPattern(cv::Mat & bgr_image, bool drawResult) {
	// Convert input image to HSV
	cv::Mat hsv_image;
	cv::cvtColor(bgr_image, hsv_image, cv::COLOR_BGR2HSV);

	// Threshold the HSV image, keep only the red pixels
	cv::Mat hue_image;
	// Case of low hue > high hue (use cyclic propertie of hue)
	if(UserPattern::hLo > UserPattern::hHi) {
		// upper hue
		cv::inRange(hsv_image, cv::Scalar(UserPattern::hLo, UserPattern::sLo, UserPattern::vLo),
				cv::Scalar(180, UserPattern::sHi, UserPattern::vHi), hue_image);
		// lower hue
		cv::Mat lowerHue;
		cv::inRange(hsv_image, cv::Scalar(0, UserPattern::sLo, UserPattern::vLo),
				cv::Scalar(UserPattern::hHi, UserPattern::sHi, UserPattern::vHi), lowerHue);
		// Combine lower and upper hue
		cv::addWeighted(hue_image, 1.0, lowerHue, 1.0, 0.0, hue_image);
	}
	// Case : low hue < high hue
	else {
		cv::inRange(hsv_image, cv::Scalar(UserPattern::hLo, UserPattern::sLo, UserPattern::vLo),
				cv::Scalar(UserPattern::hHi, UserPattern::sHi, UserPattern::vHi), hue_image);
	}

	//morphological opening (remove small objects from the foreground)
	cv::erode(hue_image, hsv_image, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) );
	cv::dilate(hsv_image, hue_image, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) ); 

	//morphological closing (fill small holes in the foreground)
	cv::dilate(hue_image, hsv_image, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) ); 
	cv::erode(hsv_image, hue_image, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) );

	// Apply blur
	cv::GaussianBlur(hue_image, hue_image, cv::Size(9, 9), 2, 2);

	// Use the Hough transform to detect circles in the combined threshold image
	imageCircles.clear();
	cv::HoughCircles(hue_image, imageCircles, CV_HOUGH_GRADIENT, 1, hue_image.rows/8, 100, 20, 0, 0);

	// Loop over all detected circles and outline them on the original image
	if(drawResult)  {
		hue_image.copyTo(filterImage);
		bgr_image.copyTo(resultImage);
		if(imageCircles.size() > 0) {
			cv::Point center(std::round(imageCircles[0][0]), std::round(imageCircles[0][1]));
			int radius = std::round(imageCircles[0][2]);

			cv::circle(resultImage, center, radius, cv::Scalar(0, 255, 0), 2);

			// Half width/height of the image in pixels
			float halfWidth = Camera::getFrameWidth() * 0.5f;
			float halfHeight = Camera::getFrameHeight() * 0.5f;
			float x = imageCircles[0][0] - halfWidth;
			float y = imageCircles[0][1] - halfHeight;
			// Radius
			float r = imageCircles[0][2];

			cv::Mat corrected = kalmanFilter.Kalman_Filter_User_Detection(x, y, r);
			LogD << "KF x:"<< (x - corrected.at<float>(0)) << std::endl;
			LogD << "KF y:"<< (y - corrected.at<float>(1)) << std::endl;
			LogD << "KF r:"<< (r - corrected.at<float>(2)) << std::endl;
			imageCircles[0][0] = corrected.at<float>(0);
			imageCircles[0][1] = corrected.at<float>(1);
			imageCircles[0][2] = corrected.at<float>(2);

			center = cv::Point(std::round(imageCircles[0][0]+halfWidth), std::round(imageCircles[0][1]+halfHeight));
			radius = std::round(imageCircles[0][2]);

			cv::circle(resultImage, center, radius, cv::Scalar(255, 128, 0), 2);
		}
		resultImageCreated = true;
	}
}

void UserPatternDetection::imageCirclesToPosition() {
	isUserDetected = false;

	// Half width/height of the image in pixels
	float halfWidth = Camera::getFrameWidth() * 0.5f;
	float halfHeight = Camera::getFrameHeight() * 0.5f;
	// focal length of the camera in pixels (yes, in pixels)
	float focalLength = halfWidth / tan(Camera::horizontalFOV/2);

	if(imageCircles.size() > 0) {
		// Center x and y coordinate with center of image as origin
		float x = imageCircles[0][0];
		float y = imageCircles[0][1];
		// Radius
		float r = imageCircles[0][2];

		// position and radius -> perceived visual angle (PVA) of the circle
		// left and right bounds of the horizontal PVA (transform circle's horizontal bounds image position to an angle from camera direction)
		float angleXmin = atan2(x - r, focalLength), angleXmax = atan2(x + r, focalLength);
		// up and down bounds of the vertical PVA (transform circle's vertical bounds image position to an angle from camera direction)
		float angleYmin = atan2(y - r, focalLength), angleYmax = atan2(y + r, focalLength);
		// PVA is the mean of x PVA and y PVA
		float pva = ((angleXmax - angleXmin) + (angleYmax - angleYmin)) * 0.5f;
		// PVA -> distance
		float distance = UserPattern::CircleRadius / tan(pva / 2);
		// y -> pitch
		float pitch = Camera::pitch + atan2(-y, focalLength);
		// x -> yaw
		float yaw = atan2(-x, focalLength);

		// pitch+yaw+distance -> cartesian coordinate relative to car origin
		float px = Camera::PosX + distance * -sin(yaw); // back->front axis
		float py = Camera::PosY + distance * cos(yaw) * cos(pitch); // right->left axis
		//float pz = Camera::PosZ + distance * cos(yaw) * sin(pitch); // down->up axis

		// pitch/yaw -> horizontal direction: 0 radian is toward, positive is left, negative is right
		float direction = atan2(px, py);

		// export result
		isUserDetected = true;
		detectedDirection = direction;
		detectedDistance = distance;
	}
}

cv::Mat & UserPatternDetection::getResultImage() {
	return resultImage;
}

cv::Mat & UserPatternDetection::getFilterImage() {
	return filterImage;
}

bool UserPatternDetection::hasImage() {
	return resultImageCreated;
}

bool UserPatternDetection::isDetected() {
	return isUserDetected;
}

float UserPatternDetection::getDirection() {
	return detectedDirection;
}

float UserPatternDetection::getDistance() {
	return detectedDistance;
}
