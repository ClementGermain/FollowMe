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
=======
DiagnosticMotor::DiagnosticMotor(void):MotorModel(),panne(false){}

DiagnosticMotor::DiagnosticMotor(int size_model):MotorModel(size_model),panne(false){}

void DiagnosticMotor::Diagnostic_create_and_save_model(int CmdStart, int CmdStop, float waitTime, const std::string & fileName){
	 MotorModel::create(CmdStart, CmdStop, waitTime);
	//MotorModel::save(std::fileName);
}

bool DiagnosticMotor::Comparison_RealModel(int delta_current, int delta_voltage1, int delta_voltage2, int delta_speed){

	model=MotorModel::getState(Cmd);
	
	if (not((Model_value.current-delta_current)<barstow_model.directionMotor) && ((Model_value.current+delta_current)>barstow_model.directionMotor)){panne=true;}

	if (not((Model_value.current-delta_current)<barstow_model.leftWheelMotor) && ((Model_value.current+delta_current)>barstow_model.leftWheelMotor)){panne=true;}
	
	if (not((Model_value.current-delta_current)<barstow_model.rightWheelMotor) && ((Model_value.current+delta_current)>barstow_model.rightWheelMotor)){panne=true;}
return panne;
}


float getValVotage1(){
  float cmd = BarstowControl->propulsionMotor.speed;
  if (BarstowControl->propulsionMotor.direction == -1)
    cmd = -cmd;
  MotorModel_Typedef MotorModel = Model->getState(cmd);
  return MotorModel.voltage1

*/
