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
} failure_TypeDef;

class DiagnosticMotor : public MotorModel{
  
public:
  //constructor
  DiagnosticMotor(const char * fileName,  BarstowControl_Typedef * BarstowControl_, int size_model = 1200);

  void getFailure(failure_TypeDef * failure);

  float getDeltaVoltage();
  float getValVoltage();
  
protected:
  Model_TypeDef * Model;
  float delta_voltage;
  BarstowControl_Typedef * BarstowControl;
};
#endif
