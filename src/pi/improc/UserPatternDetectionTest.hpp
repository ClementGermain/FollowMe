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
		float x_mes_1000[1000];
		float y_mes_1000[1000];
		float r_mes_1000[1000];

		float* Get_x_mes_1000(void);
		float* Get_y_mes_1000(void);
		float* Get_r_mes_1000(void);

		void Get_measures(void);

	private:
		void run();

		bool endThread;
		std::thread * threadTest;
};

extern UserPatternDetectionTest UserDetectionTest;

#endif

