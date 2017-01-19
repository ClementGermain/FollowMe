#ifndef __MOTORMODEL__
#define __MOTORMODEL__

#include <string>
#include <cstdio>
#include "Car.hpp"
#include "../../stm32/KeilProject/Sources/Barstow/Model.h"
#include "../../stm32/KeilProject/Sources/Barstow/Control.h"

/*
enum type to choose the voltage number of the motor
*/
enum numVoltage {v1 = 1, v2 = 2};

/*
Structure that will be stock the motors model informations.
Every command is associated to a MotorModel_Typedef of a single motor with his voltage, current, (speed), ...
*/
typedef struct {
  float cmd;
  MotorModel_Typedef MotorModel;
} Model_TypeDef;

/*
Class that will handle the model of a single motor.

The model is stored into a Tab (==> Model)
Each element of the tab represent the association of a command and the motor's values corresponding
*/
class MotorModel{
  
public :

  /* 
  Destructor
  */
  ~MotorModel(void);

  /*
  Constructor
    @parma : sizeModel : number of point of the model ==> number of Model_Typedf element into the tab
  */
  MotorModel(int sizeModel = 1200);
  
  /*
  reset the model values to 0
  */
  void reset();

  /*
  Create the model by running a simulation and get motor's value
    @param : cmdStart : command value to start the simulation
    @param : cmdStop : command value to stop the simulation
    @param : waitTime : time to wait between each new command (in micro second)
    @param : N : number of iterations of the simulations cycle to get a mean Model
  */
  void create(float cmdStart, float CmdStop, float waitTime, int N=1);
  
  /* 
  Save the model into a binary file
    @param : filename : name of the file to store the data in "res/model/"
      => the file will be save in /res/model/filename.bin
  */
  void save(const char * filename);

  /* 
  Load a model from a binary file
    @param : filename : name of the binary file to load
      => the file must be like /res/model/filename.bin
  */
  void load(const char * filename);

  /* create a Txt file that store the model tab values
     @param : filename : name of the model binary file in which the data are come from
      => the new new txt file will have the same name as the binary file
      => the file will be save in /res/model/filename.txt
  */
  void exportTxt(const char * filename);  
  
  /* get the votage value of the model matching a given commandMotorBack 
    @param : cmd
    @param : numVoltage : number of the votltage wanted
  */
  float getVoltage(float cmd, numVoltage n);

  /* get the current value of the model matching a given commandMotorBack 
    @param : cmd
  */
  float getCurrent(float cmd);
  
protected :

  /*
    get the index of a Model element from the tab Model that match a given cmd
    @param : cmd
  */
  int getIndex(float cmd);

  /*
    send a command to propulsion motor
    @param : cmd
  */
  void writeCmd(float cmd);
		
	//Number of element of Model_Typedef into the tab
  int sizeModel;  

  // Tab of Model_Typedef that will store all the model informations for each command
  Model_TypeDef * Model;
};

#endif

