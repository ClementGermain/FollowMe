#include "MotorModel.hpp"
#include <cmath>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <algorithm>
#include "../utils/Log.hpp"
#include "MotorDiagnostic.hpp"

using namespace std;

DiagnosticMotor::DiagnosticMotor(const char * filename_, int size_model = 1200) : filename(filename_), delta_voltage(0.2), MotorModel_Prop(size_model), failure({false, false, false, false})
{
  Car::getControlStructure(BarstowControl);
  Car::getModelStructure(BarstowModel);
}

float DiagnosticMotor::getDeltaVoltage(){
  return delta_voltage;
}

float DiagnosticMotor::getValVoltage1(){
  float cmd = getCmd_Propulsion();
  return MotorModel_Prop.getVoltage1(cmd);
}

float DiagnosticMotor::getValVoltage2(){
  float cmd = getCmd_Propulsion();
  return MotorModel_Prop.getVoltage2(cmd);
}

void DiagnosticMotor::loadModel(){
  MotorModel_Prop.load(filename);
}
  
void DiagnosticMotor::createModel(int CmdStart, int CmdStop, float waitTime){
  MotorModel_Prop.create(CmdStart, CmdStop, waitTime);
  MotorModel_Prop.save(filename);
}

void DiagnosticMotor::compareModel(){
  
  float cmd_p = getCmd_Propulsion();
  if ((fabs(MotorModel_Prop.getVoltage1(cmd_p) - BarstowModel.leftWheelMotor.voltage1) < delta_voltage) ||
	  (fabs(MotorModel_Prop.getVoltage2(cmd_p) - BarstowModel.leftWheelMotor.voltage2) < delta_voltage))
    failure.failure_cmd = true;
  else
    failure.failure_cmd = false;

    if (failure.failure_cmd || failure.failure_current || failure.failure_speed)
	failure.yes = true;
    else failure.yes = false;
}

float DiagnosticMotor::getCmd_Propulsion(){
  float cmd = BarstowControl.propulsionMotor.speed;
  if (BarstowControl.propulsionMotor.direction == -1)
    cmd = -cmd;
  return cmd;
}

