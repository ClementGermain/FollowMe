#ifndef __IA_HPP__
#define __IA_HPP__

#include <thread>
#include "car/Car.hpp"

class IA {
	public:
		static void start();
		static void stop();
	
		static void SpeedControl(float distanceUserToCamera, bool isUserDetected);
		static void DirectionControl(float angleUserToCamera, bool isUserDetected, bool endOfCourseLeft, bool endOfCourseRight);
		static void IAMotorBack();
		static void IAMotorDirection();

	private:		
		static float Speed;		
		static float directionSpeed;
		static Car::Turn Direction; //{TurnLeft, TurnRight, NoTurn};

		static void run();

		static bool endThread;
		static std::thread * threadTest;

};


#endif
