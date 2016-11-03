#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <string>
#include <opencv2/opencv2.hpp>
#include <pthread.h>

#define DEFAULT_FRAME_WIDTH		640
#define DEFAULT_FRAME_HEIGHT	480
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
	private:
		void updatePreview();

		const std::string windowName;
		RaspiCamCvCapture * raspiCam;
		cv::Mat imageCam;
		RASPIVID_CONFIG configCam;
		pthread_t thread;
		bool threadRunning;
};

#endif

