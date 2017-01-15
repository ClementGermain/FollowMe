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
	if(!isRunning()) {
		LogI << "Starting thread \"" << name << "\"..." << std::endl;
		requestEndThread = false;
		runningThread = new std::thread(&Thread::run, this);
	}
}

void Thread::stop() {
	if(isRunning() && !requestEndThread && runningThread != NULL) {
		LogI << "Joining thread \"" << name << "\"..." << std::endl;
		requestEndThread = true;
		if(runningThread->joinable())
			runningThread->join();
		delete runningThread;
		runningThread = NULL;
		LogI << "Terminated: thread \"" << name << "\"" << std::endl;
	}
}

bool Thread::isRunning() {
	return runningThread != NULL;
}

bool Thread::isEndRequested() {
	return requestEndThread;
}
