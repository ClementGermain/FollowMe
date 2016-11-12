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
		enum Motor {DirectionMotor, LeftWheelMotor, RightWheelMotor};

		static void getControlStructure(BarstowControl_Typedef & out);
		static void getModelStructure(BarstowModel_Typedef & out);
		static void writeControlMotor(Motor target, MotorControl_Typedef & control);
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

