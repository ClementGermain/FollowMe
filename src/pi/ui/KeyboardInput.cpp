#include <SDL/SDL.h>
#include <iostream>

void commandMotorFront(int direction);
void commandMotorBack(int direction);


// Create keyboard commands
void runKeyboardControl() {
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		return;
	SDL_Surface * screen;
	if(!(screen = SDL_SetVideoMode(200, 200, 24, SDL_HWSURFACE)))
		return;

	// Disable key repeat
	SDL_EnableKeyRepeat(0, 0);

	bool end = false;
	// [right, left, down, up]
	bool isKeyDown[4] = {false, false, false, false};
	int lastDirection[2] = {0, 0};

	while(!end) {
		SDL_Event event;
		SDL_WaitEvent(&event);

		bool isUp = false;
		int keycode = 0;

		switch(event.type) {
		case SDL_QUIT:
			end = true;
			break;
		case SDL_KEYUP:
			isUp = true;
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym) {
			case SDLK_ESCAPE:
				end = true;
				break;
			case SDLK_UP:
				keycode++;
			case SDLK_DOWN:
				keycode++;
			case SDLK_LEFT:
				keycode++;
			case SDLK_RIGHT:
				isKeyDown[keyCode] = !isUp;
				break;
			}
		}

		int newDirection[2] = {
			isKeyDown[3] - isKeyDown[2], // forward/backward
			isKeyDown[1] - isKeyDown[0] // turn left/right
		};
	
		// Update command motors back (go forward/backward)
		if(newDirection[0] != lastDirection[0]) {
			commandMotorBack(newDirection[0]);	
			lastDirection[0] = newDirection[0];
		}
		
		// Update command motor front (turn left/right)
		if(newDirection[1] != lastDirection[1]) {
			commandMotorFront(newDirection[1]);	
			lastDirection[1] = newDirection[1];
		}
	}
	SDL_Quit();
}

void commandMotorFront(int direction) {
	cout << "TODO motor front : " << (direction == 0 ? "stop" : (direction > 0 ? "turn left" : "turn right")) << endl;
}

void commandMotorBack(int direction) {
	cout << "TODO motors back : " << (direction == 0 ? "stop" : (direction > 0 ? "go forward" : "go backward")) << endl;
}

