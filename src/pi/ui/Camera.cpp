#include <opencv2/opencv.hpp>
#include <iostream>
#include <pthread.h>
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
	// Open a window
	cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
	cv::waitKey(100);

	// Start thread
	threadRunning = true;
	pthread_create(&thread, NULL, Camera::loopPreview, this);
}

void Camera::closePreview() {
	if(threadRunning) {
		threadRunning = false;
		pthread_join(thread, NULL);
	}

	// Close the window
	cv::destroyWindow(windowName);
}

void Camera::getImage(cv::Mat & out) {
	out = raspiCamCvQueryFrame(raspiCam);
}

void * Camera::loopPreview(void * data) {
	Camera * that = (Camera*) data;

	while(that->threadRunning) {
		cv::waitKey(500);
		that->updatePreview();
	}

	return NULL;
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
