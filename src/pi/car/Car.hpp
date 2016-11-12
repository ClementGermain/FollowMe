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

		static void getControlStructure(BarstowControl_Typedef & out);
		static void getModelStructure(BarstowModel_Typedef & out);
		static void writeControlMotor(Car::Motor target, MotorControl_Typedef & control);
		static void writeControlMotor(Car::Moving action, float speed);
		static void writeControlMotor(Car::Turn action, float speed);
		static void updateModelStructure(BarstowModel_Typedef & model);

	private:
		// disallow creating an instance of this object
		Car() {}
		static std::mutex controlMutex;
		static std::mutex modelMutex;
		static BarstowControl_Typedef controlStructure;
		static BarstowModel_Typedef modelStructure;
};

#endif

