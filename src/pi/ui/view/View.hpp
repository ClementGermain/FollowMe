#ifndef __VIEW_HPP__
#define __VIEW_HPP__

#include <SDL/SDL.h>

class View {
	public:
		View(int x, int y);
		virtual void draw(SDL_Surface * screen, bool needRedraw, bool updateScreen) = 0;
	protected:
		SDL_Rect screenPos;
		
};

#endif

