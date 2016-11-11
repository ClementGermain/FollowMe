#include "MainView.hpp"
#include <SDL/SDL.h>
#include <SDL/SDL_framerate.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <thread>
#include <iostream>
#include <vector>
#include "KeyboardInput.hpp"
#include "Trackbar.hpp"
#include "Digital.hpp"

using namespace std;


MainView::MainView() : threadView(NULL) {

}

void MainView::open() {
	if(threadView == NULL)
		threadView = new thread([this] { this->run(); });
}

MainView::~MainView() {
	if(threadView != NULL) {
		threadView->join();
		delete threadView;
	}
}

void commandMotorFront(int direction) {
	cout << "TODO motor front : ";
	switch(direction) {
		case KeyboardInput::Idle:
			cout << "stop";
			break;
		case KeyboardInput::TurnLeft:
			cout << "turn left";
			break;
		case KeyboardInput::TurnRight:
			cout << "turn right";
			break;
	}
	cout << endl;
}

void commandMotorBack(int direction) {
	cout << "TODO motor back : ";
	switch(direction) {
		case KeyboardInput::Idle:
			cout << "stop";
			break;
		case KeyboardInput::GoForward:
			cout << "go forward";
			break;
		case KeyboardInput::GoBackward:
			cout << "go backward";
			break;
	}
	cout << endl;
}


void MainView::run() {
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		return;
	if(!(screen = SDL_SetVideoMode(800, 400, 32, SDL_HWSURFACE)))
		return;
	
	SDL_WM_SetCaption("FollowMe GUI", NULL);
	
	FPSmanager fpsManager;
	SDL_initFramerate(&fpsManager);

	KeyboardInput arrowKeysControl(commandMotorFront, commandMotorBack, 0, 240, 320, 160);

	SDL_FillRect(screen, NULL, 0xffffffff);
	stringRGBA(screen, 540, 10, "Motor Front", 0,0,120,255);
	stringRGBA(screen, 540, 145, "Raspberry Pi 3", 0,0,120,255);
	stringRGBA(screen, 540, 220, "Motor Left", 0,0,120,255);
	stringRGBA(screen, 540, 310, "Motor Right", 0,0,120,255);

	SDL_Rect carPos = {330, 25};
	car = SDL_LoadBMP("../../../res/img/car_top_view.bmp");
	SDL_BlitSurface(car, NULL,  screen, &carPos);

	digitalValues.emplace_back("PWM: %.0f%%", 540, 40, 80, 15, false);
	trackbarMotors.emplace_back(0, 100, 620, 40);
	digitalValues.emplace_back("I: %.0fmA", 540, 70, 80, 15, false);
	trackbarMotors.emplace_back(0, 2, 620, 70);
	digitalValues.emplace_back("PWM: %.0f%%", 540, 250, 80, 15, false);
	trackbarMotors.emplace_back(0, 100, 620, 250);
	digitalValues.emplace_back("I: %.0fmA", 540, 280, 80, 15, false);
	trackbarMotors.emplace_back(0, 2, 620, 280);
	digitalValues.emplace_back("PWM: %.0f%%", 540, 340, 80, 15, false);
	trackbarMotors.emplace_back(0, 100, 620, 340);
	digitalValues.emplace_back("I: %.0fmA", 540, 370, 80, 15, false);
	trackbarMotors.emplace_back(0, 2, 620, 370);

	digitalValues.emplace_back("%.2fm", 330, 5, 70);
	digitalValues.emplace_back("%.2fm", 400, 5, 70);
	digitalValues.emplace_back("%.2fm", 470, 5, 70);
	digitalValues.emplace_back("%.2fm", 330, 380, 70);
	digitalValues.emplace_back("%.2fm", 400, 380, 70);
	digitalValues.emplace_back("%.2fm", 470, 380, 70);

	digitalValues.emplace_back("CPU: %.0f%%", 540, 175, 80, 15, false);

	bool end = false;

	while(!end) {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			if(arrowKeysControl.handleEvent(event))
				continue;

			switch(event.type) {
				case SDL_QUIT:
					end = true;
					break;
				case SDL_KEYUP:
				case SDL_KEYDOWN:

					switch(event.key.keysym.sym) {
						case SDLK_ESCAPE:
							end = true;
							break;
						case SDLK_F4:
							if((SDL_GetModState() & KMOD_LALT) == KMOD_LALT)
								end = true;
							break;
						default:
							break;
					}
					break;
			}
		}
		
		arrowKeysControl.draw(screen);

		for(Trackbar & tb : trackbarMotors) {
			tb.setPosition(SDL_getFramecount(&fpsManager));
			tb.draw(screen);
		}

		digitalValues.back().setValue(cpuLoad.get());
		for(Digital & d : digitalValues) {
			d.draw(screen);
		}
		SDL_Flip(screen);

		SDL_framerateDelay(&fpsManager);
	}

	trackbarMotors.clear();
	digitalValues.clear();
	SDL_FreeSurface(car);
	SDL_Quit();
}

int main() {
	MainView view;
	view.open();
	return 0;
}
