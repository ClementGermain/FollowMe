#ifndef __MOTORDIAGNOSTIC__
#define __MOTORDIAGNOSTIC__

#include <string>
#include <cstdio>
#include "MotorModel.hpp"
#include "../../stm32/KeilProject/Sources/Barstow/Model.h"
#include "../../stm32/KeilProject/Sources/Barstow/Control.h"

typedef struct{
  bool yes;              //if there is at least one failure
  bool failure_cmd;
  bool failure_current;
  bool failure_speed;
} failure_TypeDef;

class DiagnosticMotor {
  
public:
  //constructor
  DiagnosticMotor(const char * filename_, int size_model);
  
  void loadModel();
  void createModel(int CmdStart, int CmdStop, float waitTime);
  void compareModel();

  void getFailure(failure_TypeDef * failure);
  
  float getDeltaVoltage();
  float getValVoltage1();
  float getValVoltage2();

  float getCmd_Propulsion();

protected:
  const char * filename;
  BarstowControl_Typedef BarstowControl;
  BarstowModel_Typedef BarstowModel;
  float delta_voltage;
  float delta_current;
  MotorModel MotorModel_Prop;
  failure_TypeDef failure;
};
#endif
