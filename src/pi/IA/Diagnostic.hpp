#ifndef __DIAGNOSTIC__
#define __DIAGNOSTIC__

#include <thread>
#include "../car/MotorModel.hpp"
#include "../car/Car.hpp"

class Diagnostic {
    public:
        static void start();
        static void stop();

        static void diagnostic();

    protected:

        static void run();
        static void init();
        static void update();

        static bool failure;
        static bool failureMotor;

        BarstowControl_Typedef * BarstowControl;
        BarstowModel_Typedef * BarstowModel;
        MotorModel_Typedef MotorM;
        MotorModel Model;

        static float delta_voltage;
        static float delta_current;
        static float delta_speed;
};

#endif