#ifndef __PERIODIC_THREAD_HPP__
#define __PERIODIC_THREAD_HPP__

#include "Thread.hpp"

class PeriodicThread : public Thread {
	public:
		/** param: Period in millisecond between two calls of loop() **/
		PeriodicThread(int periodMillis, std::string name);
	protected:
		// callback called before the first loop
		virtual void begin();
		// callback called every 'periodMilliseconds'
		virtual void loop() = 0;
		// callbak calle after the last loop, before the thread stops
		virtual void end();
		
	private:
		void run();
		double periodMilliseconds;

};

#endif
