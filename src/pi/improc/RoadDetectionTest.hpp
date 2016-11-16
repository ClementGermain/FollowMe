#ifndef __USER_PATTERN_DETECTION_TEST_HPP__
#define __USER_PATTERN_DETECTION_TEST_HPP__

#include <thread>
#include "RoadDetection.hpp"

class RoadDetectionTest {
	public:
		RoadDetectionTest();
		~RoadDetectionTest();

		void start();
		void stop();
		RoadDetection detector;

	private:
		void run();

		bool endThread;
		std::thread * threadTest;
};

extern RoadDetectionTest RoadDetectionTest;

#endif

