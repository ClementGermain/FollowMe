#include "MotorModel.hpp"
#include <cmath>
#include <unistd.h>
#include <iostream>
#include <fstream>



DiagnosticMotor::DiagnosticMotor(void):MotorModel(),panne(false){}

DiagnosticMotor::DiagnosticMotor(size_model):MotorModel(size_model),panne(false){}

DiagnosticMotor::Diagnostic_create_and_save_model(int CmdStart, int CmdStop, float waitTime, const std::string & fileName){
	 MotorModel::create(int CmdStart, int CmdStop, float waitTime);
	MotorModel::save(const std::string & fileName);

}



