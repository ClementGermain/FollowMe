#ifndef __MOTORMODEL__
#define __MOTORMODEL__

#include <string>
#include <cstdio>
#include "Car.hpp"
#include "../../stm32/KeilProject/Sources/Barstow/Model.h"
#include "../../stm32/KeilProject/Sources/Barstow/Control.h"


// struct that will contain all the models motors informations
typedef struct {
  float cmd;
  MotorModel_Typedef MotorModel;
} Model_TypeDef;

class MotorModel {

public :
  ~MotorModel(void);
  MotorModel(int sizeModel = 1200);

  // Create the model by running a simulation and get motor's value
  // time in seconds
  void create(float cmdStart, float CmdStop, float waitTime);

  // save the model into a file
  void save(const char * FileName);

  // load the model
  void load(const char * FileName);

  void getState(float cmd, MotorModel_Typedef * MotorModel);

protected :
  int sizeModel;  
  Model_TypeDef * Model;
};

#endif
