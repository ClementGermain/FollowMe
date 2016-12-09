#include "MotorModel.hpp"
#include <cmath>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include "../utils/Log.hpp"
#include "../../stm32/KeilProject/Sources/Barstow/Model.h"

using namespace std;

MotorModel::MotorModel(void){
  MotorModel(600);
}

MotorModel::~MotorModel() {
	delete[] model;
}

MotorModel::MotorModel(int sizeModel_) : sizeModel (sizeModel_),
					 model(new StateMotor_TypeDef[sizeModel]){

  // init the state model
  for (int i=0 ; i<sizeModel ; i++){
    model[i].cmd = 0;
    model[i].current = 0;
    model[i].voltage1 = 0;
    model[i].voltage2 = 0;
    model[i].speed = 0;
    }
}
  
void MotorModel::create(float CmdStart, float CmdStop, float waitTime){
  
  BarstowModel_Typedef BarstowModel;
  int i=0;
  float cmd = CmdStart;
  
  while ( i<sizeModel ){
    cmd = CmdStart + (CmdStop - CmdStart)*i/sizeModel;
    Car::writeControlMotor(Car::MoveForward, cmd);
    usleep(waitTime);
    Car::updateModelStructure(BarstowModel);
    LogD << cmd << endl;
    model[i].cmd = cmd;
    model[i].current = BarstowModel.leftWheelMotor.current;
    model[i].voltage1 = BarstowModel.leftWheelMotor.voltage1;
    model[i].voltage2 = BarstowModel.leftWheelMotor.voltage2;
    model[i].speed = BarstowModel.leftWheelMotor.speed; 
    
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
  fwrite(&model, sizeof(StateMotor_TypeDef), sizeModel, file);
  fclose(file);
  }
}

void MotorModel::load(const char * fileName){
  char filepath[150] = "../../res/model/";
  strcat(filepath, fileName);
  strcat(filepath, ".bin");

  FILE * file = fopen(filepath, "wb");
  
if (file){
  fread(&model, sizeof(StateMotor_TypeDef), sizeModel, file);
  fclose(file);
  }
}
