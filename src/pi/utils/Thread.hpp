#ifndef __THREAD_HPP__
#define __THREAD_HPP__

#include <thread>
#include <string>

class Thread {
	public:
		Thread(std::string name);
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
		std::string name;
};

#endif
