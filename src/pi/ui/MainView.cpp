#include "MainView.hpp"
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_framerate.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <cmath>
#include <thread>
#include <iostream>
#include <map>
#include <memory>
#include "car/Car.hpp"
#include "utils/Log.hpp"
#include "IA/IA.hpp"
#include "car/Camera.hpp"
#include "car/Obstacle.hpp"
#include "car/MotorModel.hpp"
#include "car/MotorDiagnostic.hpp"
#include "view/KeyboardInput.hpp"
#include "view/trackbar/Trackbar_Horizontal.hpp"
#include "view/trackbar/Trackbar_Vertical.hpp"
#include "view/Digital.hpp"
#include "view/ToggleBox.hpp"
#include "view/StateBox.hpp"
#include "view/LogView.hpp"
#include "view/ImageView.hpp"
#include "view/TextView.hpp"
#include "view/EmptyBoxView.hpp"
#include "view/PlotsView.hpp"
#include "view/PointerView.hpp"
#include "improc/UserDetectionThread.hpp"
#include "improc/RoadDetectionThread.hpp"
#include "improc/RoadDetection.hpp"

using namespace std;

MainView::MainView() : threadView(NULL), isThreadTerminated(true) {

}

void commandMotorFront(int direction) {
	float speed = 0.5f;
	switch(direction) {
		case KeyboardInput::Idle:
			Car::writeControlMotor(Car::NoTurn, 0); 
			break;
		case KeyboardInput::TurnLeft:
			Car::writeControlMotor(Car::TurnLeft, 30); 
			break;
		case KeyboardInput::TurnRight:
			Car::writeControlMotor(Car::TurnRight, -30); 
			break;
	}
	LogD << "Keyboard: change direction to " << (direction*speed) <<endl;
}

void commandMotorBack(int direction) {
	float speed = 0.5f;
	float fastspeed = 1.0f;
	switch(direction) {
		case KeyboardInput::Idle:
			Car::writeControlMotor(Car::Stop, speed); 
			break;
		case KeyboardInput::GoFastForward:
			speed = fastspeed;
		case KeyboardInput::GoForward:
			Car::writeControlMotor(Car::MoveForward, speed); 
			break;
		case KeyboardInput::GoFastBackward:
			speed = fastspeed;
		case KeyboardInput::GoBackward:
			Car::writeControlMotor(Car::MoveBackward, speed); 
			break;
	}
	LogD << "Keyboard: change propulsion to " << (direction*speed) <<endl;
}

