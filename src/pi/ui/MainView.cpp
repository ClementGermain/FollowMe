#include "MainView.hpp"
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_framerate.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <thread>
#include <iostream>
#include <vector>
#include "../car/Car.hpp"
#include "Camera.hpp"
#include "KeyboardInput.hpp"
#include "Trackbar.hpp"
#include "Digital.hpp"
#include "LogView.hpp"

using namespace std;


MainView::MainView(Camera & camera) : threadView(NULL), isThreadTerminated(true), camera(camera) {

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

void commandMotorFront(int direction) {
	float speed = 1;
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
	float speed = 1;
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
	SDL_BlitSurface(car, NULL,  screen, &carPos);

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

// draw line from (x, y) (screen space) to (x2, y2) (car image space)
void MainView::drawPointerLine(int x, int y, int x2, int y2, SDL_Rect & carPos) {
	aalineRGBA(screen, x,y,x2+carPos.x,y2+carPos.y,      0,0,200,255);
	filledCircleRGBA(screen, x2+carPos.x,y2+carPos.y, 3, 0,0,230,255);
}

void MainView::initializeViews() {
	// motors
	digitalValues.emplace_back("PWM: %.0f%%", 540, 50, 80, 16, false);
	trackbarMotors.emplace_back(0, 100, 620, 50);
	digitalValues.emplace_back("I: %.0fmA", 540, 70, 80, 16, false);
	trackbarMotors.emplace_back(0, 2, 620, 70);
	digitalValues.emplace_back("PWM: %.0f%%", 540, 240, 80, 16, false);
	trackbarMotors.emplace_back(0, 100, 620, 240);
	digitalValues.emplace_back("I: %.0fmA", 540, 260, 80, 16, false);
	trackbarMotors.emplace_back(0, 2, 620, 260);
	digitalValues.emplace_back("PWM: %.0f%%", 540, 330, 80, 16, false);
	trackbarMotors.emplace_back(0, 100, 620, 330);
	digitalValues.emplace_back("I: %.0fmA", 540, 350, 80, 16, false);
	trackbarMotors.emplace_back(0, 2, 620, 350);
	// distance
	digitalValues.emplace_back("%.2fm", 330, 5, 70);
	digitalValues.emplace_back("%.2fm", 400, 5, 70);
	digitalValues.emplace_back("%.2fm", 470, 5, 70);
	digitalValues.emplace_back("%.2fm", 330, 380, 70);
	digitalValues.emplace_back("%.2fm", 400, 380, 70);
	digitalValues.emplace_back("%.2fm", 470, 380, 70);
	// raspi
	digitalValues.emplace_back("CPU: %.0f%%", 540, 170, 80, 16, false);
}

void MainView::updateViews() {
	BarstowModel_Typedef model;
	Car::getModelStructure(model);
	
	digitalValues[0].setValue(model.directionMotor.voltage);
	trackbarMotors[0].setPosition(model.directionMotor.voltage);
	digitalValues[1].setValue(model.directionMotor.current);
	trackbarMotors[1].setPosition(model.directionMotor.current);

	digitalValues[2].setValue(model.leftWheelMotor.voltage);
	trackbarMotors[2].setPosition(model.leftWheelMotor.voltage);
	digitalValues[3].setValue(model.leftWheelMotor.current);
	trackbarMotors[3].setPosition(model.leftWheelMotor.current);

	digitalValues[4].setValue(model.rightWheelMotor.voltage);
	trackbarMotors[4].setPosition(model.rightWheelMotor.voltage);
	digitalValues[5].setValue(model.rightWheelMotor.current);
	trackbarMotors[5].setPosition(model.rightWheelMotor.current);

	digitalValues[6].setValue(model.frontLeftUSensor.distance);
	digitalValues[7].setValue(model.frontCenterUSensor.distance);
	digitalValues[8].setValue(model.frontRightUSensor.distance);
	digitalValues[9].setValue(model.rearLeftUSensor.distance);
	digitalValues[10].setValue(model.rearCenterUSensor.distance);
	digitalValues[11].setValue(model.rearRightUSensor.distance);

	digitalValues[12].setValue(cpuLoad.get());
}

void MainView::run() {
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		return;
	if(!(screen = SDL_SetVideoMode(800, 400, 32, SDL_HWSURFACE)))
		return;
	
	SDL_WM_SetCaption("FollowMe GUI", NULL);
	
	FPSmanager fpsManager;
	SDL_initFramerate(&fpsManager);

	car = SDL_LoadBMP("../../res/img/car_top_view.bmp");

	drawStaticViews();
	
	// dynamic views
	initializeViews();
	// Arrows
	KeyboardInput arrowKeysControl(commandMotorFront, commandMotorBack, 0, 240, 320, 160);
	// Logs
	LogView logView(0, 0, 320, 240);
	

	bool end = false;

	while(!end) {
		/// Handle event queue
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			if(arrowKeysControl.handleEvent(event))
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
						case SDLK_a:
							if((SDL_GetModState() & KMOD_LCTRL) == KMOD_LCTRL)
								end = true;
						default:
							break;
					}
					break;
			}
		}
		
		/// Refresh display
		updateViews();
		
		// arrows
		arrowKeysControl.draw(screen);
		// trackbars
		for(Trackbar & tb : trackbarMotors) {
			//tb.setPosition(SDL_getFramecount(&fpsManager));
			tb.draw(screen);
		}
		// digitals
		for(Digital & d : digitalValues) {
			d.draw(screen);
		}
		// Camera
		// TODO efficient CameraView
		SDL_Surface * cam = camera.getBitmap(1);
		SDL_BlitSurface(cam, NULL, screen, NULL);
		SDL_FreeSurface(cam);

		// Logs (FIXME this actually erase camera view)
		logView.draw(screen);
		
		// commit screen buffer
		SDL_Flip(screen);

		SDL_framerateDelay(&fpsManager);
	}

	trackbarMotors.clear();
	digitalValues.clear();
	SDL_FreeSurface(car);
	SDL_Quit();
}

