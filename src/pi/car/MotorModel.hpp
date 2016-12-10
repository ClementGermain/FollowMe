#ifndef __MOTORMODEL__
#define __MOTORMODEL__

#include <string>
#include <cstdio>
#include "Car.hpp"

// struct that will contain all the models motors informations
typedef struct {
  float cmd;
  uint32_t current;
  uint32_t voltage1;
  uint32_t voltage2;
  uint32_t speed;
} StateMotor_TypeDef;

class MotorModel {

public :
  MotorModel(void);
  ~MotorModel(void);
  MotorModel(int sizeModel);

  // Create the model by running a simulation and get motor's value
  // time in seconds
  void create(float cmdStart, float CmdStop, float waitTime);

  // save the model into a file
  void save(const char * FileName);

  // load the model
  void load(const char * FileName);

  StateMotor_TypeDef getState(uint32_t cmd);

protected :
  int sizeModel;
  StateMotor_TypeDef *  model;
};

#endif
