#include "MotorModel.hpp"
#include <cmath>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include "../utils/Log.hpp"
#include "MotorDiagnostic.hpp"
/*
using namespace std;

DiagnosticMotor::DiagnosticMotor(const char * fileName,  BarstowControl_Typedef * BarstowControl_, int size_model = 1200): BarstowControl(BarstowControl_)
{
  Model_TypeDef * Model();
  Model->load(filename);
  failure_TypeDef failure = {False, False};
  float delta_voltage = 0.2; //V 
  }

void getDeltaVoltage(){
  return delta_voltage;
}

float getValVotage1(){
  float cmd = BarstowControl->propulsionMotor.speed;
  if (BarstowControl->propulsionMotor.direction == -1)
    cmd = -cmd;
  MotorModel_Typedef MotorModel = Model->getState(cmd);
  return MotorModel.voltage1

*/
