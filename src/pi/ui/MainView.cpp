#include "MainView.hpp"
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_framerate.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <thread>
#include <iostream>
#include <map>
#include <memory>
#include "car/Car.hpp"
#include "utils/Log.hpp"
#include "car/Camera.hpp"
#include "view/KeyboardInput.hpp"
#include "view/Trackbar.hpp"
#include "view/Digital.hpp"
#include "view/LogView.hpp"
#include "view/ImageView.hpp"
#include "view/TextView.hpp"
#include "improc/UserPatternDetectionTest.hpp"
#include "improc/RoadDetectionTest.hpp"

using namespace std;


MainView::MainView(Camera & camera) : threadView(NULL), isThreadTerminated(true), camera(camera) {

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
	switch(direction) {
		case KeyboardInput::Idle:
			Car::writeControlMotor(Car::Stop, speed); 
			break;
		case KeyboardInput::GoForward:
			Car::writeControlMotor(Car::MoveForward, speed); 
			break;
		case KeyboardInput::GoBackward:
			Car::writeControlMotor(Car::MoveBackward, speed); 
			break;
	}
}

void MainView::drawStaticViews() {
	// White background
	SDL_FillRect(screen, NULL, 0xffffffff);
	// Box
	roundedRectangleRGBA(screen, 535, 25,  795, 25+70,  5, 0, 0, 180, 255);
	roundedRectangleRGBA(screen, 535, 145, 795, 145+50, 5, 0, 0, 180, 255);
	roundedRectangleRGBA(screen, 535, 215, 795, 215+70, 5, 0, 0, 180, 255);
	roundedRectangleRGBA(screen, 535, 305, 795, 305+70, 5, 0, 0, 180, 255);
	// Title
	int charSize = 8, hOffset = (16-charSize)/2;
	stringRGBA(screen, 540, 30+hOffset, "Motor Front",     0,0,180,255);
	stringRGBA(screen, 540, 150+hOffset, "Raspberry Pi 3", 0,0,180,255);
	stringRGBA(screen, 540, 220+hOffset, "Motor Left",     0,0,180,255);
	stringRGBA(screen, 540, 310+hOffset, "Motor Right",    0,0,180,255);

	// Car picture
	SDL_Rect carPos = {330, 25};

	// Link image/text
	drawPointerLine(535, 40, 190, 80,  carPos);
	drawPointerLine(535, 40, 20,  80,  carPos);
	drawPointerLine(535, 160, 127, 198, carPos);
	drawPointerLine(535, 230, 20,  279, carPos);
	drawPointerLine(535, 320, 190, 279, carPos);
	drawPointerLine(365, 25, 52, 21, carPos);
	drawPointerLine(435, 25, 100, 5, carPos);
	drawPointerLine(505, 25, 152, 18, carPos);
	drawPointerLine(365, 375, 66, 340, carPos);
	drawPointerLine(435, 375, 100, 344, carPos);
	drawPointerLine(505, 375, 142, 340, carPos);
}

void MainView::initializeViews(ViewManager & mgr) {
	//// DEFAULT VIEW ////
	Layout & defaultLayout = mgr.createLayout("default");

	// Car top view
	defaultLayout.addView("imgCar", new ImageView(330, 25, 200, 350));
	SDL_Surface * car = SDL_LoadBMP("../../res/img/car_top_view.bmp");
	defaultLayout.getImageView("imgCar").setImage(car);
	SDL_FreeSurface(car);

	// titles
	defaultLayout.addView("titleFront", new TextView("Motor front", 540, 30, 250, 16, true));
	defaultLayout.addView("titleRaspi", new TextView("Raspberry Pi 3", 540, 150, 250, 16, true));
	defaultLayout.addView("titleLeft", new TextView("Motor left", 540, 220, 250, 16, true));
	defaultLayout.addView("titleRight", new TextView("Motor right", 540, 310, 250, 16, true));

	// motors
	defaultLayout.addView("dVoltageFront", new Digital("V: %.0fmV", 540, 50, 80, 16, false));
	defaultLayout.addView("tbVoltageFront", new Trackbar(0, 100, 620, 50));
	defaultLayout.addView("dCurrentFront", new Digital("I: %.0fmA", 540, 70, 80, 16, false));
	defaultLayout.addView("tbCurrentFront", new Trackbar(0, 2, 620, 70));
	defaultLayout.addView("dVoltageLeft", new Digital("V: %.0fmV", 540, 240, 80, 16, false));
	defaultLayout.addView("tbVoltageLeft", new Trackbar(0, 100, 620, 240));
	defaultLayout.addView("dCurrentLeft", new Digital("I: %.0fmA", 540, 260, 80, 16, false));
	defaultLayout.addView("tbCurrentLeft", new Trackbar(0, 2, 620, 260));
	defaultLayout.addView("dVoltageRight", new Digital("V: %.0fmV", 540, 330, 80, 16, false));
	defaultLayout.addView("tbVoltageRight", new Trackbar(0, 100, 620, 330));
	defaultLayout.addView("dCurrentRight", new Digital("I: %.0fmA", 540, 350, 80, 16, false));
	defaultLayout.addView("tbCurrentRight", new Trackbar(0, 2, 620, 350));

	// distance
	defaultLayout.addView("distFrontLeft", new Digital("%.0fcm", 330, 5, 70));
	defaultLayout.addView("distFrontCenter", new Digital("%.0fcm", 400, 5, 70));
	defaultLayout.addView("distFrontRight", new Digital("%.0fcm", 470, 5, 70));
	defaultLayout.addView("distBackLeft", new Digital("%.0fcm", 330, 380, 70));
	defaultLayout.addView("distBackCenter", new Digital("%.0fcm", 400, 380, 70));
	defaultLayout.addView("distBackRight", new Digital("%.0fcm", 470, 380, 70));
	// raspi
	defaultLayout.addView("cpu", new Digital("CPU: %.0f%%", 540, 170, 80, 16, false));
	// other
	defaultLayout.addView("keyboard", new KeyboardInput(commandMotorFront, commandMotorBack, 0, 240, 320, 160));
	defaultLayout.addView("camera", new ImageView(0, 0, 320, 240));

	//// LOG FULLSCREEN ////
	Layout & logLayout = mgr.createLayout("logs");

	logLayout.addView("logs", new LogView(0,0,800,400));
}

