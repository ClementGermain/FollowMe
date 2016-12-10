#include "MotorModel.hpp"
#include <cmath>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include "../utils/Log.hpp"
#include "MotorDiagnostic.hpp"

using namespace std;

DiagnosticMotor::DiagnosticMotor(void):MotorModel(),panne(false){}

DiagnosticMotor::DiagnosticMotor(int size_model):MotorModel(size_model),panne(false){}

void DiagnosticMotor::Diagnostic_create_and_save_model(int CmdStart, int CmdStop, float waitTime, const std::string & fileName){
	 MotorModel::create(CmdStart, CmdStop, waitTime);
	//MotorModel::save(std::fileName);
}

/*bool DiagnosticMotor::Comparison_RealModel(int delta_current, int delta_voltage1, int delta_voltage2, int delta_speed){

	model=MotorModel::getState(Cmd);
	
	if (not((Model_value.current-delta_current)<barstow_model.directionMotor) && ((Model_value.current+delta_current)>barstow_model.directionMotor)){panne=true;}

	if (not((Model_value.current-delta_current)<barstow_model.leftWheelMotor) && ((Model_value.current+delta_current)>barstow_model.leftWheelMotor)){panne=true;}
	
	if (not((Model_value.current-delta_current)<barstow_model.rightWheelMotor) && ((Model_value.current+delta_current)>barstow_model.rightWheelMotor)){panne=true;}
return panne;
}*/




