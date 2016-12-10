#ifndef __IA_HPP__
#define __IA_HPP__

#include <thread>

class IA {
	public:
		static void start();
		static void stop();
	
		static void SpeedControl(float distanceUserToCamera, bool isUserDetected);
		static void IAMotorBack();

	private:		
		static float Speed;		

		static void run();

		static bool endThread;
		static std::thread * threadTest;

};


#endif
