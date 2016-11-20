#include <opencv2/opencv.hpp>
#include "UserPatternDetection.hpp"

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
	cv::inRange(hsv_image, cv::Scalar(35*360/255/2, 110, 50), cv::Scalar(58*360/255/2, 255, 245), yellow_hue_image);

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
	std::vector<cv::Vec3f> circles;
	cv::HoughCircles(yellow_hue_image, circles, CV_HOUGH_GRADIENT, 1, yellow_hue_image.rows/8, 100, 20, 0, 0);

	// Loop over all detected circles and outline them on the original image
	if(drawResult) {
		resultImage = bgr_image.clone();
		for(size_t current_circle = 0; current_circle < circles.size(); ++current_circle) {
			cv::Point center(std::round(circles[current_circle][0]), std::round(circles[current_circle][1]));
			int radius = std::round(circles[current_circle][2]);

			cv::circle(resultImage, center, radius, cv::Scalar(0, 255, 0), 5);
		}
		resultImageCreated = true;
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

