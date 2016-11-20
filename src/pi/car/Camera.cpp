#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <cmath>
#include "Camera.hpp"

#ifndef __NO_RASPI__
#include "RaspiCamCV.h"
#endif

using namespace std;


const float Camera::height			= 1.0f;
const float Camera::pitch			= 20 * M_PI/180;
const float Camera::horizontalFOV	= 53 * M_PI/180;
const float Camera::verticalFOV		= 41 * M_PI/180;
std::mutex Camera::camLock;
IplImage * Camera::imageCam;
std::chrono::time_point Camera::lastCaptureDate;
#ifndef __NO_RASPI__
RaspiCamCvCapture * Camera::raspiCam = NULL;
RASPIVID_CONFIG Camera::configCam;
#endif

void Camera::init(int width, int height, int framerate)
{
#ifndef __NO_RASPI__
	if(raspiCam == NULL) {
		// Init configuration
		configCam.width = width;
		configCam.height = height;
		configCam.bitrate = 0; // default
		configCam.framerate = framerate;
		configCam.monochrome = 0; // default

		// Initialize the camera
		raspiCam = raspiCamCvCreateCameraCapture2(0, &configCam);
	}
#endif
}

void Camera::destroy() {
#ifndef __NO_RASPI__
	if(raspiCam != NULL) {
		// Release camera
		raspiCamCvReleaseCapture(&raspiCam);
	}
#endif
	if(imageCam != NULL) {
		cvReleaseImage(&imageCam);
	}
}

int Camera::getFrameWidth() {
	return configCam.width;
}

int Camera::getFrameHeight() {
	return configCam.height;
}

int Camera::getFrameRate() {
	return configCam.framerate;
}

int Camera::getFrameDuration() {
	return 1000 / configCam.framerate + 1;
}

void Camera::updateImage() {
	// Get elapsed time since last capture
	chrono::duration<chrono::milliseconds> timeSinceLastCapture = chrono::system_clock::now() - lastCaptureDate;
	
	// Update the capture if too old
	if(timeSinceLastCapture > getFrameDuration() || imageCam == NULL) {
		// lock the mutex 
		// FIXME->it can block the current thread and all the others waiting threads for 30ms
		camLock.lock();

		// release the previous image
		if(imageCam != NULL)
			cvRealeaseImage(&imageCam);
		
		// If the pending frame is too old, skip it
		if(timeSinceLastCapture > 2*getFrameDuration())
			raspiCamCvQueryFrame(raspiCam);
		
		// get the next frame (can wait for 1/framerate second max...)
		imageCam = cvCloneImage(raspiCamCvQueryFrame(raspiCam));
		// update the capture date
		lastCaptureDate = chrono::system_clock::now();
		
		// unlock the mutex
		camLock.unlock();
	}
}

void Camera::getImage(cv::Mat & out) {
	updateImage();

#ifndef __NO_RASPI__
	// lock the mutex 
	camLock.lock();

	// Create a new matrix from the current capture
	out = imageCam;

	// unlock the mutex
	camLock.unlock();
#endif
}

SDL_Surface * Camera::getBitmap() {
	updateImage();

#ifndef __NO_RASPI__
	// lock the mutex 
	camLock.lock();

	// Create a new SDL_SUrface from the current capture
	s = SDL_CreateRGBSurfaceFrom((void*)img->imageData,
			img->width,
			img->height,
			img->depth * img->nChannels,
			img->widthStep,
			0xff0000, 0x00ff00, 0x0000ff, 0
	);

	// unlock the mutex
	camLock.unlock();
#else
	s = SDL_CreateRGBSurface(SDL_SWSURFACE, DEFAULT_FRAME_WIDTH, DEFAULT_FRAME_HEIGHT, 32, 0,0,0,0);
#endif

	 return s;
}

