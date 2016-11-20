#include <chrono>
#include "Timer.hpp"

using namespace std::chrono;


Timer::Timer() :
	begin(high_resolution_clock::now())
{

}

void Timer::reset() {
	begin = high_resolution_clock::now();
}

double Timer::elapsed() const {
	return duration_cast<std::chrono::duration<double, std::ratio<1>>>
		(high_resolution_clock::now() - begin).count();
}
