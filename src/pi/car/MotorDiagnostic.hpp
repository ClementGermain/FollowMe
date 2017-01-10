#ifndef __MOTORDIAGNOSTIC__
#define __MOTORDIAGNOSTIC__

#include <string>
#include <cstdio>
#include <thread>
#include "MotorModel.hpp"
#include "../../stm32/KeilProject/Sources/Barstow/Model.h"
#include "../../stm32/KeilProject/Sources/Barstow/Control.h"
#include "Car.hpp"

enum Failure_Typedef {NO, CMD, CURRENT, SPEED};

class DiagnosticMotor {
  
public:
  //constructor
  DiagnosticMotor(const char * filename,  Car::Motor MotorType_, int size_model = 1200);
  
  void start();
  void stop();
  
  void compareModel();
  Failure_Typedef getFailure();
  float getMinVoltage(numVoltage n);
  float getMaxVoltage(numVoltage n);
  
  float getCmd();
  
protected:
  
  BarstowControl_Typedef BarstowControl;
   BarstowModel_Typedef BarstowModel;

  float delta_voltage;
  float ValVoltage[2];
  float getValVoltage(numVoltage n);

  void run();
  bool endThread;
  std::thread * threadTest;

  MotorModel MotorModel_Prop;
  Failure_Typedef failure;
  Car::Motor MotorType;
};
#endif
