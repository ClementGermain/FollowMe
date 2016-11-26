#ifndef __LOG_VIEW_HPP__
#define __LOG_VIEW_HPP__

#include <SDL/SDL.h>
#include "../utils/Log.hpp"

class LogView {
	public:
		LogView(int x, int y, int w, int h);
		void draw(SDL_Surface * screen);
	private:
		SDL_Surface * buffer;
		LogStream::Cursor prevEndCursor;
		SDL_Rect screenPos;
};

#endif

