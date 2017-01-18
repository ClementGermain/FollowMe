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
	float cmd = 0;
	
	// ramp to cmd = CmdStart to have a smooth acceleration	
	while (abs(cmd) < abs(CmdStart)){
	  cmd += CmdStart / 100.0 ;
	  writeCmd(cmd);
	  usleep(10000);
	  i++;
	}
	
	// Model Acquisition
	i=0;
	cmd = CmdStart;
	while ( i<sizeModel ){
		cmd = CmdStart + (CmdStop - CmdStart)*i/sizeModel;
		writeCmd(cmd); // send a command to the motors

		if (i==0)
			usleep(500000);
		else
			usleep(waitTime); // wait to give the motor enough time to be stable

		// get the motors values and store them into the Model tab
		Car::getModelStructure(BarstowModel);
		Model[i].cmd = cmd;
		Model[i].MotorModel.current = BarstowModel.rightWheelMotor.current;
		Model[i].MotorModel.voltage1 = BarstowModel.rightWheelMotor.voltage1;
		Model[i].MotorModel.voltage2 = BarstowModel.rightWheelMotor.voltage2;
		Model[i].MotorModel.speed = BarstowModel.rightWheelMotor.speed;
		i++;
	}

	// ramp to cmd = 0 to have a smooth decelration	
	i=0;
	while (cmd > 0){
	  cmd -= CmdStop / 100.0 ;
	  writeCmd(cmd);
	  usleep(10000);
	  i++;
	}

	// stop the car
	Car::writeControlMotor(Car::Stop, 0.0);
}

void MotorModel::save(const char * filename){

	// create the filepath
	char filepath[150] = "../../res/model/";
	strcat(filepath, filename);
	strcat(filepath, ".bin");
	
	// open/create the file
	FILE * file = fopen(filepath, "wb");

	// write the datas
	if (file){
	  LogD << "Nb Objets ecrit : " << fwrite(Model, sizeof(Model_TypeDef), sizeModel, file) << endl;
	  fclose(file);
	}
}

void MotorModel::load(const char * filename){

	// create the filepath
	char filepath[150] = "../../res/model/";
	strcat(filepath, filename);
	strcat(filepath, ".bin");

	// open the file
	FILE * file = fopen(filepath, "rb");

	// read the file
	if (file){
	  fread(Model, sizeof(Model_TypeDef), sizeModel, file);
	  fclose(file);
	}
}

void MotorModel::exportTxt(const char * filename){
	
	// load some data from a binary file
  this->load(filename);

	// create the filepath
  char filepath[150] = "../../res/model/";
  strcat(filepath, filename);
  strcat(filepath, ".txt");
  
	//open/create the file and write the datas
  ofstream file(filepath , ios::out | ios::trunc); 

	// write the datas
  if (file){
    file << "Cmd Voltage1 Voltage2 Current" << endl;
    for (int i=0 ; i<sizeModel ; i++){
			file << Model[i].cmd << " " << Model[i].MotorModel.voltage1 << " "  << Model[i].MotorModel.voltage2 << " " << Model[i].MotorModel.current << endl;
    }
    file.close();
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

void MotorModel::writeCmd(float cmd){
  if (cmd < 0)
    Car::writeControlMotor(Car::MoveBackward, -cmd);
  else if (cmd >= 0)
    Car::writeControlMotor(Car::MoveForward, cmd);
}

