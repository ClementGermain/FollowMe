#ifndef __LOG_VIEW_HPP__
#define __LOG_VIEW_HPP__

#include <SDL/SDL.h>
#include "utils/Log.hpp"
#include "View.hpp"

class LogView : public View {
	public:
		LogView(int x, int y, int w, int h);
		void draw(SDL_Surface * screen, bool needRedraw=true, bool updateScreen=false);
	private:
		SDL_Surface * buffer;
		LogStream::Cursor prevEndCursor;
};

#endif

