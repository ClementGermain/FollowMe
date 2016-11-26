#include <fstream>
#include <numeric>
#include <unistd.h>
#include <vector>
#include "CPULoad.hpp"
#include "utils/Timer.hpp"

CPULoad::CPULoad() :
	prevIdleTime(0),
	prevTotalTime(0),
	prevLoad(0)
{

}

/** Return the cpu load in percentage, updated no more than once per second **/
float CPULoad::get() {
	// Update value only once per second
	if(timer.elapsed() < 1.0)
		return prevLoad;

	// read current cpu times
	std::ifstream proc_stat("/proc/stat");
	proc_stat.ignore(5, ' '); // skip prefix

	int index = 0;
	size_t time = 0, total = 0, idle = 0;
	while(proc_stat >> time) {
		if(index == 3)
			idle = time;
		total += time;
		index++;
	}

	// compute the cpu load in percentage
	float idleDelta = idle - prevIdleTime;
	float totalDelta = total - prevTotalTime;
	float load = 100 * (1.0f - idleDelta/totalDelta);

	// Get the average load over the last 2 measures
	float smoothLoad = (load + prevLoad) / 2;

	// save values for the next measure
	prevIdleTime = idle;
	prevTotalTime = total;
	prevLoad = load;
	timer.reset();

	return smoothLoad;
}
