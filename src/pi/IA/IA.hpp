#ifndef __IA_HPP__
#define __IA_HPP__

#include <thread>
#include "car/Car.hpp"

#define IA_PERIOD 100

/**
 * Artificial intelligence of the car.
 * Make the car follow the user with unknown reliability...
 * Must stop the car in case of emergency.
 * It sends commands to motor, read sensor values, and use road and user detection.
 */
class IA {
	public:
		static void start();
		static void stop();
	
		static void SpeedControl(float distanceUserToCamera, bool isUserDetected);
		static void DirectionControl(float angleUserToCamera, bool isUserDetected, bool endOfCourseLeft, bool endOfCourseRight);
		static void IAMotorBack();
		static void IAMotorDirection();

		static void toggleRoadDetection();

		static bool enableRoadDetection;
	private:		
		static float Speed;		
		static float directionSpeed;
		static Car::Turn Direction; //{TurnLeft, TurnRight, NoTurn};

		static void run();

		static bool endThread;
		static std::thread * threadTest;
                
		static float uAngleT1;
		static float uAngleT2;

		static float previousAngle;

};


#endif
