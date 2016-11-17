#include "TextView.hpp"
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

using namespace std;

TextView::TextView(const std::string & text, int x, int y, int w, int h, bool center) :
	View(x,y),
	text(text),
	invalidate(true),
	center(center)
{
	int charSize = 8;
	if(w == 0) {
		w = charSize * text.size();
		h = charSize;
	}
	buffer = shared_ptr<SDL_Surface>(SDL_CreateRGBSurface(SDL_SWSURFACE, w,h,32,0,0,0,0), [](SDL_Surface * s){SDL_FreeSurface(s);});
}

void TextView::setText(const string & t) {
	text = t;
}

void TextView::draw(SDL_Surface * screen, bool needRedraw, bool updateScreen) {
	SDL_Surface * buffer = this->buffer.get();
	if(invalidate) {
		// clear background
		SDL_FillRect(buffer, NULL, 0xffffffff);

		int charSize = 8;
		int x = center ? (buffer->w-charSize*text.size()) / 2 : 0;
		int y = center ? (buffer->h-charSize) / 2 : 0;

		// draw text
		stringRGBA(buffer, x, y, text.c_str(), 0,0,0,255);

		invalidate = false;
	}

	if(needRedraw) {
		SDL_BlitSurface(buffer, NULL, screen, &screenPos);
		if(updateScreen)
			SDL_UpdateRect(screen, screenPos.x, screenPos.y, buffer->w, buffer->h);
	}
}
