#ifndef __OBSTACLE_HPP__
#define __OBSTACLE_HPP__

#include "car/Car.hpp"
#include <thread>
#include <ctime>

/**
 * Thread that reads ultrasonic sensor values to identify the position of physical obstacles.
 */
class ObstacleDetection {
	public:
		static void obstacleDetectionLeft();
		static void obstacleDetectionCenter();
		static void obstacleDetectionRight();
		static void obstacleDetectionGlobal();
		static void obstacleDetectionGlobalTimed();

		static bool isLeftDetected();
		static bool isCenterDetected();
		static bool isRightDetected();
		static bool isGlobalDetected();

		static void start();
		static void stop();

	private:
		static bool Left;
		static bool Center;
		static bool Right;
		static bool Global;
		static time_t Timer;
		static time_t Delta;

		static void run();

		static bool endThread;
		static std::thread * threadTest;
};


#endif
