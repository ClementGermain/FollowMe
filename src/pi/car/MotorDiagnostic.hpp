#ifndef __MOTORDIAGNOSTIC__
#define __MOTORDIAGNOSTIC__

#include <string>
#include <cstdio>
#include <thread>
#include "MotorModel.hpp"
#include "../../stm32/KeilProject/Sources/Barstow/Model.h"
#include "../../stm32/KeilProject/Sources/Barstow/Control.h"
#include "Car.hpp"

/*
enum type that type the different possible failure
*/
enum Failure_Typedef {NO, CMD, CURRENT, SPEED};

/*
Class that diagnostic a single motors.
It is based on the comparaison of real times values and a Motor Model
*/
class DiagnosticMotor{
  
public:
  /*
  Constructor
    @parma : filename : name of the binary model file to use for the diagnostic
      => the file must be like /res/model/filename.bin
    @parma : MotorType : Select a motor
    @parma : sizeModel : Size of the model
    @parma : delay : value of the number of cycle with failure to wait before being sure about the failure
  */
  DiagnosticMotor(Car::Motor MotorType_, const char * filename = "model_reel", int sizeModel = 1200, int delay_ = 10);
  
  /*
  Destructor
  */
  //~DiagnosticMotor();

  /*
  Start the thread
  */
  void start();

  /*
  Stop the thread
  */
  void stop();

  /*
  load a different model
    @parma : filename of the binary model to load
      => the file must be like /res/model/filename.bin
  */
  void changeModel(const char * filename);

  /*
  Compare the current motors values to the model and update the diagnostic variables
  */
  void compareModel();
  
  /*
  get the Failure_Typedef of the diagnostic
    @return : Failure_Typedef
  */
  Failure_Typedef getFailure();

  /* 
  get the min/max value of voltage/current of the diagnostic
  */
  float getMinVoltage(numVoltage n);
  float getMaxVoltage(numVoltage n);
  float getMinCurrent();
  float getMaxCurrent();

  /*
  reset the diagnostic
  */
  void reset();
  
  /*
  get the command value send to the motor
  */
  float getCmd();
  
  /*
  return True if the Diagnostic Instance detected a failure
  */
  bool isFailureDetected();
  
protected:
  
  /* 
  get the model value of current/voltage
  */
  float getValVoltage(numVoltage n);
  float getValCurrent();
  
  // Barstow struct that comunicate with the STM32 with SPI protocole
  BarstowControl_Typedef BarstowControl;
  BarstowModel_Typedef BarstowModel;

  /*
  delta of voltage/current.
  these delta will set the acceptable range of motors values
  */
  float delta_voltage; // mV
  float delta_current; // mA

  float ValVoltage[2]; // store the model voltage (v1 and v2) of the motor : mV
  float ValCurrent; // store the model current of the motor : mA

  int delay; // number of cycle with failure to wait before being sure about the failure
  int delay_compt; // dynamic compteur of the number of cycle with a failure

  // thread managment
  void run();
  bool endThread;
  std::thread * threadTest;

  MotorModel Model; // model
  Failure_Typedef failure;
  Car::Motor MotorType;
};

// create instance of DiagnosticMotor for both propulsion motors
extern DiagnosticMotor Diag_Prop_Right;
extern DiagnosticMotor Diag_Prop_Left;

#endif

