#ifndef __OBSTACLE_HPP__
#define __OBSTACLE_HPP__

#include "../../stm32/KeilProject/Sources/Barstow/Model.h"
#include "car/Car.hpp"
#include <thread>

class ObstacleDetection {
	public:
		static void ObstacleDetectionLeft();
		static void ObstacleDetectionCenter();
		static void ObstacleDetectionRigth();
		static void ObstacleDetectionGlobal();

		static bool IsLeftDetected();
		static bool IsCenterDetected();
		static bool IsRightDetected();
		static bool IsGlobalDetected();

		ObstacleDetection();
		~ObstacleDetection();

		void start();
		void stop();

	private:
		static bool Left;
		static bool Center;
		static bool Right;
		static bool Global;

		void run();

		bool endThread;
		std::thread * threadTest;
};
#endif
