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
#include "view/trackbar/Trackbar_Horizontal.hpp"
#include "view/trackbar/Trackbar_Vertical.hpp"
#include "view/Digital.hpp"
#include "view/ToggleBox.hpp"
#include "view/LogView.hpp"
#include "view/ImageView.hpp"
#include "view/TextView.hpp"
#include "view/EmptyBoxView.hpp"
#include "view/PointerView.hpp"
#include "improc/UserPatternDetectionTest.hpp"
#include "improc/RoadDetectionTest.hpp"

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
	int carX = 330, carY = 25;
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
	defaultLayout.addView("toggle grass", new ToggleBox("NO GRASS", "GRASS DETECTED", 535, 190));

	// motors digital infos
	defaultLayout.addView("dVoltageFront", new Digital("%1.1fV", 380, 70, 50, 16, false));
	defaultLayout.addView("dCurrentFront", new Digital("%1.1fmA", 450, 70, 50, 16, false));
	defaultLayout.addView("dVoltageLeft", new Digital("%1.1fV", 328, 240, 50, 16, false));
	defaultLayout.addView("dCurrentLeft", new Digital("%1.1fmA", 398, 240, 50, 16, false));
	defaultLayout.addView("dVoltageRight", new Digital("%1.1fV", 433, 260, 50, 16, false));
	defaultLayout.addView("dCurrentRight", new Digital("%1.1fmA", 503, 260, 50, 16, false));
	
	// motors trackbar
	defaultLayout.addView("tbVoltageFront", new Trackbar_Vertical(0, 100, 415, 50, 10, 130));
	defaultLayout.addView("tbCurrentFront", new Trackbar_Vertical(0, 2, 435, 50, 10, 130));
	defaultLayout.addView("tbVoltageLeft", new Trackbar_Vertical(0, 100, 363, 230, 10, 130));
	defaultLayout.addView("tbCurrentLeft", new Trackbar_Vertical(0, 2, 383, 230, 10, 130));
	defaultLayout.addView("tbVoltageRight", new Trackbar_Vertical(0, 100, 468, 230, 10, 130));
	defaultLayout.addView("tbCurrentRight", new Trackbar_Vertical(0, 2, 488, 230, 10, 130));

	// distance
	defaultLayout.addView("distFrontLeft", new Digital("%.0fcm", 330, 5, 65));
	defaultLayout.addView("distFrontCenter", new Digital(".0fcm", 397, 5, 66));
	defaultLayout.addView("distFrontRight", new Digital("%.0fcm", 465, 5, 65));
	defaultLayout.addView("distBackLeft", new Digital("%.0fcm", 330, 380, 65));
	defaultLayout.addView("distBackCenter", new Digital("%.0fcm", 397, 380, 66));
	defaultLayout.addView("distBackRight", new Digital("%.0fcm", 465, 380, 65));

	// other
	defaultLayout.addView("keyboard", new KeyboardInput(commandMotorFront, commandMotorBack, 0, 240, 320, 160));
	defaultLayout.addView("camera", new ImageView(0, 0, 320, 240));


	//// LOG FULLSCREEN ////
	Layout & logLayout = mgr.createLayout("logs");

	logLayout.addView("logs", new LogView(0,0,800,400));


	//// USER DETECTION ////
	Layout & userLayout = mgr.createLayout("user");

	userLayout.addView("filter", new ImageView(0,0,400,300));
	userLayout.addView("result", new ImageView(400,0,400,300));
	userLayout.addView("logs", new LogView(0,300,800,100));

	//// ROAD IMPROC ////
	Layout & roadImprocLayout = mgr.createLayout("roadImproc");
	roadImprocLayout.addView("roadimage", new ImageView(0, 0, 400, 300));
	roadImprocLayout.addView("roadcamera", new ImageView(400, 0, 400, 300));


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
		SDL_Surface * cam = Camera::getBitmap();
		l.getImageView("camera").setImage(cam, ImageView::NORMAL);
		SDL_FreeSurface(cam);

	}
	else if(mgr.isActive("user")) {
		Layout & l = mgr.getLayout("user");
		l.getImageView("result").setImage(&UserDetectionTest.detector.getResultImage());
		l.getImageView("filter").setImage(&UserDetectionTest.detector.getFilterImage());
	}
	else if(mgr.isActive("roadImproc")) {
		Layout & l = mgr.getLayout("roadImproc");
		l.getImageView("roadimage").setImage(&roadDetectionTest.detector.getImage());
		SDL_Surface * cam = Camera::getBitmap();
		l.getImageView("roadcamera").setImage(cam, ImageView::NORMAL);
		SDL_FreeSurface(cam);
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
		threadView = NULL;
	}
}

// draw line from (x, y) (screen space) to (x2, y2) (car image space)
void MainView::drawPointerLine(int x, int y, int x2, int y2, SDL_Rect & carPos) {
	aalineRGBA(screen, x,y,x2+carPos.x,y2+carPos.y,      0,0,200,255);
	filledCircleRGBA(screen, x2+carPos.x,y2+carPos.y, 3, 0,0,230,255);
}
