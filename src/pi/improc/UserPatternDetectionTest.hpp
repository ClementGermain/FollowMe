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
		float x_mes_1000[2][1000];
		float y_mes_1000[2][1000];
		float r_mes_1000[2][1000];

		void Get_measures(void);
		void printMeasures();

	private:
		void run();

		bool endThread;
		std::thread * threadTest;
};

extern UserPatternDetectionTest UserDetectionTest;

#endif

