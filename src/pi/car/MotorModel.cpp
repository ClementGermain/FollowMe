#include "MotorModel.hpp"
#include <cmath>
#include <unistd.h>
#include <iostream>
#include <fstream>
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
  
void MotorModel::create(int CmdStart, int CmdStop, float waitTime){
  
  BarstowModel_Typedef BarstowModel;
  int i=0;

  for (float cmd=CmdStart ; cmd<CmdStop ; cmd+=(CmdStop-CmdStart)/sizeModel){
    Car::writeControlMotor(Car::MoveForward, cmd);
    
    sleep(waitTime);

    Car::updateModelStructure(BarstowModel);
    model[i].current = cmd;
    model[i].current = BarstowModel.leftWheelMotor.current;
    model[i].current = BarstowModel.leftWheelMotor.voltage1;
    model[i].current = BarstowModel.leftWheelMotor.voltage2;
    model[i].current = BarstowModel.leftWheelMotor.speed; 
    
    i++;
  }  
}

void MotorModel::save(const std::string & fileName){
	/*
  ofstream file(fileName, ios::out | ios::trunc);
if (file){
  fwrite(&model, sizeof(StateMotor_TypeDef), sizeModel, file);
  file.close();
  }*/
}

void MotorModel::load(const std::string & fileName){ /*
  ifstream file(fileName); 
  if (file){
    fread(&model, sizeof(StateMotor_TypeDef), sizeModel, file);
    file.close();
  }*/
}
