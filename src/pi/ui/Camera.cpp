#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <unistd.h>
#include "Camera.hpp"

#ifndef __NO_RASPI__
#include "RaspiCamCV.h"
#endif

using namespace std;

Camera::Camera(const char * winName, int width, int height, int framerate) :
	windowName(winName),
	threadRunning(false)
{
#ifndef __NO_RASPI__
	// Init configuration
	configCam.width = width;
	configCam.height = height;
	configCam.bitrate = 0; // default
	configCam.framerate = framerate;
	configCam.monochrome = 0; // default
	
	// Initialize the camera
	raspiCam = raspiCamCvCreateCameraCapture2(0, &configCam);
#endif
}

Camera::~Camera() {
	// stop preview if necessary
	closePreview();

#ifndef __NO_RASPI__
	// Release camera
	raspiCamCvReleaseCapture(&raspiCam);
#endif
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
	cv::waitKey(100);
}

void Camera::getImage(cv::Mat & out) {
#ifndef __NO_RASPI__
	out = raspiCamCvQueryFrame(raspiCam);
#endif
}

SDL_Surface * Camera::getBitmap(double scale) {
#ifndef __NO_RASPI__
	IplImage * img = raspiCamCvQueryFrame(raspiCam);
	cv::resize(img, img, cv::Size(0,0), scale, scale);
	return SDL_CreateRGBSurfaceFrom((void*)img->imageData,
			img->width,
			img->height,
			img->depth * img->nChannels,
			img->widthStep,
			0xff0000, 0x00ff00, 0x0000ff, 0
	);
#else
	return SDL_CreateRGBSurface(SDL_SWSURFACE, DEFAULT_FRAME_WIDTH, DEFAULT_FRAME_HEIGHT, 32, 0,0,0,0);
#endif
}

void Camera::loopPreview(Camera * that) {
	while(that->threadRunning) {
		cv::waitKey(100);
		that->updatePreview();
	}
}

void Camera::updatePreview() {
#ifndef __NO_RASPI__
	if(raspiCam != NULL) {
		// Get a frame
		imageCam = raspiCamCvQueryFrame(raspiCam);
		
		if(imageCam.data) {
			// Display frame
			cv::imshow(windowName, imageCam);
		}
	}
#endif
}
