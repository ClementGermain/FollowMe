#ifndef __ROAD_DETECTION_TEST_HPP__
#define __ROAD_DETECTION_TEST_HPP__

#include "RoadDetection.hpp"
#include "utils/PeriodicThread.hpp"

/**
 * Periodic task that executes the RoadDetection algorithm.
class RoadDetectionThread : public PeriodicThread {
	public:
		RoadDetectionThread();

		RoadDetection detector;

	private:
		void loop();
};

extern RoadDetectionThread roadDetectionThread;

#endif

