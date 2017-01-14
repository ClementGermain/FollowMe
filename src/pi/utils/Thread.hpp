#ifndef __THREAD_HPP__
#define __THREAD_HPP__

#include <thread>

class Thread {
	public:
		Thread();
		~Thread();
		void start();
		void stop();
		bool isRunning();
	protected:
		bool isEndRequested();
		virtual void run() = 0;
	private:
		bool requestEndThread;
		std::thread * runningThread;
};

#endif