void MainView::initializeViews(ViewManager & mgr) {
	//// PAGE 1 : ALL (DEFAULT VIEW) ////
	Layout & page1Layout = mgr.createLayout("All");	

	//image with target
	page1Layout.addView("target", new ImageView(0,0,290,240));
	page1Layout.addView("filtered_road", new ImageView(290,0,290,240));

	// keyboard
	page1Layout.addView("keyboard", new KeyboardInput(commandMotorFront, commandMotorBack, 0, 240, 290, 160));

	//prompt + CPU
	page1Layout.addView("logs", new LogView(580,0,220,400));
	page1Layout.addView("cpu", new Digital("CPU: %.0f%%", 650, 5, 80, 16, false));

	//user detection toggle
	page1Layout.addView("toggle_user", new ToggleBox("USER DETECTED", "NO USER", 300, 245));

	//road detection toggle
	page1Layout.addView("state_road", new StateBox(300, 285));
	page1Layout.getStateBoxView("state_road").add_state("ROAD : NO IDEA", 150, 0, 0); //NO ROAD DETECTED
	page1Layout.getStateBoxView("state_road").add_state("ROAD : YES", 0, 150, 0); //ROAD DETECTED
	page1Layout.getStateBoxView("state_road").add_state("ROAD : LIKELY", 255, 153, 0); //ROAD UNCERTAIN
	page1Layout.getStateBoxView("state_road").add_state("ROAD : UNLIKELY", 255, 153, 0); //ROAD UNCERTAIN
	page1Layout.getStateBoxView("state_road").add_state("ROAD : NO", 150, 0, 0);

	//motor_left toggle
	page1Layout.addView("state_motor_left", new StateBox(300, 325));
	page1Layout.getStateBoxView("state_motor_left").add_state("MOTOR LEFT OK", 0, 150, 0);
	page1Layout.getStateBoxView("state_motor_left").add_state("CMD LEFT FAILURE", 150, 0, 0);
	page1Layout.getStateBoxView("state_motor_left").add_state("MOTOR LEFT FAILURE", 150, 0, 0);

	//motor_right toggle
	page1Layout.addView("state_motor_right", new StateBox(300, 365));
	page1Layout.getStateBoxView("state_motor_right").add_state("MOTOR RIGHT OK", 0, 150, 0);
	page1Layout.getStateBoxView("state_motor_right").add_state("CMD RIGHT FAILURE", 150, 0, 0);
	page1Layout.getStateBoxView("state_motor_right").add_state("MOTOR RIGHT FAILURE", 150, 0, 0);


	//// PAGE 2 : MOTORS ////
	Layout & page2Layout = mgr.createLayout("Motors");	
	
	// keyboard
	page2Layout.addView("keyboard", new KeyboardInput(commandMotorFront, commandMotorBack, 0, 240, 290, 160));

	//prompt + CPU
	page2Layout.addView("logs", new LogView(580,0,220,400));
	page2Layout.addView("cpu", new Digital("CPU: %.0f%%", 650, 5, 80, 16, false));

	// Car top view
	page2Layout.addView("imgCar", new ImageView(330, 25, 200, 350));
	SDL_Surface * car = SDL_LoadBMP("../../res/img/car_top_view.bmp");
	page2Layout.getImageView("imgCar").setImage(car);
	SDL_FreeSurface(car);

	// motors digital infos 
	page2Layout.addView("dCmdFront", new Digital("Cmd: %02.0f/100", 380, 55, 100, 16, false));
      	page2Layout.addView("dCurrentFront", new Digital("Cur: %04.0f mA", 380, 85, 100, 16, false));
	page2Layout.addView("dVoltage1Front", new Digital("U1 : %1.2f V", 380, 115, 100, 16, false));
	page2Layout.addView("dVoltage2Front", new Digital("U2 : %1.2f V", 380, 145, 100, 16, false));

	page2Layout.addView("dCmdLeft", new Digital("Cmd: %02.0f/100", 330, 225, 100, 16, false));
	page2Layout.addView("dCurrentLeft", new Digital("Cur: %04.0f mA", 330, 255, 100, 16, false));
	page2Layout.addView("dVoltage1Left", new Digital("U1 : %1.2f V", 330, 285, 100, 16, false));
	page2Layout.addView("dVoltage2Left", new Digital("U2 : %1.2f V", 330,315, 100, 16, false));

	page2Layout.addView("dCmdRight", new Digital("Cmd: %02.0f/100", 435, 225, 100, 16, false));
	page2Layout.addView("dCurrentRight", new Digital("Cur: %04.0f mA", 435, 255, 100, 16, false));
	page2Layout.addView("dVoltage1Right", new Digital("U1 : %1.2f V", 435, 285, 100, 16, false));
	page2Layout.addView("dVoltage2Right", new Digital("U2 : %1.2f V", 435,315, 100, 16, false));
	
	// motors trackbar
	page2Layout.addView("tbCmdFront", new Trackbar_Horizontal(-1, 1, 385, 70, 90, 10, CENTREE));
	page2Layout.addView("tbCurrentFront", new Trackbar_Horizontal(0, 1500, 385, 100, 90, 10));
	page2Layout.addView("tbVoltage1Front", new Trackbar_Horizontal(0, 8000, 385, 130, 90, 10));
	page2Layout.addView("tbVoltage2Front", new Trackbar_Horizontal(0, 8000, 385, 160, 90, 10));

	page2Layout.addView("tbCmdLeft", new Trackbar_Horizontal(-1, 1, 333, 240, 90, 10, CENTREE));
	page2Layout.addView("tbCurrentLeft", new Trackbar_Horizontal(0, 1500, 333, 270, 90, 10));
	page2Layout.addView("tbVoltage1Left", new Trackbar_Horizontal(0, 8000, 333, 300, 90, 10));
	page2Layout.addView("tbVoltage2Left", new Trackbar_Horizontal(0, 8000, 333, 330, 90, 10));

	page2Layout.addView("tbCmdRight", new Trackbar_Horizontal(-1, 1, 438, 240, 90, 10, CENTREE));
	page2Layout.addView("tbCurrentRight", new Trackbar_Horizontal(0, 1500, 438, 270, 90, 10));
	page2Layout.addView("tbVoltage1Right", new Trackbar_Horizontal(0, 8000, 438, 300, 90, 10));
	page2Layout.addView("tbVoltage2Right", new Trackbar_Horizontal(0, 8000, 438, 330, 90, 10));

	//motor_left toggle
	page2Layout.addView("simple_motor_left", new StateBox(325, 350, 100));
	page2Layout.getStateBoxView("simple_motor_left").add_state("OK", 0, 150, 0);
	page2Layout.getStateBoxView("simple_motor_left").add_state("COMMAND", 150, 0, 0);
	page2Layout.getStateBoxView("simple_motor_left").add_state("FAILURE", 150, 0, 0);

	//motor_right toggle
	page2Layout.addView("simple_motor_right", new StateBox(435, 350, 100));
	page2Layout.getStateBoxView("simple_motor_right").add_state("OK", 0, 150, 0);
	page2Layout.getStateBoxView("simple_motor_right").add_state("COMMAND", 150, 0, 0);
	page2Layout.getStateBoxView("simple_motor_right").add_state("FAILURE", 150, 0, 0);


	//// PAGE 3 : USER ////
	Layout & page3Layout = mgr.createLayout("User");	

	page3Layout.addView("target_camera", new ImageView(280,0,320,200));
	page3Layout.addView("white_target", new ImageView(0,200,320,200));

	// position user trackbar
	page3Layout.addView("sensor_UserDistance", new Trackbar_Vertical(0, 3, 125, 85, 10, 100, INVERSE));
	page3Layout.addView("sensor_UserAngle", new Trackbar_Horizontal(-30, 30, 45, 7, 170, 11, CENTREE));
	// position user text
	page3Layout.addView("sensor_UserDistanceText", new Digital("%.2fm", 65, 85, 60));
	page3Layout.addView("sensor_UserAngleText", new Digital("%+3.0fdeg", 45, 25, 60));


	//// DEFAULT VIEW ////
	Layout & defaultLayout = mgr.createLayout("Motor");

	// Car top view
/*	defaultLayout.addView("imgCar", new ImageView(330, 25, 200, 350));
	SDL_Surface * car = SDL_LoadBMP("../../res/img/car_top_view.bmp");
	defaultLayout.getImageView("imgCar").setImage(car);
	SDL_FreeSurface(car);
*/
	// raspi info
	defaultLayout.addView("boxRaspi", new EmptyBoxView(535, 10, 260, 50));
	defaultLayout.addView("titleRaspi", new TextView("Raspberry Pi 3", 540, 20, 250, 16, true));
	defaultLayout.addView("cpu", new Digital("CPU: %.0f%%", 540, 40, 80, 16, false));

	// Toggle Informations
	
	//motor_left toggle
	defaultLayout.addView("state_motor_left", new StateBox(535, 70));
	defaultLayout.getStateBoxView("state_motor_left").add_state("MOTOR LEFT OK", 0, 150, 0);
	defaultLayout.getStateBoxView("state_motor_left").add_state("CMD LEFT FAILURE", 150, 0, 0);
	defaultLayout.getStateBoxView("state_motor_left").add_state("MOTOR LEFT FAILURE", 150, 0, 0);

	//motor_right toggle
	defaultLayout.addView("state_motor_right", new StateBox(535, 110));
	defaultLayout.getStateBoxView("state_motor_right").add_state("MOTOR RIGHT OK", 0, 150, 0);
	defaultLayout.getStateBoxView("state_motor_right").add_state("CMD RIGHT FAILURE", 150, 0, 0);
	defaultLayout.getStateBoxView("state_motor_right").add_state("MOTOR RIGHT FAILURE", 150, 0, 0);
	
	//user detection toggle
	defaultLayout.addView("toggle_user", new ToggleBox("USER DETECTED", "NO USER", 535, 150));

	//road detection toggle
	defaultLayout.addView("state_road", new StateBox(535, 190));
	defaultLayout.getStateBoxView("state_road").add_state("ROAD : NO IDEA", 150, 0, 0); //NO ROAD DETECTED
	defaultLayout.getStateBoxView("state_road").add_state("ROAD : YES", 0, 150, 0); //ROAD DETECTED
	defaultLayout.getStateBoxView("state_road").add_state("ROAD : LIKELY", 255, 153, 0); //ROAD UNCERTAIN
	defaultLayout.getStateBoxView("state_road").add_state("ROAD : UNLIKELY", 255, 153, 0); //ROAD UNCERTAIN
	defaultLayout.getStateBoxView("state_road").add_state("ROAD : NO", 150, 0, 0);

	// motors digital infos 

	defaultLayout.addView("dCmdFront", new Digital("Cmd: %02.0f/100", 380, 55, 100, 16, false));
      	defaultLayout.addView("dCurrentFront", new Digital("Cur: %04.0f mA", 380, 85, 100, 16, false));
	defaultLayout.addView("dVoltage1Front", new Digital("U1 : %1.2f V", 380, 115, 100, 16, false));
	defaultLayout.addView("dVoltage2Front", new Digital("U2 : %1.2f V", 380, 145, 100, 16, false));

	defaultLayout.addView("dCmdLeft", new Digital("Cmd: %02.0f/100", 330, 225, 100, 16, false));
	defaultLayout.addView("dCurrentLeft", new Digital("Cur: %04.0f mA", 330, 255, 100, 16, false));
	defaultLayout.addView("dVoltage1Left", new Digital("U1 : %1.2f V", 330, 285, 100, 16, false));
	defaultLayout.addView("dVoltage2Left", new Digital("U2 : %1.2f V", 330,315, 100, 16, false));

	defaultLayout.addView("dCmdRight", new Digital("Cmd: %02.0f/100", 435, 225, 100, 16, false));
	defaultLayout.addView("dCurrentRight", new Digital("Cur: %04.0f mA", 435, 255, 100, 16, false));
	defaultLayout.addView("dVoltage1Right", new Digital("U1 : %1.2f V", 435, 285, 100, 16, false));
	defaultLayout.addView("dVoltage2Right", new Digital("U2 : %1.2f V", 435,315, 100, 16, false));
	
	// motors trackbar

	defaultLayout.addView("tbCmdFront", new Trackbar_Horizontal(-1, 1, 385, 70, 90, 10, CENTREE));
	defaultLayout.addView("tbCurrentFront", new Trackbar_Horizontal(0, 1500, 385, 100, 90, 10));
	defaultLayout.addView("tbVoltage1Front", new Trackbar_Horizontal(0, 8000, 385, 130, 90, 10));
	defaultLayout.addView("tbVoltage2Front", new Trackbar_Horizontal(0, 8000, 385, 160, 90, 10));

	defaultLayout.addView("tbCmdLeft", new Trackbar_Horizontal(-1, 1, 333, 240, 90, 10, CENTREE));
	defaultLayout.addView("tbCurrentLeft", new Trackbar_Horizontal(0, 1500, 333, 270, 90, 10));
	defaultLayout.addView("tbVoltage1Left", new Trackbar_Horizontal(0, 8000, 333, 300, 90, 10));
	defaultLayout.addView("tbVoltage2Left", new Trackbar_Horizontal(0, 8000, 333, 330, 90, 10));

	defaultLayout.addView("tbCmdRight", new Trackbar_Horizontal(-1, 1, 438, 240, 90, 10, CENTREE));
	defaultLayout.addView("tbCurrentRight", new Trackbar_Horizontal(0, 1500, 438, 270, 90, 10));
	defaultLayout.addView("tbVoltage1Right", new Trackbar_Horizontal(0, 8000, 438, 300, 90, 10));
	defaultLayout.addView("tbVoltage2Right", new Trackbar_Horizontal(0, 8000, 438, 330, 90, 10));

	// other
	defaultLayout.addView("keyboard", new KeyboardInput(commandMotorFront, commandMotorBack, 0, 240, 320, 160));
	defaultLayout.addView("camera", new ImageView(0, 0, 320, 240));



	//// SENSOR FULLSCREEN
	Layout & sensorLayout = mgr.createLayout("Sensor");

	// Car top view
	sensorLayout.addView("sensor_imgCar", new ImageView(330, 25, 200, 375));
	SDL_Surface * car_view_sensor = SDL_LoadBMP("../../res/img/car_top_view_capteurs.bmp");
	sensorLayout.getImageView("sensor_imgCar").setImage(car_view_sensor);
	SDL_FreeSurface(car_view_sensor);

	// raspi info
	sensorLayout.addView("sensor_boxRaspi", new EmptyBoxView(535, 10, 260, 50));
	sensorLayout.addView("sensor_titleRaspi", new TextView("Raspberry Pi 3", 540, 20, 250, 16, true));
	sensorLayout.addView("sensor_cpu", new Digital("CPU: %.0f%%", 540, 40, 80, 16, false));

	// Toggle Informations
	sensorLayout.addView("sensor_toggle_motor", new ToggleBox("MOTOR OK", "MOTOR FAILURE", 535, 70));

	//motor_left toggle
	sensorLayout.addView("sensor_motor_left", new StateBox(535, 70));
	sensorLayout.getStateBoxView("sensor_motor_left").add_state("MOTOR LEFT OK", 0, 150, 0);
	sensorLayout.getStateBoxView("sensor_motor_left").add_state("CMD LEFT FAILURE", 150, 0, 0);
	sensorLayout.getStateBoxView("sensor_motor_left").add_state("MOTOR LEFT FAILURE", 150, 0, 0);

	//motor_right toggle
	sensorLayout.addView("sensor_motor_right", new StateBox(535, 110));
	sensorLayout.getStateBoxView("sensor_motor_right").add_state("MOTOR RIGHT OK", 0, 150, 0);
	sensorLayout.getStateBoxView("sensor_motor_right").add_state("CMD RIGHT FAILURE", 150, 0, 0);
	sensorLayout.getStateBoxView("sensor_motor_right").add_state("MOTOR RIGHT FAILURE", 150, 0, 0);

	//user detection toggle
	sensorLayout.addView("sensor_toggle_user", new ToggleBox("USER DETECTED", "NO USER", 535, 150));
	
	//sensorLayout.addView("sensor_toggle_obstacle", new ToggleBox("NO OBSTACLES", "OBSTACLE DETECTED", 535, 150));

	//road detection toggle	
	sensorLayout.addView("sensor_state_road", new StateBox(535, 190));
	sensorLayout.getStateBoxView("sensor_state_road").add_state("ROAD : NO IDEA", 150, 0, 0); //NO ROAD DETECTED
	sensorLayout.getStateBoxView("sensor_state_road").add_state("ROAD : YES", 0, 150, 0); //ROAD DETECTED
	sensorLayout.getStateBoxView("sensor_state_road").add_state("ROAD : LIKELY", 255, 153, 0); //ROAD UNCERTAIN
	sensorLayout.getStateBoxView("sensor_state_road").add_state("ROAD : UNLIKELY", 255, 153, 0); //ROAD UNCERTAIN
	sensorLayout.getStateBoxView("sensor_state_road").add_state("ROAD : NO", 150, 0, 0);
	//sensorLayout.addView("sensor_toggle_eoc_left", new ToggleBox("NO EOC LEFT", "EOC LEFT", 535, 230));
	//sensorLayout.addView("sensor_toggle_eoc_right", new ToggleBox("NO EOC RIGHT", "EOC RIGHT", 535, 270));

	// distance Usound trackbar
	sensorLayout.addView("sensor_USCenter", new Trackbar_Vertical(0, 500, 426, 240, 10, 130, INVERSE));
	sensorLayout.addView("sensor_USLeft", new Trackbar_Vertical(0, 500, 369, 255, 10, 130, INVERSE));
	sensorLayout.addView("sensor_USRight", new Trackbar_Vertical(0, 500, 483, 255, 10, 130, INVERSE));
	
	// position user trackbar
	sensorLayout.addView("sensor_UserDistance", new Trackbar_Vertical(0, 3, 425, 85, 10, 100, INVERSE));
	sensorLayout.addView("sensor_UserAngle", new Trackbar_Horizontal(-30, 30, 345, 7, 170, 11, CENTREE));
	// position user text
	sensorLayout.addView("sensor_UserDistanceText", new Digital("%.2fm", 365, 85, 60));
	sensorLayout.addView("sensor_UserAngleText", new Digital("%+3.0fdeg", 345, 25, 60));

	// distance Usound text
	sensorLayout.addView("sensor_distFrontLeft", new Digital("%.0fcm", 330, 210, 65));
	sensorLayout.addView("sensor_distFrontCenter", new Digital("%.0fcm", 397, 195, 66));
	sensorLayout.addView("sensor_distFrontRight", new Digital("%.0fcm", 465, 210, 65));

	// other
	sensorLayout.addView("keyboard", new KeyboardInput(commandMotorFront, commandMotorBack, 0, 240, 320, 160));
	sensorLayout.addView("camera", new ImageView(0, 0, 320, 240));
	
	//// LOG FULLSCREEN ////
	Layout & logLayout = mgr.createLayout("Logs");

	logLayout.addView("logs", new LogView(0,0,800,400));


	//// USER DETECTION ////
	Layout & userLayout = mgr.createLayout("User Detection");

	userLayout.addView("filter", new ImageView(0,0,400,240));
	userLayout.addView("result", new ImageView(400,0,400,240));
	userLayout.addView("logs", new LogView(0,240,400,160));
	userLayout.addView("graph", new PlotsView(400,240,400,160));

	//// ROAD IMPROC ////
	Layout & roadImprocLayout = mgr.createLayout("Road Detection");
	roadImprocLayout.addView("roadimage", new ImageView(40, 80, 320, 240));
	roadImprocLayout.addView("roadcamera", new ImageView(440, 80, 320, 240));
}

