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
	MotorModel::save(std::fileName);


/*bool DiagnosticMOTOR::Diagnostic_panne (BarstowModel_Typedef barstow_model){
	

	MotorModel::getState(barstow_model.directionMotor.);*/


}



