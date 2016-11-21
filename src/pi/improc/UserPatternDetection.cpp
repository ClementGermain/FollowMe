#include <opencv2/opencv.hpp>
#include "UserPatternDetection.hpp"
#include <vector>
#include "utils/Log.hpp"
#include "car/Camera.hpp"

using namespace std;

const  float UserPattern::CircleRadius = 0.05f; // diameter = 10 cm
const int UserPattern::hLo = 35*180/255;
const int UserPattern::sLo = 110;
const int UserPattern::vLo = 50;
const int UserPattern::hHi = 58*180/255;
const int UserPattern::sHi = 255;
const int UserPattern::vHi = 245;


UserPatternDetection::UserPatternDetection() : resultImageCreated(false) {

}

void UserPatternDetection::findPattern(cv::Mat & bgr_image, bool drawResult) {
	cv::Mat median;
	// Apply median blur to remove noise
	cv::medianBlur(bgr_image, median, 3);

	// Convert input image to HSV
	cv::Mat hsv_image;
	cv::cvtColor(median, hsv_image, cv::COLOR_BGR2HSV);

	// Threshold the HSV image, keep only the red pixels
	cv::Mat yellow_hue_image;
	cv::inRange(hsv_image, cv::Scalar(UserPattern::hLo, UserPattern::sLo, UserPattern::vLo),
			cv::Scalar(UserPattern::hHi, UserPattern::sHi, UserPattern::vHi), yellow_hue_image);

	//morphological opening (remove small objects from the foreground)
	cv::erode(yellow_hue_image, yellow_hue_image, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) );
	cv::dilate(yellow_hue_image, yellow_hue_image, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) ); 

	//morphological closing (fill small holes in the foreground)
	cv::dilate(yellow_hue_image, yellow_hue_image, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) ); 
	cv::erode(yellow_hue_image, yellow_hue_image, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) );

	if(drawResult) {
		filterImage = yellow_hue_image.clone();
	}

	// Apply blur
	cv::GaussianBlur(yellow_hue_image, yellow_hue_image, cv::Size(9, 9), 2, 2);

	// Use the Hough transform to detect circles in the combined threshold image
	imageCircles.clear();
	cv::HoughCircles(yellow_hue_image, imageCircles, CV_HOUGH_GRADIENT, 1, yellow_hue_image.rows/8, 100, 20, 0, 0);

	// Loop over all detected circles and outline them on the original image
	if(drawResult) {
		resultImage = bgr_image.clone();
		for(size_t current_circle = 0; current_circle < imageCircles.size(); ++current_circle) {
			cv::Point center(std::round(imageCircles[current_circle][0]), std::round(imageCircles[current_circle][1]));
			int radius = std::round(imageCircles[current_circle][2]);

			cv::circle(resultImage, center, radius, cv::Scalar(0, 255, 0), current_circle*2+1);
		}
		resultImageCreated = true;
	}
}

void UserPatternDetection::imageCirclesToPosition() {
	LogI<<"Found "<<imageCircles.size()<<" circles"<<endl;
	// Half width/height of the image in pixels
	float halfWidth = Camera::getFrameWidth() * 0.5f;
	float halfHeight = Camera::getFrameHeight() * 0.5f;
	// focal length of the camera in pixels (yes, in pixels)
	float focalLength = Camera::getFrameWidth()*0.5f / tan(Camera::horizontalFOV/2);
	for(int i = 0; i < (int) imageCircles.size(); i++) {
		// Center x and y coordinate with center of image as origin
		float x = imageCircles[i][0] - halfWidth;
		float y = imageCircles[i][1] - halfHeight;
		// Radius
		float r = imageCircles[i][2];

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
		float pz = Camera::PosZ + distance * cos(yaw) * sin(pitch); // down->up axis

		// pitch/yaw -> horizontal direction: 0 radian is toward, positive is left, negative is right
		float direction = atan2(px, py);
		LogI << "   Circle "<<i<<": PAV.x="<<((angleXmax - angleXmin)*180/M_PI)<<"* PAV.y="<< ((angleYmax - angleYmin)*180/M_PI)<<"*"<<endl;
		LogI << "   Circle "<<i<<": dist="<<distance<<"m dir="<<(direction*180/M_PI)<<"* x="<<px<<"m y="<<py<<"m z="<<pz<<"m pitch="<<(pitch*180/M_PI)<<"* yaw="<<(yaw*180/M_PI)<<"*"<<endl;
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

