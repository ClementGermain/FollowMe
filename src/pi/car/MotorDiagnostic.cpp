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

DiagnosticMotor::DiagnosticMotor(const char * filename, Car::Motor MotorType_, int size_model) :  delta_voltage(600.0), MotorModel_Prop(size_model), failure(NO), MotorType(MotorType_)
{
  Car::getControlStructure(BarstowControl);
  Car::getModelStructure(BarstowModel);
  MotorModel_Prop.load(filename);
}

Failure_Typedef DiagnosticMotor::getFailure(){
  return failure;
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
  float volt1, volt2;

  Car::getModelStructure(BarstowModel);

  switch (MotorType){
  case Car::LeftWheelMotor:
    volt1 =  BarstowModel.leftWheelMotor.voltage1;
    volt2 =  BarstowModel.leftWheelMotor.voltage2;
    break;
  case Car::RightWheelMotor:
    volt1 =  BarstowModel.rightWheelMotor.voltage1;
    volt2 =  BarstowModel.rightWheelMotor.voltage2;
    break;
  case Car::BothWheelMotors:
    break;
  case Car::DirectionMotor:
    volt1 =  BarstowModel.directionMotor.voltage1;
    volt2 =  BarstowModel.directionMotor.voltage2;
    break;
  }
  
  if ((fabs(getValVoltage(v1) - volt1) < delta_voltage) ||
	(fabs(getValVoltage(v2) - volt2) < delta_voltage))
    failure = CMD;
  else
    failure = NO;
}

float DiagnosticMotor::getCmd(){
  float cmd;

  Car::getControlStructure(BarstowControl);

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