void MainView::updateViews(ViewManager & mgr) {
	BarstowModel_Typedef model;
	BarstowControl_Typedef control;

	Car::getControlStructure(control);
	Car::getModelStructure(model);

	if(mgr.isActive("All")) {
		Layout & l = mgr.getLayout("All");
		l.getImageView("target").setImage(&userDetectionThread.detector.getResultImage());
		l.getImageView("filtered_road").setImage(&roadDetectionThread.detector.getImage(), ImageView::FITXY);
		l.getToggleBoxView("toggle_user").toggle(userDetectionThread.detector.isDetected());
		l.getStateBoxView("state_road").set_state(roadDetectionThread.detector.canGoForward());
		l.getStateBoxView("state_motor_left").set_state(Diag_Prop_Left.getFailure());
		l.getStateBoxView("state_motor_right").set_state(Diag_Prop_Right.getFailure());
		l.getDigitalView("cpu").setValue(cpuLoad.get());
	}

	if(mgr.isActive("Motors")) {
		Layout & l = mgr.getLayout("Motors");

		l.getDigitalView("dCmdFront").setValue( control.directionMotor.speed * control.directionMotor.direction * 100.0);
		l.getTrackbarView("tbCmdFront").setPosition( control.directionMotor.speed * control.directionMotor.direction );
		l.getDigitalView("dVoltage1Front").setValue(((float) model.directionMotor.voltage1)/1000.0);
		l.getTrackbarView("tbVoltage1Front").setPosition(model.directionMotor.voltage1);
		l.getDigitalView("dVoltage2Front").setValue(((float) model.directionMotor.voltage2)/1000.0);
		l.getTrackbarView("tbVoltage2Front").setPosition(model.directionMotor.voltage2);
		l.getDigitalView("dCurrentFront").setValue(((float) model.directionMotor.current));
		l.getTrackbarView("tbCurrentFront").setPosition(model.directionMotor.current); 
		
		l.getDigitalView("dCmdLeft").setValue( control.propulsionMotor.speed * control.propulsionMotor.direction * 100.0);
		l.getTrackbarView("tbCmdLeft").setPosition( control.propulsionMotor.speed * control.propulsionMotor.direction );
		l.getDigitalView("dVoltage1Left").setValue(((float) model.leftWheelMotor.voltage1)/1000.0);
		l.getTrackbarView("tbVoltage1Left").setPosition(model.leftWheelMotor.voltage1);
		l.getDigitalView("dVoltage2Left").setValue(((float) model.leftWheelMotor.voltage2)/1000.0);
		l.getTrackbarView("tbVoltage2Left").setPosition(model.leftWheelMotor.voltage2);
		l.getDigitalView("dCurrentLeft").setValue(((float) model.leftWheelMotor.current));
		l.getTrackbarView("tbCurrentLeft").setPosition(model.leftWheelMotor.current);

		l.getTrackbarView("tbVoltage1Left").setInnerBounds(Diag_Prop_Left.getMinVoltage(v1), Diag_Prop_Left.getMaxVoltage(v1)); // add inner bounds
		l.getTrackbarView("tbVoltage2Left").setInnerBounds(Diag_Prop_Left.getMinVoltage(v2), Diag_Prop_Left.getMaxVoltage(v2)); // add inner bounds
		l.getTrackbarView("tbCurrentLeft").setInnerBounds(Diag_Prop_Left.getMinCurrent(), Diag_Prop_Left.getMaxCurrent()); // add inner bounds
		l.getTrackbarView("tbVoltage1Right").setInnerBounds(Diag_Prop_Right.getMinVoltage(v1), Diag_Prop_Right.getMaxVoltage(v1)); // add inner bounds
		l.getTrackbarView("tbVoltage2Right").setInnerBounds(Diag_Prop_Right.getMinVoltage(v2), Diag_Prop_Right.getMaxVoltage(v2)); // add inner bounds
		l.getTrackbarView("tbCurrentRight").setInnerBounds(Diag_Prop_Right.getMinCurrent(), Diag_Prop_Right.getMaxCurrent()); // add inner bounds
		
		l.getDigitalView("dCmdRight").setValue( control.propulsionMotor.speed * control.propulsionMotor.direction * 100.0);
		l.getTrackbarView("tbCmdRight").setPosition( control.propulsionMotor.speed * control.propulsionMotor.direction );
	    	l.getDigitalView("dVoltage1Right").setValue(((float) model.rightWheelMotor.voltage1)/1000.0);
		l.getTrackbarView("tbVoltage1Right").setPosition(model.rightWheelMotor.voltage1);
		l.getDigitalView("dVoltage2Right").setValue(((float) model.rightWheelMotor.voltage2)/1000.0);
		l.getTrackbarView("tbVoltage2Right").setPosition(model.rightWheelMotor.voltage2);
		l.getDigitalView("dCurrentRight").setValue(((float) model.rightWheelMotor.current));
		l.getTrackbarView("tbCurrentRight").setPosition(model.rightWheelMotor.current);

		l.getStateBoxView("simple_motor_left").set_state(Diag_Prop_Left.getFailure());
		l.getStateBoxView("simple_motor_right").set_state(Diag_Prop_Right.getFailure());	

	}	

	if(mgr.isActive("User")) {
		Layout & l = mgr.getLayout("User");
		l.getImageView("target_camera").setImage(&userDetectionThread.detector.getResultImage());
		l.getImageView("white_target").setImage(&userDetectionThread.detector.getFilterImage());
		l.getTrackbarView("sensor_UserDistance").setPosition(userDetectionThread.detector.getDistance()-Car::CarSize);
		l.getTrackbarView("sensor_UserAngle").setPosition(userDetectionThread.detector.getDirection()*180/M_PI);
		l.getDigitalView("sensor_UserDistanceText").setValue(userDetectionThread.detector.getDistance()-Car::CarSize);
		l.getDigitalView("sensor_UserAngleText").setValue(userDetectionThread.detector.getDirection()*180/M_PI);

	}
	
	if(mgr.isActive("Motor")) {
		Layout & l = mgr.getLayout("Motor");
      		
		l.getDigitalView("dCmdFront").setValue( control.directionMotor.speed * control.directionMotor.direction * 100.0);
		l.getTrackbarView("tbCmdFront").setPosition( control.directionMotor.speed * control.directionMotor.direction );
		l.getDigitalView("dVoltage1Front").setValue(((float) model.directionMotor.voltage1)/1000.0);
		l.getTrackbarView("tbVoltage1Front").setPosition(model.directionMotor.voltage1);
		l.getDigitalView("dVoltage2Front").setValue(((float) model.directionMotor.voltage2)/1000.0);
		l.getTrackbarView("tbVoltage2Front").setPosition(model.directionMotor.voltage2);
		l.getDigitalView("dCurrentFront").setValue(((float) model.directionMotor.current));
		l.getTrackbarView("tbCurrentFront").setPosition(model.directionMotor.current); 
		
		l.getDigitalView("dCmdLeft").setValue( control.propulsionMotor.speed * control.propulsionMotor.direction * 100.0);
		l.getTrackbarView("tbCmdLeft").setPosition( control.propulsionMotor.speed * control.propulsionMotor.direction );
		l.getDigitalView("dVoltage1Left").setValue(((float) model.leftWheelMotor.voltage1)/1000.0);
		l.getTrackbarView("tbVoltage1Left").setPosition(model.leftWheelMotor.voltage1);
		l.getDigitalView("dVoltage2Left").setValue(((float) model.leftWheelMotor.voltage2)/1000.0);
		l.getTrackbarView("tbVoltage2Left").setPosition(model.leftWheelMotor.voltage2);
		l.getDigitalView("dCurrentLeft").setValue(((float) model.leftWheelMotor.current));
		l.getTrackbarView("tbCurrentLeft").setPosition(model.leftWheelMotor.current);

		l.getTrackbarView("tbVoltage1Left").setInnerBounds(Diag_Prop_Left.getMinVoltage(v1), Diag_Prop_Left.getMaxVoltage(v1)); // add inner bounds
		l.getTrackbarView("tbVoltage2Left").setInnerBounds(Diag_Prop_Left.getMinVoltage(v2), Diag_Prop_Left.getMaxVoltage(v2)); // add inner bounds
		l.getTrackbarView("tbCurrentLeft").setInnerBounds(Diag_Prop_Left.getMinCurrent(), Diag_Prop_Left.getMaxCurrent()); // add inner bounds
		l.getTrackbarView("tbVoltage1Right").setInnerBounds(Diag_Prop_Right.getMinVoltage(v1), Diag_Prop_Right.getMaxVoltage(v1)); // add inner bounds
		l.getTrackbarView("tbVoltage2Right").setInnerBounds(Diag_Prop_Right.getMinVoltage(v2), Diag_Prop_Right.getMaxVoltage(v2)); // add inner bounds
		l.getTrackbarView("tbCurrentRight").setInnerBounds(Diag_Prop_Right.getMinCurrent(), Diag_Prop_Right.getMaxCurrent()); // add inner bounds
		
		l.getDigitalView("dCmdRight").setValue( control.propulsionMotor.speed * control.propulsionMotor.direction * 100.0);
		l.getTrackbarView("tbCmdRight").setPosition( control.propulsionMotor.speed * control.propulsionMotor.direction );
	    	l.getDigitalView("dVoltage1Right").setValue(((float) model.rightWheelMotor.voltage1)/1000.0);
		l.getTrackbarView("tbVoltage1Right").setPosition(model.rightWheelMotor.voltage1);
		l.getDigitalView("dVoltage2Right").setValue(((float) model.rightWheelMotor.voltage2)/1000.0);
		l.getTrackbarView("tbVoltage2Right").setPosition(model.rightWheelMotor.voltage2);
		l.getDigitalView("dCurrentRight").setValue(((float) model.rightWheelMotor.current));
		l.getTrackbarView("tbCurrentRight").setPosition(model.rightWheelMotor.current);
		
		l.getDigitalView("cpu").setValue(cpuLoad.get());
		
		l.getStateBoxView("state_motor_left").set_state(Diag_Prop_Left.getFailure());
		l.getStateBoxView("state_motor_right").set_state(Diag_Prop_Right.getFailure());

		l.getToggleBoxView("toggle_user").toggle(userDetectionThread.detector.isDetected());
		l.getStateBoxView("state_road").set_state(roadDetectionThread.detector.canGoForward());

		cv::Mat cam;
		Camera::getImage(cam);
		l.getImageView("camera").setImage(&cam, ImageView::NORMAL);
	}

	else if(mgr.isActive("Sensor")) {
		Layout & l = mgr.getLayout("Sensor");
		l.getTrackbarView("sensor_USLeft").setPosition(model.frontLeftUSensor.distance);
		l.getTrackbarView("sensor_USRight").setPosition(model.frontRightUSensor.distance);
		l.getTrackbarView("sensor_USCenter").setPosition(model.frontCenterUSensor.distance);
		
		l.getDigitalView("sensor_distFrontLeft").setValue(model.frontLeftUSensor.distance);
		l.getDigitalView("sensor_distFrontRight").setValue(model.frontRightUSensor.distance);
		l.getDigitalView("sensor_distFrontCenter").setValue(model.frontCenterUSensor.distance);
		
		l.getTrackbarView("sensor_UserDistance").setPosition(userDetectionThread.detector.getDistance()-Car::CarSize);
		l.getTrackbarView("sensor_UserAngle").setPosition(userDetectionThread.detector.getDirection()*180/M_PI);
		l.getDigitalView("sensor_UserDistanceText").setValue(userDetectionThread.detector.getDistance()-Car::CarSize);
		l.getDigitalView("sensor_UserAngleText").setValue(userDetectionThread.detector.getDirection()*180/M_PI);

		l.getStateBoxView("sensor_motor_left").set_state(Diag_Prop_Left.getFailure());
		l.getStateBoxView("sensor_motor_right").set_state(Diag_Prop_Right.getFailure());

		l.getToggleBoxView("sensor_toggle_user").toggle(userDetectionThread.detector.isDetected() && userDetectionThread.detector.getDistance() < 3.0f);
		//l.getToggleBoxView("sensor_toggle_obstacle").toggle(!ObstacleDetection::isGlobalDetected());
		l.getStateBoxView("sensor_state_road").set_state(roadDetectionThread.detector.canGoForward());
		//l.getToggleBoxView("sensor_toggle_eoc_left").toggle(model.leftEocSensor.endOfCourse);
		//l.getToggleBoxView("sensor_toggle_eoc_right").toggle(model.rightEocSensor.endOfCourse);

		l.getDigitalView("sensor_cpu").setValue(cpuLoad.get());

		cv::Mat cam;
		Camera::getImage(cam);
		l.getImageView("camera").setImage(&cam, ImageView::NORMAL);
	}
	else if(mgr.isActive("User Detection")) {
		Layout & l = mgr.getLayout("User Detection");
		l.getImageView("result").setImage(&userDetectionThread.detector.getResultImage());
		l.getImageView("filter").setImage(&userDetectionThread.detector.getFilterImage());
		((PlotsView*)l.getView("graph"))->addPlot(userDetectionThread.detector.getDistance());
	}
	else if(mgr.isActive("Road Detection")) {
		Layout & l = mgr.getLayout("Road Detection");
		l.getImageView("roadimage").setImage(&roadDetectionThread.detector.getImage(), ImageView::FITXY);
		cv::Mat cam;
		Camera::getImage(cam);
		//l.getImageView("roadcamera").setImage(&cam, ImageView::NORMAL);
		l.getImageView("roadcamera").setImage(&roadDetectionThread.detector.getCameraImage(), ImageView::FITXY);	
	}
}

