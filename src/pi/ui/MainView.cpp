#include "MainView.hpp"
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_framerate.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <thread>
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include "car/Car.hpp"
#include "utils/Log.hpp"
#include "car/Camera.hpp"
#include "view/KeyboardInput.hpp"
#include "view/Trackbar.hpp"
#include "view/Digital.hpp"
#include "view/LogView.hpp"

using namespace std;


MainView::MainView(Camera & camera) : threadView(NULL), isThreadTerminated(true), camera(camera), showCamera(false) {

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
	digitalValues.emplace_back("V: %.0fmV", 540, 50, 80, 16, false);
	trackbarMotors.emplace_back(0, 100, 620, 50);
	digitalValues.emplace_back("I: %.0fmA", 540, 70, 80, 16, false);
	trackbarMotors.emplace_back(0, 2, 620, 70);
	digitalValues.emplace_back("V: %.0fmV", 540, 240, 80, 16, false);
	trackbarMotors.emplace_back(0, 100, 620, 240);
	digitalValues.emplace_back("I: %.0fmA", 540, 260, 80, 16, false);
	trackbarMotors.emplace_back(0, 2, 620, 260);
	digitalValues.emplace_back("V: %.0fmV", 540, 330, 80, 16, false);
	trackbarMotors.emplace_back(0, 100, 620, 330);
	digitalValues.emplace_back("I: %.0fmA", 540, 350, 80, 16, false);
	trackbarMotors.emplace_back(0, 2, 620, 350);
	// distance
	addView("distFrontLeft", new Digital("%.0fcm", 330, 5, 70));
	addView("distFrontCenter", new Digital("%.0fcm", 400, 5, 70));
	addView("distFrontRight", new Digital("%.0fcm", 470, 5, 70));
	addView("distBackLeft", new Digital("%.0fcm", 330, 380, 70));
	addView("distBackCenter", new Digital("%.0fcm", 400, 380, 70));
	addView("distBackRight", new Digital("%.0fcm", 470, 380, 70));
	// raspi
	addView("cpu", new Digital("CPU: %.0f%%", 540, 170, 80, 16, false));
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

	getDigitalView("distFrontLeft").setValue(model.frontLeftUSensor.distance);
	getDigitalView("distFrontCenter").setValue(model.frontCenterUSensor.distance);
	getDigitalView("distFrontRight").setValue(model.frontRightUSensor.distance);
	getDigitalView("distBackLeft").setValue(model.rearLeftUSensor.distance);
	getDigitalView("distBackCenter").setValue(model.rearCenterUSensor.distance);
	getDigitalView("distBackRight").setValue(model.rearRightUSensor.distance);

	getDigitalView("cpu").setValue(cpuLoad.get());
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
	
	addView("keyboard", new KeyboardInput(commandMotorFront, commandMotorBack, 0, 240, 320, 160));
	// Logs
	addView("logs", new LogView(0,0,320,240));
	

	bool end = false;

	while(!end) {
		/// Handle event queue
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			if(((KeyboardInput*)views["keyboard"].get())->handleEvent(event))
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
							showCamera = !showCamera;
							break;
						default:
							break;
					}
					break;
			}
		}
		
		/// Refresh display
		updateViews();
		
		// arrows
		getView("keyboard").draw(screen);
		// trackbars
		for(Trackbar & tb : trackbarMotors) {
			tb.draw(screen);
		}
		// digitals
		for(Digital & d : digitalValues) {
			d.draw(screen);
		}

		getView("distFrontLeft").draw(screen);
		getView("distFrontCenter").draw(screen);
		getView("distFrontRight").draw(screen);
		getView("distBackLeft").draw(screen);
		getView("distBackCenter").draw(screen);
		getView("distBackRight").draw(screen);
		getView("cpu").draw(screen);

		// Camera
		// TODO efficient CameraView
		if(showCamera) {
			SDL_Surface * cam = camera.getBitmap(1);
			SDL_BlitSurface(cam, NULL, screen, NULL);
			SDL_FreeSurface(cam);
		} else {
			// Logs (FIXME this actually erase camera view)
			getView("logs").draw(screen);
		}
		// commit screen buffer
		SDL_Flip(screen);

		SDL_framerateDelay(&fpsManager);
	}

	trackbarMotors.clear();
	digitalValues.clear();
	views.clear();
	SDL_FreeSurface(car);
	SDL_Quit();
}

View & MainView::getView(const string & name) {
	return *views[name].get();
}
Digital & MainView::getDigitalView(const string & name) {
	return *((Digital*)views[name].get());
}

void MainView::addView(const string & name, View * v) {
	views.emplace(name, shared_ptr<View>(v));
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

