#include "Diagnostic.hpp"
#include <cmath>

using namespace std;

void Diagnostic::diagnostic(){
	BarstowControl_Typedef * BarstowControl = &(Car::controlStructure);
	BarstowModel_Typedef * BarstowModel = &(Car::modelStructure);
    bool firstLoop = True;
    if (firstLoop)
        init();
    update();
    
    if (abs(ModelRef.speed - BarstowModel->propulsionMotor.speed) > delta_voltage){
        failure = True;
        failureMotor = True;
    }
    else{
        failure = False;
        failureMotor = False;
    }        
}

void Diagnostic::init(){
    MotorModel ModelRef();
    ModelRef.load("model_propulsion");
}

void Diagnostic::update(){
    ModelRef.getState(BarstowControl->propulsionMotor.speed, &MotorModel)
}

thread * Diagnostic::threadTest = NULL;
bool Diagnostic::endThread = true;

// ------------ Thread management -------- //
void Diagnostic::start() {
	if(threadTest == NULL) {
		endThread = false;
		threadTest = new thread(Diagnostic::run);
	}
}

void Diagnostic::stop() {
	if(threadTest != NULL) {
		endThread = true;
		threadTest->join();
		delete threadTest;
		threadTest = NULL;
	}
}

voidDiagnostic::run() {
	while(!Diagnostic::endThread) {
        Diagnostic::diagnostic();
		// sleep 
		this_thread::sleep_for(chrono::milliseconds(100));
	}
}
// --------------------------------------- //