void MainView::run() {
	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		return;
	// Open a window
	if(!(screen = SDL_SetVideoMode(800, 400, 32, SDL_SWSURFACE)))
		return;
	
	// Manage sleep duration between two updates
	FPSmanager fpsManager;
	SDL_initFramerate(&fpsManager);

	// Create the views
	ViewManager mgr;
	initializeViews(mgr);

	bool end = false;

	while(!end && !isThreadTerminated)  {
		/// Handle event queue
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			// Trasmit event to keyboard if necessary
			if(mgr.isActive("Motor") || mgr.isActive("Sensor") || mgr.isActive("All") || mgr.isActive("Motors"))
			if(mgr.getActiveLayout().getKeyboardView("keyboard").handleEvent(event))
				continue;

			switch(event.type) {
				case SDL_QUIT:
					// Close window with cross in title bar
					end = true;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_ESCAPE:
							// Close window with 'ESCAPE'
							end = true;
							break;
						case SDLK_F4:
							// Close window with 'ALT+F4'
							if((SDL_GetModState() & KMOD_LALT) == KMOD_LALT)
								end = true;
							break;
						case SDLK_q:
							// Close window with 'CTRL+Q'
							if((SDL_GetModState() & KMOD_LCTRL) == KMOD_LCTRL)
								end = true;
							break;
						case SDLK_TAB:
							// Switch view with 'TABULATION' or 'CTRL-TABULATION'
							if((SDL_GetModState() & KMOD_LCTRL) == KMOD_LCTRL)
								mgr.switchToPrevLayout();
							else
								mgr.switchToNextLayout();
							break;
						default:
							break;
					}
					break;
			}
		}
		
		// Update views content
		updateViews(mgr);

		// Draw the active views on the screen
		mgr.drawActiveLayout(screen, true);

		// Sleep for a while
		SDL_framerateDelay(&fpsManager);
	}

	LogI << "Closing GUI..." << endl;

	// Close SDL
	SDL_Quit();
}

void MainView::open() {
	if(isThreadTerminated) {
		LogI << "Opening graphical interface..." << endl;
		isThreadTerminated = false;
		threadView = new thread([this] {
				this->run();
				isThreadTerminated = true;
		});
	}
	else {
		cout << "GUI is already running!" << endl;
	}
}

bool MainView::isOpen() {
	return !isThreadTerminated;
}

void MainView::close() {
	if(threadView != NULL) {
		LogI << "Joining GUI thread..." << endl;
		isThreadTerminated = true;
		threadView->join();
		delete threadView;
		threadView = NULL;
		LogI << "GUI thread terminated" << endl;
	}
}

MainView::~MainView() {
	close();
}
