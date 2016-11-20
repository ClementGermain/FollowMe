#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <thread>
#include <string>
#include <opencv2/opencv.hpp>
#include <SDL/SDL.h>
#include <chrono>
#include <mutex>
#include "utils/Timer.hpp"

#ifndef __NO_RASPI__
	#include "RaspiCamCV.h"
#endif

#define DEFAULT_FRAME_WIDTH		320
#define DEFAULT_FRAME_HEIGHT	240
#define DEFAULT_FRAMERATE		30

class Camera {
	public:
		// Initialize the camera
		static void init(int width=DEFAULT_FRAME_WIDTH, int height=DEFAULT_FRAME_HEIGHT, int framerate=DEFAULT_FRAMERATE);
		// Release the camera
		static void destroy();
		// update the current frame
		static void updateImage();
		// Give image from camera
		static void getImage(cv::Mat & out);
		static SDL_Surface * getBitmap();
		// Dynamic properties
		static int getFrameWidth(); // in Pixels
		static int getFrameHeight(); // in Pixels
		static int getFrameRate(); // in Hertz
		static int getFrameDuration(); // in Milliseconds

		/// Static camera's position and properties
		// Camera position in the car (relative to center of car) (meter)
		static const float PosX,PosY,PosZ;
		// pitch angle from horizontal (radians)
		static const float pitch;
		// horizontal field of view of the camera sensor (radians)
		static const float horizontalFOV;
		// vertical field of view of the camera sensor (radians)
		static const float verticalFOV;
		
	private:
	
		static std::mutex camLock;
		static IplImage * imageCam;
		static Timer timerCapture;
#ifndef __NO_RASPI__
		static RaspiCamCvCapture * raspiCam;
		static RASPIVID_CONFIG configCam;
#endif
};


#endif

