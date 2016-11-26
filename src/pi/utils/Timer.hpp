#ifndef __TIMER_HPP__
#define __TIMER_HPP__

#include <chrono>
/**
 * A timer allows to easily get elapsed time between to point in a code.
 * 
 * Example of use:
 * 		Timer timer;
 *
 * 		function1();
 *
 * 		double t1 = timer.elapsed();
 * 		timer.reset();
 *
 * 		function2();
 *
 * 		double t2 = timer.elapsed();
 *
 * 		std::cout << "Functions 1 and 2 took " << (t1+t2) << " seconds" << std::endl;
 **/
class Timer {
	public:
		/** Create a timer with the current time **/
		Timer();
		/** Reset the timer to the current time **/
		void reset();
		/** Return the elapsed time in seconds since the last call to the constructor or to reset() **/
		double elapsed() const;

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> begin;
};

#endif

