#ifndef __IA_HPP__
#define __IA_HPP__

#include <thread>

class IA {
	public:
		static void start();
		static void stop();
	
		static void SpeedControl(float);
		static void IAMotorBack(float);

	private:		
		static float Speed;		
		static float Dist;
		static bool ObstacleDetected;
		static bool UserDetected;
		static float RealDistance;
		static float ActualSpeed;
		static float TargetSpeed;

		static void run();

		static bool endThread;
		static std::thread * threadTest;

};


#endif
