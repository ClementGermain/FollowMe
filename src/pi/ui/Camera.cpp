#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>
#include "RaspiCamCV.h"
#include "Camera.hpp"

using namespace std;

Camera::Camera(const char * winName, int width, int height, int framerate)
	: windowName(winName), raspiCam(NULL), threadRunning(false)
{
	// Init configuration
	configCam.width = width;
	configCam.height = height;
	configCam.bitrate = 0; // default
	configCam.framerate = framerate;
	configCam.monochrome = 0; // default
	
	// Initialize the camera
	raspiCam = raspiCamCvCreateCameraCapture2(0, &configCam);
}

Camera::~Camera() {
	// stop preview if necessary
	closePreview();

	// Release camera
	raspiCamCvReleaseCapture(&raspiCam);
}

void Camera::openPreview() {
	if(!threadRunning) {
		// Open a window
		cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);

		// Start thread
		threadRunning = true;
		threadPreview = new thread(Camera::loopPreview, this);
	}
}

void Camera::closePreview() {
	if(threadRunning) {
		threadRunning = false;
		threadPreview->join();
		delete threadPreview;
	}

	// Close the window
	cv::destroyWindow(windowName);
}

void Camera::getImage(cv::Mat & out) {
	out = raspiCamCvQueryFrame(raspiCam);
}

void Camera::loopPreview(Camera * that) {
	while(that->threadRunning) {
		this_thread::sleep_for(chrono::milliseconds(250));
		that->updatePreview();
	}
}

void Camera::updatePreview() {
	if(raspiCam != NULL) {
		// Get a frame
		imageCam = raspiCamCvQueryFrame(raspiCam);
		
		if(imageCam.data) {
			// Display frame
			cv::imshow(windowName, imageCam);
		}
	}
}
