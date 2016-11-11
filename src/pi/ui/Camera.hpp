#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <string>
#include <opencv2/opencv.hpp>
#include "RaspiCamCV.h"
#include <thread>

#define DEFAULT_FRAME_WIDTH		320
#define DEFAULT_FRAME_HEIGHT	240
#define DEFAULT_FRAMERATE		30

class Camera {
	public:
		Camera(const char * winName, int width=DEFAULT_FRAME_WIDTH, int height=DEFAULT_FRAME_HEIGHT, int framerate=DEFAULT_FRAMERATE);
		~Camera();
		// Open camera preview window
		void openPreview();
		// Close camera preview window
		void closePreview();
		// Give image from camera
		void getImage(cv::Mat & out);
		SDL_Surface * getBitmp(double scale);

	private:
		void updatePreview();
		static void loopPreview(Camera * that);

		const std::string windowName;
		RaspiCamCvCapture * raspiCam;
		cv::Mat imageCam;
		RASPIVID_CONFIG configCam;
		std::thread * threadPreview;
		bool threadRunning;
};

#endif

