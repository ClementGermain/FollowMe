#include "MotorModel.hpp"
#include <cmath>
#include <unistd.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <string.h>
#include "../utils/Log.hpp"

using namespace std;

MotorModel::~MotorModel() {
	delete[] Model;
}

MotorModel::MotorModel(int sizeModel_) : sizeModel (sizeModel_),
	Model(new Model_TypeDef[sizeModel])
{
	// init the state model
	for (int i=0 ; i<sizeModel ; i++) {
		Model[i].cmd = 0;
		Model[i].MotorModel.current = 0;
		Model[i].MotorModel.voltage1 = 0;
		Model[i].MotorModel.voltage2 = 0;
		Model[i].MotorModel.speed = 0;
	}
}

void MotorModel::create(float CmdStart, float CmdStop, float waitTime){

	BarstowModel_Typedef BarstowModel;
	int i=0;
	float cmd = CmdStart;

	while ( i<sizeModel ){
		cmd = CmdStart + (CmdStop - CmdStart)*i/sizeModel;

		if (cmd < 0)
			Car::writeControlMotor(Car::MoveBackward, -cmd);
		else if (cmd >= 0)
			Car::writeControlMotor(Car::MoveForward, cmd);

		if (i==0)
			usleep(500000);
		else
			usleep(waitTime);
		Car::getModelStructure(BarstowModel);
		Model[i].cmd = cmd;
		Model[i].MotorModel.current = BarstowModel.leftWheelMotor.current;
		Model[i].MotorModel.voltage1 = BarstowModel.leftWheelMotor.voltage1;
		Model[i].MotorModel.voltage2 = BarstowModel.leftWheelMotor.voltage2;
		Model[i].MotorModel.speed = BarstowModel.leftWheelMotor.speed; 

		LogD << "Cmd : " << Model[i].cmd << endl;
		LogD << "Val1 : " << Model[i].MotorModel.voltage1 << endl;
		LogD << "Val2 : " << Model[i].MotorModel.voltage2 << endl;
		
		i++;
	}
	Car::writeControlMotor(Car::Stop, 0.0);
}

void MotorModel::save(const char * fileName){
	char filepath[150] = "../../res/model/";
	strcat(filepath, fileName);
	strcat(filepath, ".bin");

	FILE * file = fopen(filepath, "wb");
	if (file){
	  LogD << "Nb Objets ecrit : " << fwrite(Model, sizeof(Model_TypeDef), sizeModel, file) << endl;
	  fclose(file);
	}
}

void MotorModel::load(const char * fileName){
	char filepath[150] = "../../res/model/";
	strcat(filepath, fileName);
	strcat(filepath, ".bin");

	FILE * file = fopen(filepath, "rb");

	if (file){
	  fread(Model, sizeof(Model_TypeDef), sizeModel, file);
	  fclose(file);
	}
}

void MotorModel::getState(float cmd, MotorModel_Typedef & MotorModel){
	int index=0;
	float delta=100000;
	for (int i=0; i<sizeModel ; i++){
		if (abs(Model[i].cmd - cmd) < delta){
			delta = abs(Model[i].cmd - cmd);
			index = i;
		}
	}
	MotorModel = Model[index].MotorModel;
}

float MotorModel::getVoltage(float cmd, numVoltage n){
  int index=0;
  float delta=1000000;
  for (int i=0 ; i<sizeModel ; i++){
    if (abs(Model[i].cmd - cmd) < delta){
	delta = abs(Model[i].cmd - cmd);
	index = i;
    }
  }
  if (n==1)
    return Model[index].MotorModel.voltage1;
  else if (n==2)
    return Model[index].MotorModel.voltage2;
  else
    return 0;
}
