#include "Thread.hpp"


Thread::Thread() : requestEndThread(true), runningThread(NULL)
{

}


Thread::~Thread() {
	stop();
}

void Thread::start() {
	if(!isRunning()) {
		requestEndThread = false;
		runningThread = new std::thread(&Thread::run, this);
	}
}

void Thread::stop() {
	if(isRunning()) {
		requestEndThread = true;
		runningThread->join();
		delete runningThread;
		runningThread = NULL;
	}
}

bool Thread::isRunning() {
	return runningThread != NULL;
}

bool Thread::isEndRequested() {
	return requestEndThread;
}
