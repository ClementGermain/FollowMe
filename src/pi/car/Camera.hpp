#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <thread>
#include <string>
#include <opencv2/opencv.hpp>
#include <SDL/SDL.h>
#include <chrono>
#include <mutex>
#include <thread>
#include "utils/Timer.hpp"

#ifndef __NO_RASPI__
	#include "RaspiCamCV.h"
#endif

#define DEFAULT_FRAME_WIDTH		320
#define DEFAULT_FRAME_HEIGHT	240
#define DEFAULT_FRAMERATE		30
#define OVERSCALE_FRAME			4

class Camera {
	public:
		// Initialize the camera and start the thread
		static void initAndStart(int width=DEFAULT_FRAME_WIDTH, int height=DEFAULT_FRAME_HEIGHT, int framerate=DEFAULT_FRAMERATE);
		// Release the camera and stop the thread
		static void destroyAndStop();
		// Indicate whether the camera is ready to fetch an image
		static bool imageCanBeFetchFromCamera();
		// Indicate whether an image has been already put in buffer
		static bool imageIsInBuffer();
		// update the current frame
		static void updateImage();
		// Give image from camera and write to 'out'
		// if camera is not ready, 'out' will not be updated
		static void getImage(cv::Mat & out);
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
		// Thread runnable method
		static void run();
	
		static std::mutex camLock;
		static bool imageInBuffer;
		static cv::Mat imageMat;
		static Timer timerCapture;
#ifndef __NO_RASPI__
		static RaspiCamCvCapture * raspiCam;
		static RASPIVID_CONFIG configCam;
#endif
		static bool isUp;
		static bool endThread;
		static std::thread * threadTest;
};


#endif

