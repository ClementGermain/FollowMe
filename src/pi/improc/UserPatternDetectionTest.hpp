#ifndef __USER_PATTERN_DETECTION_TEST_HPP__
#define __USER_PATTERN_DETECTION_TEST_HPP__

#include <thread>
#include "UserPatternDetection.hpp"

class UserPatternDetectionTest {
	public:
		UserPatternDetectionTest();
		~UserPatternDetectionTest();

		void start();
		void stop();
		UserPatternDetection detector;

	private:
		void run();

		bool endThread;
		std::thread * threadTest;
};

extern UserPatternDetectionTest UserDetectionTest;

#endif

