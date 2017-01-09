#include <opencv2/opencv.hpp>
#include "UserPatternDetection.hpp"
#include <vector>
#include "utils/Log.hpp"
#include <iostream>
#include "car/Camera.hpp"

#define CLAMP(X) std::min(std::max((double)(X), 0.0), 1.0)

using namespace std;

const float UserPattern::CircleRadius = 0.065f; // diameter = 13 cm
int UserPattern::hLo = 160;//35*180/255;
int UserPattern::sLo = 70;
int UserPattern::vLo = 50;
int UserPattern::hHi = 5;//58*180/255;
int UserPattern::sHi = 255;
int UserPattern::vHi = 255;
const int UserPattern::maxFrameUserUndetected = 5;
const int UserPattern::frameDurationMillis = 100;


UserPatternDetection::UserPatternDetection() : resultImageCreated(false), frameCountSinceUserUndetected(UserPattern::maxFrameUserUndetected), isUserDetected(false), detectedDirection(0), detectedDistance(1.8), useRevolutionnaryMode(false) {
	filteredCircle[0] = 0;
	filteredCircle[1] = 0;
	filteredCircle[2] = 0;
	x_mes = 0.0;
	y_mes = 0.0;
	r_mes = 0.0;
}

float UserPatternDetection::Get_x_mes() { return x_mes; }
float UserPatternDetection::Get_y_mes() { return y_mes; }
float UserPatternDetection::Get_r_mes() { return r_mes; }
float UserPatternDetection::Get_x_kalman() { return filteredCircle[0]; }
float UserPatternDetection::Get_y_kalman() { return filteredCircle[1]; }
float UserPatternDetection::Get_r_kalman() { return filteredCircle[2]; }

