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

bool DiagnosticMotor::failureDetected = false;

DiagnosticMotor Diag_Prop_Right("model_propulsion", Car::RightWheelMotor);
DiagnosticMotor Diag_Prop_Left("model_propulsion", Car::LeftWheelMotor);

DiagnosticMotor::DiagnosticMotor(const char * filename,  Car::Motor MotorType_, int size_model): MotorModel_Prop(1200){

  threadTest = NULL;
  endThread = true;

  delta_voltage = 600.0;
  delta_current = 100.0;
  MotorModel_Prop.load("model_propulsion");
  failure = NO;
  MotorType = MotorType_;
  ValVoltage[0] = 0;
  ValVoltage[1] = 0;
  ValCurrent = 0;
  delay_compt = 0;
  delay = 5;
}

Failure_Typedef DiagnosticMotor::getFailure(){
  return failure;
}

bool DiagnosticMotor::isFailureDetected() {
  return failureDetected;
}

void DiagnosticMotor::checkFailure(){
  if (failure!=NO)
    failureDetected = true;
  else 
    failureDetected = false;   
}

float DiagnosticMotor::getValVoltage(numVoltage n){
  float cmd = getCmd();
  //if (n==v1)
    //LogD << "Cmd: " << cmd << endl;
    //LogD << "Diagnostic : Voltage" << n << " : " << MotorModel_Prop.getVoltage(cmd,n) << endl;
  return MotorModel_Prop.getVoltage(cmd, n);
}

float DiagnosticMotor::getValCurrent(){
  return MotorModel_Prop.getCurrent(getCmd());
}

float DiagnosticMotor::getMinVoltage(numVoltage n){
  return ValVoltage[n-1] - delta_voltage;
}

float DiagnosticMotor::getMaxVoltage(numVoltage n){
  return ValVoltage[n-1] + delta_voltage;
}

float DiagnosticMotor::getMaxCurrent(){
  return ValCurrent + delta_current;
}

float DiagnosticMotor::getMinCurrent(){
  return ValCurrent - delta_current;
}

void DiagnosticMotor::compareModel(){
  float volt1, volt2, current;
  Car::getModelStructure(BarstowModel);

  switch (MotorType){
  case Car::LeftWheelMotor:
    volt1 =  BarstowModel.leftWheelMotor.voltage1;
    volt2 =  BarstowModel.leftWheelMotor.voltage2;
    current =  BarstowModel.leftWheelMotor.current;
    break;
  case Car::RightWheelMotor:
    volt1 =  BarstowModel.rightWheelMotor.voltage1;
    volt2 =  BarstowModel.rightWheelMotor.voltage2;
    current =  BarstowModel.rightWheelMotor.current;
    break;
  case Car::BothWheelMotors:
    break;
  case Car::DirectionMotor:
    volt1 =  BarstowModel.directionMotor.voltage1;
    volt2 =  BarstowModel.directionMotor.voltage2;
    current =  BarstowModel.directionMotor.current;
    break;
  }
/*
  LogD << "----------------" << endl;
  LogD << "   MOTOR  MODEL" << endl;
  LogD << "V1 : " << volt1 << "   " << ValVoltage[0] << endl;
  LogD << "V2 : " << volt2 << "   " << ValVoltage[1] << endl;
  LogD << "Cur: " << current << "   " << ValCurrent << endl;
*/
  if ((fabs(ValVoltage[0] - volt1) >= delta_voltage) ||	
	(fabs(ValVoltage[1] - volt2) >= delta_voltage)){
    if (delay_compt < delay)
	delay_compt++;
    else
	failure = CMD;
  }
  else if (fabs(ValCurrent - current) >= delta_current)
    if (delay_compt < delay)
	delay_compt++;
    else
	failure = CURRENT;
  else{
    failure = NO;
    delay_compt = 0;
  }
}

float DiagnosticMotor::getCmd(){
  float cmd;

  Car::getControlStructure(BarstowControl);

  switch (MotorType){
  case Car::DirectionMotor:
    if (BarstowControl.directionMotor.direction==0)
      cmd=0;
    else{
      cmd = BarstowControl.directionMotor.speed;
      cmd = (BarstowControl.directionMotor.direction==1) ? cmd : -cmd;
    }
    break;
  case Car::LeftWheelMotor:
  case Car::RightWheelMotor:
  case Car::BothWheelMotors:
    if (BarstowControl.propulsionMotor.direction==0)
      cmd=0;
    else {
      cmd = BarstowControl.propulsionMotor.speed;
      cmd = (BarstowControl.propulsionMotor.direction==1) ? cmd : -cmd;
    }
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
    
    ValVoltage[0] = getValVoltage(v1);
    ValVoltage[1] = getValVoltage(v2);
    ValCurrent = getValCurrent();

    compareModel();
    
    // sleep 
    this_thread::sleep_for(chrono::milliseconds(100));
  }
}
// --------------------------------------- //
