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
const float Camera::pitch			= -31.75f * M_PI/180.f;
const float Camera::horizontalFOV	= 62.2f * M_PI/180.f;
const float Camera::verticalFOV		= 48.8f * M_PI/180.f;
std::mutex Camera::camLock;
IplImage * Camera::imageCam;
cv::Mat Camera::imageMat;
Timer Camera::timerCapture;
#ifndef __NO_RASPI__
RaspiCamCvCapture * Camera::raspiCam = NULL;
RASPIVID_CONFIG Camera::configCam;
#endif
thread * Camera::threadTest = NULL;
bool Camera::endThread = true;

void Camera::initAndStart(int width, int height, int framerate)
{
#ifndef __NO_RASPI__
	if(raspiCam == NULL) {
		// Init configuration
		configCam.width = width * OVERSCALE_FRAME;
		configCam.height = height * OVERSCALE_FRAME;
		configCam.bitrate = 0; // default
		configCam.framerate = framerate;
		configCam.monochrome = 0; // default

		// Initialize the camera
		raspiCam = raspiCamCvCreateCameraCapture2(0, &configCam);
	}
#endif
	if(threadTest == NULL) {
		endThread = false;
		threadTest = new thread(Camera::run);
	}
}

void Camera::run() {
	while(!Camera::endThread) {
		// wait the next frame and update the buffer
		updateImage();
		this_thread::sleep_for(chrono::milliseconds(Camera::getFrameDuration()-1));
	}
}

void Camera::destroyAndStop() {
	camLock.lock();
#ifndef __NO_RASPI__
	if(raspiCam != NULL) {
		// Release camera
		raspiCamCvReleaseCapture(&raspiCam);
	}
#endif
	if(imageCam != NULL) {
		cvReleaseImage(&imageCam);
	}
	camLock.unlock();
	if(threadTest != NULL) {
		endThread = true;
		threadTest->join();
		delete threadTest;
		threadTest = NULL;
	}
}

bool Camera::imageCanBeFetchFromCamera() {
#ifndef __NO_RASPI__
	return raspiCam != NULL && !endThread && threadTest != NULL;
#else
	return !endThread && threadTest != NULL;
#endif
}
bool Camera::imageIsInBuffer() {
#ifndef __NO_RASPI__
	return raspiCam != NULL && imageCam != NULL;
#else
	return true;
#endif
}

int Camera::getFrameWidth() {
#ifndef __NO_RASPI__
	return configCam.width / OVERSCALE_FRAME;
#else
	return DEFAULT_FRAME_WIDTH;
#endif
}

int Camera::getFrameHeight() {
#ifndef __NO_RASPI__
	return configCam.height / OVERSCALE_FRAME;
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

	if(Camera::imageCanBeFetchFromCamera()) {
#ifndef __NO_RASPI__
		// release the previous image
		if(imageCam != NULL)
			cvReleaseImage(&imageCam);

		// get the next frame (can wait for 1/framerate second max...)
		imageCam = cvCloneImage(raspiCamCvQueryFrame(raspiCam));
		cv::Mat mat = imageCam;
		cv::resize(mat, imageMat, cv::Size(getFrameWidth(), getFrameHeight()));
#endif
	}

	// unlock the mutex
	camLock.unlock();
}

void Camera::getImage(cv::Mat & out) {
#ifndef __NO_RASPI__
	// lock the mutex 
	camLock.lock();

	if(Camera::imageIsInBuffer()) {
		// Copy the matrix to the ouput
		imageMat.copyTo(out);
	}

	// unlock the mutex
	camLock.unlock();
#endif
}

