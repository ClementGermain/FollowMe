#include "Timer.hpp"
#include <chrono>

using namespace std::chrono;


/** Create a timer with the current time **/
Timer::Timer() :
	begin(high_resolution_clock::now())
{

}

/** Reset the timer to the current time **/
void Timer::reset() {
	begin = high_resolution_clock::now();
}

/** Return the elapsed time in seconds since the last call to the constructor or to reset() **/
double Timer::elapsed() const {
	return duration_cast<std::chrono::duration<double, std::ratio<1>>>
		(high_resolution_clock::now() - begin).count();
}
