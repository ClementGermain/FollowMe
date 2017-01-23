#include <iostream>
#include "utils/Log.hpp"
#include "Thread.hpp"


Thread::Thread(std::string name) : requestEndThread(true), runningThread(NULL), name(name)
{

}


Thread::~Thread() {
	stop();
}

void Thread::start() {
	// start thread if not currently running
	if(!isRunning()) {
		// display log message
		LogI << "Starting thread \"" << name << "\"..." << std::endl;
		// start std::thread with the function run()
		requestEndThread = false;
		runningThread = new std::thread(&Thread::run, this);
	}
}

void Thread::stop() {
	// stop thread if currently running
	if(isRunning() && !requestEndThread && runningThread != NULL) {
		// display log message
		LogI << "Joining thread \"" << name << "\"..." << std::endl;

		// send en request
		requestEndThread = true;
		// wait for the end
		if(runningThread->joinable())
			runningThread->join();
		// delete  std::thread object
		delete runningThread;
		runningThread = NULL;

		// display log message
		LogI << "Terminated: thread \"" << name << "\"" << std::endl;
	}
}

bool Thread::isRunning() {
	// thread is considered runnnig while the std::thread object is not deleted
	return runningThread != NULL;
}

bool Thread::isEndRequested() {
	return requestEndThread;
}