void UserPatternDetection::findPattern(cv::Mat & bgr_image, bool drawResult) {
	// Convert input image to HSV
	cv::Mat hsv_image;
	cv::cvtColor(bgr_image, hsv_image, cv::COLOR_BGR2HSV);
	cv::Mat gray;
	if(useRevolutionnaryMode) {
		cv::cvtColor(bgr_image, gray, CV_BGR2GRAY);
		int hWidth = (UserPattern::hLo > UserPattern::hHi ? 180 : 0) + (UserPattern::hHi - UserPattern::hLo);
		int hMean = (UserPattern::hLo + hWidth / 2) % 180;
		double h0 = (90-hWidth/2)/90.0-0.4, h1 = (90-hWidth/2)/90.0;
		double s0 = 0.2, s1 = 0.4;
		double v0 = 0.13, v1 = 0.27;
		for(int i = 0; i < hsv_image.rows; i++)
		{
			for(int j = 0; j < hsv_image.cols; j++)
			{
				cv::Vec3b pxl = hsv_image.at<cv::Vec3b>(i,j);
				int hWrap = ((pxl[0] - hMean) + 90 + 180) % 180;
				double h = (double) (hWrap > 90 ? 180 - hWrap : hWrap) / 90;
				h = CLAMP((h-h0) / (h1-h0));
				double s = pxl[1] / 255.0;
				s = CLAMP((s-s0) / (s1-s0)); //s > UserPattern::sLo/255.0 ? 1.0 : s / (UserPattern::sLo/255.0);
				double v = pxl[2] / 255.0;
				v = CLAMP((v-v0) / (v1-v0)); //v > UserPattern::vLo/255.0 ? 1.0 : v / (UserPattern::vLo/255.0);
				gray.at<uchar>(i,j) = 255 * h*s*v;
			}
		}
		// Apply blur
		cv::GaussianBlur(gray, gray, cv::Size(3, 3), 2, 2);

	} else {
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
		std::vector<cv::Vec3f> imageCircles;
		cv::HoughCircles(hue_image, imageCircles, CV_HOUGH_GRADIENT, 1, hue_image.rows/8, 100, 20, 0, 0);
		gray = hue_image;
	}

	// Use the Hough transform to detect circles in the combined threshold image
	std::vector<cv::Vec3f> imageCircles;
	cv::HoughCircles(gray, imageCircles, CV_HOUGH_GRADIENT, 1, gray.rows/8, 100, 20, 0, 0);

	// User is detected
	if(imageCircles.size() > 0) {
		frameCountSinceUserUndetected = 0;
		// apply kalman filter
		float x = imageCircles[0][0] - Camera::getFrameWidth() * 0.5f;
		float y = imageCircles[0][1] - Camera::getFrameHeight() * 0.5f;
		float r = imageCircles[0][2];
		x_mes = x;
		y_mes = y;
		r_mes = r;

		// The user has just become detected
		if(!isUserDetected)
			kalmanFilter.resetState(x, y, r);

		// update kalman noises
		kalmanFilter.updateMeasurementNoise(detectedDistance);
		kalmanFilter.updateProcessNoise(detectedDistance);

		// Correct measurement with kalman filter
		cv::Mat corrected = kalmanFilter.correctMeasurement(x, y, r);
		filteredCircle[0] = corrected.at<float>(0);
		filteredCircle[1] = corrected.at<float>(1);
		filteredCircle[2] = corrected.at<float>(2);
		isUserDetected = true;
	}
	// User is no longer visible
	else {
		frameCountSinceUserUndetected++;

		// User has remained undetected for far too long
		if(frameCountSinceUserUndetected >= UserPattern::maxFrameUserUndetected) {
			isUserDetected = false;
		}
		// User has remained undetected for a short time
		else {
			// update kalman noises
			kalmanFilter.updateMeasurementNoise(detectedDistance);
			kalmanFilter.updateProcessNoise(detectedDistance);
			
			// predict next state with kalman filter
			cv::Mat corrected = kalmanFilter.predictNoMeasurement();
			filteredCircle[0] = corrected.at<float>(0);
			filteredCircle[1] = corrected.at<float>(1);
			filteredCircle[2] = corrected.at<float>(2);
			isUserDetected = true;
		}
	}

	// Loop over all detected circles and outline them on the original image
	if(drawResult)  {
		// copy filtered image
		gray.copyTo(filterImage);
		// copy camera image
		bgr_image.copyTo(resultImage);

		// draw kalman filtered circle on camera image
		if(isUserDetected) {
			cv::Point center(std::round(filteredCircle[0] + Camera::getFrameWidth() * 0.5f), std::round(filteredCircle[1] + Camera::getFrameHeight() * 0.5f));
			int radius = std::round(std::max(0.0f, filteredCircle[2]));

			cv::circle(resultImage, center, radius, cv::Scalar(255, 128, 0), 2);
		}

		// draw hough circle on camera image
		if(frameCountSinceUserUndetected == 0) {
			cv::Point center(std::round(imageCircles[0][0]), std::round(imageCircles[0][1]));
			int radius = std::round(imageCircles[0][2]);

			cv::circle(resultImage, center, radius, cv::Scalar(0, 255, 0), 1);
		}

		resultImageCreated = true;
	}
}

void UserPatternDetection::imageCirclesToPosition() {
	// Half width of the image in pixels
	float halfWidth = Camera::getFrameWidth() * 0.5f;
	// focal length of the camera in pixels (yes, in pixels)
	float focalLength = halfWidth / tan(Camera::horizontalFOV/2);

	if(isUserDetected) {
		// Center x and y coordinate with center of image as origin
		float x = filteredCircle[0];
		float y = filteredCircle[1];
		// Radius
		float r = filteredCircle[2];

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

bool UserPatternDetection::isVisible() {
	return frameCountSinceUserUndetected == 0;
}

float UserPatternDetection::getDirection() {
	return detectedDirection;
}

float UserPatternDetection::getDistance() {
	return detectedDistance;
}

void UserPatternDetection::toggleMode() {
	useRevolutionnaryMode = !useRevolutionnaryMode;
}
