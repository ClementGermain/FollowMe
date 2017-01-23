#ifndef __THREAD_HPP__
#define __THREAD_HPP__

#include <thread>
#include <string>

/**
 * High level pure virtual class for the std::thread class.
 * It gives simple functions to start() and to stop() the thread safely.
 * This class must be inherited by one that overides the run() function.
 */
class Thread {
	public:
		/** Create a thread a name that will appear in logs **/
		Thread(std::string name);
		/** Join and destroy thread if running **/
		~Thread();
		/** Start the thread, do nothing if already running **/
		void start();
		/** Stop and join the thread, do nothing if not running **/
		void stop();
		/** Indicate whether the thread is currently runnnig **/
		bool isRunning();
	protected:
		/** Must be used by child class to know if the run function must end. If the child class ignore it the stop() method will not work properly. **/
		bool isEndRequested();
		/** Pure virutal function that will be run in the thread. It must be overided by child class. **/
		virtual void run() = 0;
	private:
		bool requestEndThread;
		std::thread * runningThread;
		std::string name;
};

#endif
