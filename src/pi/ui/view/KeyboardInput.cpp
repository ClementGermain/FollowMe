#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <iostream>
#include <vector>
#include "KeyboardInput.hpp"

using namespace std;


KeyboardInput::KeyboardInput(void (*commandMotorFront)(int), void (*commandMotorBack)(int), int x, int y, int w, int h) :
	View(x, y),
	enabled(false),
	boostEnabled(false),
	isKeyPressed(4, false),
	lastStates(2, 0), 
	commandMotorFront(commandMotorFront),
	commandMotorBack(commandMotorBack),
	arrowsBMP(SDL_LoadBMP("../../res/img/arrows.bmp"), [](SDL_Surface * s){SDL_FreeSurface(s);}),
	buffer(SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0,0,0,0), [](SDL_Surface * s){SDL_FreeSurface(s);}),
	invalidate(true)
{
}

bool KeyboardInput::handleEvent(SDL_Event & event) {
	bool isUp = false;
	int keycode = 0;

	switch(event.type) {
		case SDL_KEYUP:
			isUp = true;
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym) {
			case SDLK_UP:
				keycode++;
			case SDLK_DOWN:
				keycode++;
			case SDLK_LEFT:
				keycode++;
			case SDLK_RIGHT:
				isKeyPressed[keycode] = !isUp;
				break;
			case SDLK_SPACE:
				if(isUp)
					toggleEnabled();
				break;
			case SDLK_RALT:
			case SDLK_LALT:
				break;
			default:
				return false;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if(screenPos.x <= event.button.x && event.button.x < screenPos.x+buffer->w
					&& screenPos.y <= event.button.y && event.button.y < screenPos.y+buffer->h) {
				toggleEnabled();
			} else {
				return false;
			}
			break;
		default:
			return false;
	}

	boostEnabled = ((SDL_GetModState() & KMOD_LALT) == KMOD_LALT);

	int newDirection[2] = {
		enabled * (1+boostEnabled) * (isKeyPressed[3] - isKeyPressed[2]), // forward/backward
		enabled * (isKeyPressed[1] - isKeyPressed[0]) // turn left/right
	};
	
	// Update command motors back (go forward/backward)
	if(newDirection[0] != lastStates[0]) {
		commandMotorBack(newDirection[0]);	
		lastStates[0] = newDirection[0];
		invalidate = true;
	}
		
	// Update command motor front (turn left/right)
	if(newDirection[1] != lastStates[1]) {
		commandMotorFront(newDirection[1]);	
		lastStates[1] = newDirection[1];
		invalidate = true;
	}

	return true;
}

void KeyboardInput::toggleEnabled() {
	enabled = !enabled;
	invalidate = true;
}

void KeyboardInput::draw(SDL_Surface * screen, bool needRedraw, bool updateScreen) {
	SDL_Surface * buffer = this->buffer.get();
	if(invalidate) {
		SDL_Rect pos = {
			(Sint16) ((buffer->w-arrowsBMP.get()->w) / 2),
			(Sint16) ((buffer->h-arrowsBMP.get()->h) / 2)
		};
		
		// Clear background
		SDL_FillRect(buffer, NULL, 0xffffffff);

		// Arrow keys image
		SDL_BlitSurface(arrowsBMP.get(), NULL, buffer, &pos);
		
		if(!enabled) {
			// Disabled
			boxRGBA(buffer, 0,0, buffer->w, buffer->h, 0,0,0,200);
			// Text
			const char * text = "Click to enable/disable key control";
			stringRGBA(buffer, (buffer->w-strlen(text)*8) / 2, (buffer->h-8) / 2, text, 255,255,255,255);
		}
		else {
			// Show valid pressed keys
			SDL_Rect rectUp =    {67, 15, 65,65};
			SDL_Rect rectDown =  {67, 80, 65,65};
			SDL_Rect rectLeft =  {0,  80, 65,65};
			SDL_Rect rectRight = {133,80, 65,65};

			int alpha = 128;

			switch(lastStates[0]) {
			case KeyboardInput::GoFastForward:
				alpha += 64;
			case KeyboardInput::GoForward:
				boxRGBA(buffer, pos.x+rectUp.x, pos.y+rectUp.y, pos.x+rectUp.x+rectUp.w, pos.y+rectUp.y+rectUp.h, 255,255,255,alpha);
				break;
			case KeyboardInput::GoFastBackward:
				alpha += 64;
			case KeyboardInput::GoBackward:
				boxRGBA(buffer, pos.x+rectDown.x, pos.y+rectDown.y, pos.x+rectDown.x+rectDown.w, pos.y+rectDown.y+rectDown.h, 255,255,255,alpha);
				break;
			}
			switch(lastStates[1]) {
			case KeyboardInput::TurnLeft:
				boxRGBA(buffer, pos.x+rectLeft.x, pos.y+rectLeft.y, pos.x+rectLeft.x+rectLeft.w, pos.y+rectLeft.y+rectLeft.h, 255,255,255,alpha);
				break;
			case KeyboardInput::TurnRight:
				boxRGBA(buffer, pos.x+rectRight.x, pos.y+rectRight.y, pos.x+rectRight.x+rectRight.w, pos.y+rectRight.y+rectRight.h, 255,255,255,alpha);
				break;
			}
		}
	}

	if(needRedraw || invalidate) {
		SDL_BlitSurface(buffer, NULL, screen, &screenPos);
		if(updateScreen)
			SDL_UpdateRect(screen, screenPos.x, screenPos.y, buffer->w, buffer->h);
	}
	invalidate = false;
}
