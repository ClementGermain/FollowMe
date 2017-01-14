#include "PeriodicThread.hpp"
#include "utils/Timer.hpp"
#include <chrono>

using namespace std;


PeriodicThread::PeriodicThread(double periodSecond) : periodSecond(periodSecond)
{

}

void PeriodicThread::run() {
	// call begin() callback
	begin();

	while(!isEndRequested()) {
		Timer timer;
		
		// call loop() callback
		loop();
		
		// sleep for period
		double sleepTime = periodSecond - timer.elapsed();
		if(sleepTime > 0)
			this_thread::sleep_for(chrono::milliseconds(sleepTime * 1000));
	}
	
	// call end() callback
	end();
}

void PeriodicThread::begin() { }
void PeriodicThread::end() { }
