#include <fstream>
#include <numeric>
#include <unistd.h>
#include <vector>
#include <SDL/SDL.h>
#include "CPULoad.hpp"

CPULoad::CPULoad() :
	prevIdleTime(0), prevTotalTime(0),
	prevDate(0), prevLoad(0)
{

}

float CPULoad::get() {
	Uint32 date = SDL_GetTicks();

	if(date - prevDate < 1000)
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

	float idleDelta = idle - prevIdleTime;
	float totalDelta = total - prevTotalTime;
	float load = 100 * (1.0f - idleDelta/totalDelta);

	prevIdleTime = idle;
	prevTotalTime = total;
	prevDate = date;
	prevLoad = load;

	return load;
}
