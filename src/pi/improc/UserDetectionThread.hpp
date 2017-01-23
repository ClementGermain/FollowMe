#ifndef __USER_PATTERN_DETECTION_TEST_HPP__
#define __USER_PATTERN_DETECTION_TEST_HPP__

#include "utils/PeriodicThread.hpp"
#include "UserDetection.hpp"

/**
 * Periodic task that execute the UserDetection algorithm.
 */
class UserDetectionThread : public PeriodicThread {
	public:
		UserDetectionThread();

		UserDetection detector;
		float x_mes_1000[2][1000];
		float y_mes_1000[2][1000];
		float r_mes_1000[2][1000];

		void Get_measures(void);
		void resetMeasures();
		void printMeasures();

	private:
		void loop();
		void begin();
		int num_mesure, num_kalman;
};

extern UserDetectionThread userDetectionThread;

#endif

