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

void writeCmd(float cmd){
  if (cmd < 0)
    Car::writeControlMotor(Car::MoveBackward, -cmd);
  else if (cmd >= 0)
    Car::writeControlMotor(Car::MoveForward, cmd);
}

void MotorModel::create(float CmdStart, float CmdStop, float waitTime){

	BarstowModel_Typedef BarstowModel;
	int i=0;
	float cmd = 0;
	
	// ramp to have a smooth acceleration	
	while (abs(cmd) < abs(CmdStart)){
	  cmd += CmdStart / 100.0 ;
	  writeCmd(cmd);
	  usleep(10000);
	  i++;
	}
	
	i=0;
	cmd = CmdStart;
	// Model Acquisition
	while ( i<sizeModel ){
		cmd = CmdStart + (CmdStop - CmdStart)*i/sizeModel;
		writeCmd(cmd);

		if (i==0)
			usleep(500000);
		else
			usleep(waitTime);
		Car::getModelStructure(BarstowModel);
		Model[i].cmd = cmd;
		Model[i].MotorModel.current = BarstowModel.rightWheelMotor.current;
		Model[i].MotorModel.voltage1 = BarstowModel.rightWheelMotor.voltage1;
		Model[i].MotorModel.voltage2 = BarstowModel.rightWheelMotor.voltage2;
		Model[i].MotorModel.speed = BarstowModel.rightWheelMotor.speed; 
		/*
		LogD << "Cmd : " << Model[i].cmd << endl;
		LogD << "Val1 : " << Model[i].MotorModel.voltage1 << endl;
		LogD << "Val2 : " << Model[i].MotorModel.voltage2 << endl;
		*/
		i++;
	}

	// ramp to have a smooth decelration	
	i=0;
	while (cmd > 0){
	  cmd -= CmdStop / 100.0 ;
	  writeCmd(cmd);
	  usleep(10000);
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

float MotorModel::getVoltage(float cmd, numVoltage n){
  switch (n){
  case v1:
    return Model[getIndex(cmd)].MotorModel.voltage1;
    break;
  case v2:
    return Model[getIndex(cmd)].MotorModel.voltage2;
    break;
  default:
    return 0;
    break;
  }
}

float MotorModel::getCurrent(float cmd){
  return Model[getIndex(cmd)].MotorModel.current;
}


int MotorModel::getIndex(float cmd){
  int index=0;
  float delta=1000000;
  for (int i=0 ; i<sizeModel ; i++){
    if (abs(Model[i].cmd - cmd) < delta){
	delta = abs(Model[i].cmd - cmd);
	index = i;
    }
  }
  return index;
}


