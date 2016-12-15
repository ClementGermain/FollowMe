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


const float Camera::PosX			= 0.0f;
const float Camera::PosY			= 0.0f;
const float Camera::PosZ			= 1.06f;//1m and 6cm above ground
const float Camera::pitch			= -30.f * M_PI/180.f;
const float Camera::horizontalFOV	= 62.2f * M_PI/180.f;
const float Camera::verticalFOV		= 48.8f * M_PI/180.f;
std::mutex Camera::camLock;
IplImage * Camera::imageCam;
Timer Camera::timerCapture;
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
#ifndef __NO_RASPI__
	return configCam.width;
#else
	return DEFAULT_FRAME_WIDTH;
#endif
}

int Camera::getFrameHeight() {
#ifndef __NO_RASPI__
	return configCam.height;
#else
	return DEFAULT_FRAME_HEIGHT;
#endif
}

int Camera::getFrameRate() {
#ifndef __NO_RASPI__
	return configCam.framerate;
#else
	return DEFAULT_FRAMERATE;
#endif
}

int Camera::getFrameDuration() {
	return 1000 / getFrameRate() + 1;
}

void Camera::updateImage() {
	// lock the mutex 
	// FIXME->it can block the current thread and all the others mutex-locked threads for 30ms
	camLock.lock();

	// Get elapsed time since last capture in milliseconds
	int timeSinceLastCapture = timerCapture.elapsed() * 1000;

	// Update the capture if too old
	if(timeSinceLastCapture > getFrameDuration() || imageCam == NULL) {
#ifndef __NO_RASPI__
		// release the previous image
		if(imageCam != NULL)
			cvReleaseImage(&imageCam);
		
		// If the pending frame is too old, skip it
		if(timeSinceLastCapture > 2*getFrameDuration())
			raspiCamCvQueryFrame(raspiCam);
		
		// get the next frame (can wait for 1/framerate second max...)
		imageCam = cvCloneImage(raspiCamCvQueryFrame(raspiCam));
#endif

		// update the capture date
		timerCapture.reset();
	}

	// unlock the mutex
	camLock.unlock();
}

void Camera::getImage(cv::Mat & out) {
	updateImage();

#ifndef __NO_RASPI__
	// lock the mutex 
	camLock.lock();

	// Create a new matrix from the current capture
	out = cv::Mat(imageCam, true);

	// unlock the mutex
	camLock.unlock();
#endif
}

SDL_Surface * Camera::getBitmap() {
	updateImage();

	SDL_Surface * s;

#ifndef __NO_RASPI__
	// lock the mutex 
	camLock.lock();

	// Create a new SDL_SUrface from the current capture
	s = SDL_CreateRGBSurfaceFrom((void*)imageCam->imageData,
			imageCam->width,
			imageCam->height,
			imageCam->depth * imageCam->nChannels,
			imageCam->widthStep,
			0xff0000, 0x00ff00, 0x0000ff, 0
	);

	// unlock the mutex
	camLock.unlock();
#else
	s = SDL_CreateRGBSurface(SDL_SWSURFACE, DEFAULT_FRAME_WIDTH, DEFAULT_FRAME_HEIGHT, 32, 0,0,0,0);
#endif

	 return s;
}

