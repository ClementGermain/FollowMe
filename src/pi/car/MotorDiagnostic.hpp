#ifndef __MOTORDIAGNOSTIC__
#define __MOTORDIAGNOSTIC__

#include <string>
#include <cstdio>
#include "MotorModel.hpp"
#include "../../stm32/KeilProject/Sources/Barstow/Model.h"


class DiagnosticMotor : public MotorModel{

	public:
		//constructor
		DiagnosticMotor(void);
		DiagnosticMotor(int size_model);
		//create and save the model
		void Diagnostic_create_and_save_model(int CmdStart, int CmdStop, float waitTime, const std::string & fileName);

		//function comparison between the Barstow Model and the Model, attribut in my class
		
		bool Diagnostic_panne (BarstowModel_Typedef);
	
	protected: 
		bool panne;



};





#endif
