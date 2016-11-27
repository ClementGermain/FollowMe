#ifndef __KEYBOARD_INPUT_HPP__
#define __KEYBOARD_INPUT_HPP__

#include <vector>
#include <SDL/SDL.h>
#include <memory>
#include "View.hpp"

class KeyboardInput : public View {
	public:
		KeyboardInput(void (*commandMotorFront)(int), void (*commandMotorBack)(int), int x, int y, int w, int h);
		bool handleEvent(SDL_Event & event);
		void toggleEnabled();
		bool isEnabled() { return enabled; }
		void draw(SDL_Surface * s, bool needRedraw=true, bool updateScreen=false);

		static const int GoForward = 1;
		static const int GoFastForward = 2;
		static const int GoBackward = -1;
		static const int GoFastBackward = -2;
		static const int TurnLeft = 1;
		static const int TurnRight = -1;
		static const int Idle = 0;

	private:
		bool enabled;
		bool boostEnabled;
		std::vector<bool> isKeyPressed;
		std::vector<int> lastStates;
		void (*commandMotorFront)(int);
		void (*commandMotorBack)(int);
		std::shared_ptr<SDL_Surface> arrowsBMP;
		std::shared_ptr<SDL_Surface> buffer;
		bool invalidate;
};

#endif

