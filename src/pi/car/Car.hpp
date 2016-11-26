#ifndef __CAR_HPP__
#define __CAR_HPP__

#include <mutex>
extern "C" {
#include "../../stm32/KeilProject/Sources/Barstow/Control.h"
#include "../../stm32/KeilProject/Sources/Barstow/Model.h"
}

// This class is fully static
// You cannot create an instance of Car
// You can instead use 'Car::functionName()'
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
		/** Write control to the direction motor **/
		static void writeControlMotor(Car::Moving action, float speed);
		/** Write control to the propulsion motors **/
		static void writeControlMotor(Car::Turn action, float speed);
		/** Write model (could be used to simulate sensor output, if SPI communication is disabled) **/
		static void updateModelStructure(BarstowModel_Typedef & model);

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

