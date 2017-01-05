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

DiagnosticMotor::DiagnosticMotor(const char * filename, Car::Motor MotorType_, int size_model) :  delta_voltage(0.2), MotorModel_Prop(size_model), failure({false, false, false, false}), MotorType(MotorType_)
{
  Car::getControlStructure(BarstowControl);
  Car::getModelStructure(BarstowModel);
  MotorModel_Prop.load(filename);
}

float DiagnosticMotor::getDeltaVoltage(){
  return delta_voltage;
}

float DiagnosticMotor::getValVoltage(numVoltage n){
  float cmd = getCmd();
  return MotorModel_Prop.getVoltage(cmd, n);
}

float DiagnosticMotor::getMinVoltage(numVoltage n){
  return getValVoltage(n) - delta_voltage;
}

float DiagnosticMotor::getMaxVoltage(numVoltage n){
  return getValVoltage(n) + delta_voltage;
}

void DiagnosticMotor::compareModel(){
  
  if ((fabs(getValVoltage(v1) - BarstowModel.leftWheelMotor.voltage1) < delta_voltage) ||
	(fabs(getValVoltage(v2) - BarstowModel.leftWheelMotor.voltage2) < delta_voltage))
    failure.failure_cmd = true;
  else
    failure.failure_cmd = false;

    if (failure.failure_cmd || failure.failure_current || failure.failure_speed)
	failure.yes = true;
    else failure.yes = false;
}

float DiagnosticMotor::getCmd(){
  float cmd;  
  switch (MotorType){
  case Car::DirectionMotor:
    cmd = BarstowControl.directionMotor.speed;
    cmd = (BarstowControl.directionMotor.direction==-1) ? cmd : -cmd;
    break;
  case Car::LeftWheelMotor:
  case Car::RightWheelMotor:
  case Car::BothWheelMotors:
    cmd = BarstowControl.propulsionMotor.speed;
    cmd = (BarstowControl.propulsionMotor.direction==-1) ? cmd : -cmd;
    break;
  }
  return cmd;
}

