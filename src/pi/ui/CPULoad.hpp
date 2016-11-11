#ifndef __CPU_LOAD_HPP__
#define __CPU_LOAD_HPP__

#include <SDL/SDL.h>
#include <unistd.h>

class CPULoad {
	public:
		CPULoad();
		float get();
	private:
		size_t prevIdleTime;
		size_t prevTotalTime;
		Uint32 prevDate;
		float prevLoad;
};

#endif
