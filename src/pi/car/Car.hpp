#ifndef __CAR_HPP__
#define __CAR_HPP__

#include <mutex>
extern "C" {
#include "../../stm32/KeilProject/Sources/Barstow/Control.h"
#include "../../stm32/KeilProject/Sources/Barstow/Model.h"
}

/**
 * This class is fully static (cannot create an instance of Car).
 * It contains structures that describe the state of the car (sensors) and commands to send.
 * The internal structures are protected with mutex and can be read or writen using copy of these structures.
 */
class Car {
	public:
		enum Motor {DirectionMotor, LeftWheelMotor, RightWheelMotor, BothWheelMotors};
		enum Moving {MoveForward, MoveBackward, Stop };
		enum Turn {TurnLeft, TurnRight, NoTurn };

		/** Copy control structure in 'out' **/
		static void getControlStructure(BarstowControl_Typedef & out);
		/** Copy model structure in 'out' **/
		static void getModelStructure(BarstowModel_Typedef & out);
		/** Write control to the targeted motor **/
		static void writeControlMotor(Car::Motor target, MotorControl_Typedef & control);
		/** Write control to the propulsion motor **/
		static void writeControlMotor(Car::Moving action, float speed);
		/** Write control to the direction motors **/
		static void writeControlMotor(Car::Turn action, float speed);
		/** Write model (could be used to simulate sensor output, if SPI communication is disabled) **/
		static void updateModelStructure(BarstowModel_Typedef & model);
		/** Write control to the gyro**/
		static void writeControlGyro(bool isGyrophareOn);
		/**Size of the car**/
		static const float CarSize;
		/**Width of the car**/
		static const float CarWidth;

	private:
		// disallow creating an instance of this object
		Car() {}
		// Structures model and control, they are private and must be accessed after mutex lock
		static std::mutex controlMutex;
		static std::mutex modelMutex;
		static BarstowControl_Typedef controlStructure;
		static BarstowModel_Typedef modelStructure;
};

#endif

