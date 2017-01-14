#ifndef __PERIODIC_THREAD_HPP__
#define __PERIODIC_THREAD_HPP__

#include "Thread.hpp"

class PeriodicThread : public Thread {
	public:
		PeriodicThread(double periodMillis);
		virtual void begin();
		virtual void loop() = 0;
		virtual void end();
		
	private:
		void run();
		double periodSecond;

};

#endif
