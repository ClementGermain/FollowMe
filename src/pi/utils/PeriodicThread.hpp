#ifndef __PERIODIC_THREAD_HPP__
#define __PERIODIC_THREAD_HPP__

#include "Thread.hpp"

/**
 * Thread that executes a task with constant period.
 */
class PeriodicThread : public Thread {
	public:
		/** Create a thraad that executes a periodic task. 
		 * param: Period in millisecond between two calls of loop() **/
		PeriodicThread(int periodMillis, std::string name);
	protected:
		//! callback called before the first loop, can be overided by child class
		virtual void begin();
		//! callback called every 'periodMilliseconds', must be overided by child class
		virtual void loop() = 0;
		//! callbak calle after the last loop, before the thread stops, can be overided by child class
		virtual void end();
		
	private:
		//! overide THread::run() to manage period and loop
		void run();
		double periodMilliseconds;

};

#endif
