#include <opencv2/opencv2.hpp>
#include <pthread.h>
#include "RaspiCamCV.h"

void * loopPreview(void * data);

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
	raspiCamCvReleaseCapture(raspiCam);
	raspiCam = NULL;
}

void Camera::openPreview() {
	// Open a window
	cv::namedWindow(windowNameCam, WINDOW_AUTOSIZE);

	// Start thread
	threadRunning = true;
	pthread_create(&thread, NULL, loopPreview, this);
}

void Camera::closePreview() {
	if(threadRunning) {
		threadRunning = false;
		pthread_join(thread, NULL);
	}

	// Close the window
	cv::destroyWindow(windowNameCam);
}

void Camera::getImage(cv::Mat & out) {
	out = raspiCamCvQueryFrame(raspiCam);
}

void * loopPreview(void * data) {
	Camera * that = (Camera*) data;

	while(that->threadRunning) {
		that->updatePreview();
		usleep(100*1000);
	}

	return NULL;
}

void Camera::updatePreview() {
	if(raspiCam != NULL) {
		// Get a frame
		imageCam = raspiCamCvQueryFrame(raspiCam);
		
		if(imageCam.data) {
			// Display frame
			cv::imshow(windowNameCam, imageCam);
		}
	}
}
