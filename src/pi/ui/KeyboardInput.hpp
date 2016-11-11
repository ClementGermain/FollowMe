#ifndef __KEYBOARD_INPUT_HPP__
#define __KEYBOARD_INPUT_HPP__

#include <vector>
#include <SDL/SDL.h>

class KeyboardInput {
	public:
		KeyboardInput(void (*commandMotorFront)(int), void (*commandMotorBack)(int), int x, int y, int w, int h);
		~KeyboardInput();
		bool handleEvent(SDL_Event & event);
		void toggleEnabled();
		bool isEnabled() { return enabled; }
		void draw(SDL_Surface * s);

		static const int GoForward = 1;
		static const int GoBackward = -1;
		static const int TurnLeft = 1;
		static const int TurnRight = -1;
		static const int Idle = 0;

	private:
		bool enabled;
		std::vector<bool> isKeyPressed;
		std::vector<int> lastStates;
		void (*commandMotorFront)(int);
		void (*commandMotorBack)(int);
		SDL_Surface * arrowsBMP;
		SDL_Surface * buffer;
		bool invalidate;
		SDL_Rect screenPos;
};

#endif

