#include "MotorModel.hpp"
#include <cmath>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <algorithm>
#include "../utils/Log.hpp"
#include "MotorDiagnostic.hpp"
#include "Car.hpp"

using namespace std;


DiagnosticMotor::DiagnosticMotor(const char * filename,  Car::Motor MotorType_, int size_model): MotorModel_Prop(1200){

  threadTest = NULL;
  endThread = true;

  delta_voltage = 600.0;
  MotorModel_Prop.load("model_propulsion");
  failure = NO;
  MotorType = Car::LeftWheelMotor;
  ValVoltage[0] = 0;
  ValVoltage[1] = 0;
}

Failure_Typedef DiagnosticMotor::getFailure(){
  return failure;
}

float DiagnosticMotor::getValVoltage(numVoltage n){
  float cmd = getCmd();
  static int a=0;
  LogI << "Cmd = " << cmd << "  " << a << " " << MotorModel_Prop.getVoltage(cmd, n) << endl;
  a++;
  return MotorModel_Prop.getVoltage(cmd, n);
}

float DiagnosticMotor::getMinVoltage(numVoltage n){
  return ValVoltage[n+1] - delta_voltage;
}

float DiagnosticMotor::getMaxVoltage(numVoltage n){
  return ValVoltage[n+1] + delta_voltage;
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
  
  if ((fabs(ValVoltage[0] - volt1) < delta_voltage) ||
	(fabs(ValVoltage[1] - volt2) < delta_voltage))
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
    cmd = (BarstowControl.directionMotor.direction==1) ? cmd : -cmd;
    break;
  case Car::LeftWheelMotor:
  case Car::RightWheelMotor:
  case Car::BothWheelMotors:
    cmd = BarstowControl.propulsionMotor.speed;
    cmd = (BarstowControl.propulsionMotor.direction==1) ? cmd : -cmd;
    break;
  }
  return cmd;
}


// ------------ Thread management -------- //
void DiagnosticMotor::start() {
  if(threadTest == NULL) {
    endThread = false;
    threadTest = new thread([this] { this->run();});
  }
}

void DiagnosticMotor::stop() {
  if(threadTest != NULL) {
    endThread = true;
    threadTest->join();
    delete threadTest;
    threadTest = NULL;
  }
}

void DiagnosticMotor::run() {
  while(!DiagnosticMotor::endThread) {
    
    numVoltage v;
    for (int i=0; i<2; i++){
	v=(i=0)?v1:v2;
	LogD << "Model_V : " << getValVoltage(v) << endl;
	ValVoltage[i] = getValVoltage(v);
    }
    compareModel();
    
    // sleep 
    this_thread::sleep_for(chrono::milliseconds(100));
  }
}
// --------------------------------------- //