void MainView::updateViews(ViewManager & mgr) {
	BarstowModel_Typedef model;
	Car::getModelStructure(model);
	
	if(mgr.isActive("default")) {
		Layout & l = mgr.getLayout("default");
		l.getDigitalView("dVoltageFront").setValue(model.directionMotor.voltage);
		l.getTrackbarView("tbVoltageFront").setPosition(model.directionMotor.voltage);
		l.getDigitalView("dCurrentFront").setValue(model.directionMotor.current);
		l.getTrackbarView("tbCurrentFront").setPosition(model.directionMotor.current);

		l.getDigitalView("dVoltageLeft").setValue(model.leftWheelMotor.voltage);
		l.getTrackbarView("tbVoltageLeft").setPosition(model.leftWheelMotor.voltage);
		l.getDigitalView("dCurrentLeft").setValue(model.leftWheelMotor.current);
		l.getTrackbarView("tbCurrentLeft").setPosition(model.leftWheelMotor.current);

		l.getDigitalView("dVoltageRight").setValue(model.rightWheelMotor.voltage);
		l.getTrackbarView("tbVoltageRight").setPosition(model.rightWheelMotor.voltage);
		l.getDigitalView("dCurrentRight").setValue(model.rightWheelMotor.current);
		l.getTrackbarView("tbCurrentRight").setPosition(model.rightWheelMotor.current);

		l.getDigitalView("distFrontLeft").setValue(model.frontLeftUSensor.distance);
		l.getDigitalView("distFrontCenter").setValue(model.frontCenterUSensor.distance);
		l.getDigitalView("distFrontRight").setValue(model.frontRightUSensor.distance);
		l.getDigitalView("distBackLeft").setValue(model.rearLeftUSensor.distance);
		l.getDigitalView("distBackCenter").setValue(model.rearCenterUSensor.distance);
		l.getDigitalView("distBackRight").setValue(model.rearRightUSensor.distance);

		l.getDigitalView("cpu").setValue(cpuLoad.get());
		l.getImageView("camera").setImage(&roadDetectionTest.detector.getImage(), ImageView::NORMAL);
	}
	
}

void MainView::run() {
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		return;
	if(!(screen = SDL_SetVideoMode(800, 400, 32, SDL_HWSURFACE)))
		return;
	
	SDL_WM_SetCaption("FollowMe GUI", NULL);
	
	FPSmanager fpsManager;
	SDL_initFramerate(&fpsManager);


	// TODO remove static views
	drawStaticViews();

	ViewManager mgr;
	initializeViews(mgr);

	bool end = false;

	while(!end) {
		/// Handle event queue
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			if(mgr.isActive("default"))
			if(mgr.getActiveLayout().getKeyboardView("keyboard").handleEvent(event))
				continue;

			switch(event.type) {
				case SDL_QUIT:
					end = true;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_ESCAPE:
							end = true;
							break;
						case SDLK_F4:
							if((SDL_GetModState() & KMOD_LALT) == KMOD_LALT)
								end = true;
							break;
						case SDLK_q:
							if((SDL_GetModState() & KMOD_LCTRL) == KMOD_LCTRL)
								end = true;
							break;
						case SDLK_TAB:
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
		
		/// Refresh display
		updateViews(mgr);

		mgr.drawActiveLayout(screen);

		// commit screen buffer
		SDL_Flip(screen);

		SDL_framerateDelay(&fpsManager);
	}

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

MainView::~MainView() {
	if(threadView != NULL) {
		threadView->join();
		delete threadView;
	}
}

// draw line from (x, y) (screen space) to (x2, y2) (car image space)
void MainView::drawPointerLine(int x, int y, int x2, int y2, SDL_Rect & carPos) {
	aalineRGBA(screen, x,y,x2+carPos.x,y2+carPos.y,      0,0,200,255);
	filledCircleRGBA(screen, x2+carPos.x,y2+carPos.y, 3, 0,0,230,255);
}
