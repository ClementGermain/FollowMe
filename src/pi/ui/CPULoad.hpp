#ifndef __CPU_LOAD_HPP__
#define __CPU_LOAD_HPP__

#include <unistd.h>
#include "utils/Timer.hpp"

/**
 * Small class that provides CPU usage.
 * THe CPU usage is in ppercentage and is calculated with the average over the last second.
 * The method get() must be call frequently to keep the value up to date.
 */
class CPULoad {
	public:
		CPULoad();
		/** Return the cpu load in percentage, updated no more than once per second **/
		float get();
	private:
		size_t prevIdleTime;
		size_t prevTotalTime;
		Timer timer;
		float prevLoad;
};

#endif
