#include "Thread.hpp"

using namespace std;


Thread::Thread() : requestEndThread(true), runningThread(NULL)
{

}


Thread::~Thread() {
	stop();
}

void Thread::start() {
	if(!isRunning()) {
		requestEndThread = false;
		thread = new Thread(run, this);
	}
}

void Thread::stop() {
	if(isRunning()) {
		requestEndThread = true;
		thread->join();
		delete thread;
		thread = NULL;
	}
}

bool Thread::isRunning() {
	return thread != NULL;
}

bool Thread::isEndRequested() {
	return requestEndThread;
}
