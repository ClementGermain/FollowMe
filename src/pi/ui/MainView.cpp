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
#include "improc/UserPatternDetectionTest.hpp"
#include "improc/RoadDetectionTest.hpp"
#include "improc/RoadDetection.hpp"

using namespace std;

MainView::MainView() : threadView(NULL), isThreadTerminated(true) {

}

void commandMotorFront(int direction) {
	LogD << "Dir " << direction <<endl;
	float speed = 0.5f;
	switch(direction) {
		case KeyboardInput::Idle:
			Car::writeControlMotor(Car::NoTurn, speed); 
			break;
		case KeyboardInput::TurnLeft:
			Car::writeControlMotor(Car::TurnLeft, speed); 
			break;
		case KeyboardInput::TurnRight:
			Car::writeControlMotor(Car::TurnRight, speed); 
			break;
	}
}

void commandMotorBack(int direction) {
	LogD << "Prop " << direction<<endl;
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
}

void MainView::initializeViews(ViewManager & mgr) {
	//// DEFAULT VIEW ////
	Layout & defaultLayout = mgr.createLayout("Motor");

	// Car top view
	defaultLayout.addView("imgCar", new ImageView(330, 25, 200, 350));
	SDL_Surface * car = SDL_LoadBMP("../../res/img/car_top_view.bmp");
	defaultLayout.getImageView("imgCar").setImage(car);
	SDL_FreeSurface(car);

	// raspi info
	defaultLayout.addView("boxRaspi", new EmptyBoxView(535, 10, 260, 50));
	defaultLayout.addView("titleRaspi", new TextView("Raspberry Pi 3", 540, 20, 250, 16, true));
	defaultLayout.addView("cpu", new Digital("CPU: %.0f%%", 540, 40, 80, 16, false));

	// Toggle Informations
	defaultLayout.addView("toggle_motor", new ToggleBox("MOTOR OK", "MOTOR FAILURE", 535, 70));
	defaultLayout.addView("toggle_user", new ToggleBox("USER DETECTED", "NO USER", 535, 110));
	defaultLayout.addView("toggle_obstacle", new ToggleBox("NO OBSTACLES", "OBSTACLE DETECTED", 535, 150));
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
	defaultLayout.addView("dSpeedLeft", new Digital("Spd: %02.1f T/s", 330, 345, 100, 16, false));

	defaultLayout.addView("dCmdRight", new Digital("Cmd: %02.0f/100", 435, 225, 100, 16, false));
	defaultLayout.addView("dCurrentRight", new Digital("Cur: %04.0f mA", 435, 255, 100, 16, false));
	defaultLayout.addView("dVoltage1Right", new Digital("U1 : %1.2f V", 435, 285, 100, 16, false));
	defaultLayout.addView("dVoltage2Right", new Digital("U2 : %1.2f V", 435,315, 100, 16, false));
	defaultLayout.addView("dSpeedRight", new Digital("Spd: %02.1f T/s", 435, 345, 100, 16, false));
	
	// motors trackbar

	defaultLayout.addView("tbCmdFront", new Trackbar_Horizontal(-1, 1, 385, 70, 90, 10, CENTREE));
	defaultLayout.addView("tbCurrentFront", new Trackbar_Horizontal(0, 2000, 385, 100, 90, 10));
	defaultLayout.addView("tbVoltage1Front", new Trackbar_Horizontal(0, 8000, 385, 130, 90, 10));
	defaultLayout.addView("tbVoltage2Front", new Trackbar_Horizontal(0, 8000, 385, 160, 90, 10));

	defaultLayout.addView("tbCmdLeft", new Trackbar_Horizontal(-1, 1, 333, 240, 90, 10, CENTREE));
	defaultLayout.addView("tbCurrentLeft", new Trackbar_Horizontal(0, 2000, 333, 270, 90, 10));
	defaultLayout.addView("tbVoltage1Left", new Trackbar_Horizontal(0, 8000, 333, 300, 90, 10));
	defaultLayout.addView("tbVoltage2Left", new Trackbar_Horizontal(0, 8000, 333, 330, 90, 10));
	defaultLayout.addView("tbSpeedLeft", new Trackbar_Horizontal(0, 10, 333, 360, 90, 10));

	defaultLayout.addView("tbCmdRight", new Trackbar_Horizontal(-1, 1, 438, 240, 90, 10, CENTREE));
	defaultLayout.addView("tbCurrentRight", new Trackbar_Horizontal(0, 2000, 438, 270, 90, 10));
	defaultLayout.addView("tbVoltage1Right", new Trackbar_Horizontal(0, 8000, 438, 300, 90, 10));
	defaultLayout.addView("tbVoltage2Right", new Trackbar_Horizontal(0, 8000, 438, 330, 90, 10));
	defaultLayout.addView("tbSpeedRight", new Trackbar_Horizontal(0, 10, 438, 360, 90, 10));

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
	sensorLayout.addView("sensor_toggle_user", new ToggleBox("USER DETECTED", "NO USER", 535, 110));
	sensorLayout.addView("sensor_toggle_obstacle", new ToggleBox("NO OBSTACLES", "OBSTACLE DETECTED", 535, 150));
	sensorLayout.addView("sensor_state_road", new StateBox(535, 190));
	sensorLayout.getStateBoxView("sensor_state_road").add_state("ROAD : NO IDEA", 150, 0, 0); //NO ROAD DETECTED
	sensorLayout.getStateBoxView("sensor_state_road").add_state("ROAD : YES", 0, 150, 0); //ROAD DETECTED
	sensorLayout.getStateBoxView("sensor_state_road").add_state("ROAD : LIKELY", 255, 153, 0); //ROAD UNCERTAIN
	sensorLayout.getStateBoxView("sensor_state_road").add_state("ROAD : UNLIKELY", 255, 153, 0); //ROAD UNCERTAIN
	sensorLayout.getStateBoxView("sensor_state_road").add_state("ROAD : NO", 150, 0, 0);

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
	Car::getModelStructure(model);
	Car::getControlStructure(control);
	
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
		l.getTrackbarView("tbCurrentFront").setInnerBounds(860, 1140); // example: add inner bounds 
		
		l.getDigitalView("dCmdLeft").setValue( control.propulsionMotor.speed * control.propulsionMotor.direction * 100.0);
		l.getTrackbarView("tbCmdLeft").setPosition( control.propulsionMotor.speed * control.propulsionMotor.direction );
		l.getDigitalView("dVoltage1Left").setValue(((float) model.leftWheelMotor.voltage1)/1000.0);
		l.getTrackbarView("tbVoltage1Left").setPosition(model.leftWheelMotor.voltage1);
		l.getDigitalView("dVoltage2Left").setValue(((float) model.leftWheelMotor.voltage2)/1000.0);
		l.getTrackbarView("tbVoltage2Left").setPosition(model.leftWheelMotor.voltage2);
		l.getDigitalView("dSpeedLeft").setValue(((float) model.leftWheelMotor.speed)/60);
		l.getTrackbarView("tbSpeedLeft").setPosition(model.leftWheelMotor.speed/60);
		l.getDigitalView("dCurrentLeft").setValue(((float) model.leftWheelMotor.current));
		l.getTrackbarView("tbCurrentLeft").setPosition(model.leftWheelMotor.current);

		l.getDigitalView("dCmdRight").setValue( control.propulsionMotor.speed * control.propulsionMotor.direction * 100.0);
		l.getTrackbarView("tbCmdRight").setPosition( control.propulsionMotor.speed * control.propulsionMotor.direction );
	    	l.getDigitalView("dVoltage1Right").setValue(((float) model.rightWheelMotor.voltage1)/1000.0);
		l.getTrackbarView("tbVoltage1Right").setPosition(model.rightWheelMotor.voltage1);
		l.getDigitalView("dVoltage2Right").setValue(((float) model.rightWheelMotor.voltage2)/1000.0);
		l.getTrackbarView("tbVoltage2Right").setPosition(model.rightWheelMotor.voltage2);
		l.getDigitalView("dSpeedRight").setValue(((float) model.rightWheelMotor.speed)/60);
		l.getTrackbarView("tbSpeedRight").setPosition(model.rightWheelMotor.speed/60);
		l.getDigitalView("dCurrentRight").setValue(((float) model.rightWheelMotor.current));
		l.getTrackbarView("tbCurrentRight").setPosition(model.rightWheelMotor.current);
		
		l.getDigitalView("cpu").setValue(cpuLoad.get());

		l.getToggleBoxView("toggle_motor").toggle(true);
		l.getToggleBoxView("toggle_user").toggle(UserDetectionTest.detector.isDetected());
		l.getToggleBoxView("toggle_obstacle").toggle(!ObstacleDetection::isGlobalDetected());
		l.getStateBoxView("state_road").set_state(roadDetectionTest.detector.canGoForward());

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
		
		l.getTrackbarView("sensor_UserDistance").setPosition(UserDetectionTest.detector.getDistance()-Car::CarSize);
		l.getTrackbarView("sensor_UserAngle").setPosition(UserDetectionTest.detector.getDirection()*180/M_PI);
		l.getDigitalView("sensor_UserDistanceText").setValue(UserDetectionTest.detector.getDistance()-Car::CarSize);
		l.getDigitalView("sensor_UserAngleText").setValue(UserDetectionTest.detector.getDirection()*180/M_PI);

		l.getToggleBoxView("sensor_toggle_motor").toggle(true);
		l.getToggleBoxView("sensor_toggle_user").toggle(UserDetectionTest.detector.isDetected() && UserDetectionTest.detector.getDistance() < 3.0f);
		l.getToggleBoxView("sensor_toggle_obstacle").toggle(!ObstacleDetection::isGlobalDetected());
		l.getStateBoxView("sensor_state_road").set_state(roadDetectionTest.detector.canGoForward());
		
		l.getDigitalView("sensor_cpu").setValue(UserDetectionTest.detector.getDistance());

		cv::Mat cam;
		Camera::getImage(cam);
		l.getImageView("camera").setImage(&cam, ImageView::NORMAL);
	}
	else if(mgr.isActive("User Detection")) {
		Layout & l = mgr.getLayout("User Detection");
		l.getImageView("result").setImage(&UserDetectionTest.detector.getResultImage());
		l.getImageView("filter").setImage(&UserDetectionTest.detector.getFilterImage());
		((PlotsView*)l.getView("graph"))->addPlot(UserDetectionTest.detector.getDistance());
	}
	else if(mgr.isActive("Road Detection")) {
		Layout & l = mgr.getLayout("Road Detection");
		l.getImageView("roadimage").setImage(&roadDetectionTest.detector.getImage(), ImageView::FITXY);
		cv::Mat cam;
		Camera::getImage(cam);
		//l.getImageView("roadcamera").setImage(&cam, ImageView::NORMAL);
		l.getImageView("roadcamera").setImage(&roadDetectionTest.detector.getCameraImage(), ImageView::FITXY);	
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
			if(mgr.isActive("Motor") || mgr.isActive("Sensor"))
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

	// Close SDL
	SDL_Quit();
}

void MainView::open() {
	if(isThreadTerminated) {
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
		isThreadTerminated = true;
		threadView->join();
		delete threadView;
		threadView = NULL;
	}
}

MainView::~MainView() {
	close();
}
