#ifndef __TIMER_HPP__
#define __TIMER_HPP__

class Timer
{
	public:
		Timer();
		void reset();
		// In seconds
		double elapsed() const;

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> begin;
};

#endif

