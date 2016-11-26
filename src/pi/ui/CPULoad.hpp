#ifndef __CPU_LOAD_HPP__
#define __CPU_LOAD_HPP__

#include <unistd.h>
#include "utils/Timer.hpp"

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
