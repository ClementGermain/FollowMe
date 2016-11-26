#ifndef __OBSTACLE_HPP__
#define __OBSTACLE_HPP__

#include "car/Car.hpp"
#include <thread>

class ObstacleDetection {
	public:
		static void obstacleDetectionLeft();
		static void obstacleDetectionCenter();
		static void obstacleDetectionRight();
		static void obstacleDetectionGlobal();

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

		static void run();

		static bool endThread;
		static std::thread * threadTest;
};


#endif
