#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <thread>
#include <string>
#include <opencv2/opencv.hpp>
#include <SDL/SDL.h>

#ifndef __NO_RASPI__
	#include "RaspiCamCV.h"
#endif

#define DEFAULT_FRAME_WIDTH		320
#define DEFAULT_FRAME_HEIGHT	240
#define DEFAULT_FRAMERATE		30

class Camera {
	public:
		Camera(int width=DEFAULT_FRAME_WIDTH, int height=DEFAULT_FRAME_HEIGHT, int framerate=DEFAULT_FRAMERATE);
		~Camera();
		// Give image from camera
		void getImage(cv::Mat & out);
		SDL_Surface * getBitmap(double scale);

	private:

		cv::Mat imageCam;
#ifndef __NO_RASPI__
		RaspiCamCvCapture * raspiCam;
		RASPIVID_CONFIG configCam;
#endif
};

extern Camera RaspiCam;

#endif

