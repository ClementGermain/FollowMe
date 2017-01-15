#include "PeriodicThread.hpp"
#include "utils/Timer.hpp"
#include <chrono>

using namespace std;


PeriodicThread::PeriodicThread(int periodMilliseconds, string name) :
	Thread(name + " periodic ("+to_string(periodMilliseconds)+" ms)"),
	periodMilliseconds(periodMilliseconds)
{

}

void PeriodicThread::run() {
	// call begin() callback
	begin();
	
	Timer timer;

	while(!isEndRequested()) {
		timer.reset();

		// call loop() callback
		loop();
		
		// sleep for the rest of the period
		while(!isEndRequested() && (int)(timer.elapsed() * 1000) < periodMilliseconds)
			this_thread::sleep_for(chrono::milliseconds(1));
	}
	
	// call end() callback
	end();
}

void PeriodicThread::begin() { }
void PeriodicThread::end() { }
