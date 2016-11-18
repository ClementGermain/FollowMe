#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <cstdio>
#include <string>
#include "Digital.hpp"

using namespace std;


Digital::Digital(string const& format, int x, int y, int w, int h, bool centerTextHoriz, bool centerTextVert) :
	View(x,y),
	format(format),
	value(0),
	buffer(SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0,0,0,0), [](SDL_Surface * s){SDL_FreeSurface(s);}),
	invalidate(true),
	centerHoriz(centerTextHoriz),
	centerVert(centerTextVert)
{
}

void Digital::setValue(float value) {
	if(this->value != value) {
		this->value = value;
		invalidate = true;
	}
}

void Digital::draw(SDL_Surface * screen, bool needRedraw, bool updateScreen) {
	SDL_Surface * buffer = this->buffer.get();
	if(invalidate) {
		int charSize = 8;
		int maxChar = buffer->w / charSize + 1;
		
		// Clear background
		SDL_FillRect(buffer, NULL, 0xffffffff);
		
		// Format string
		char text[maxChar+1];
		int len = snprintf(text, maxChar, format.c_str(), value);

		int x = centerHoriz ? (buffer->w-charSize*len) / 2 : 0;
		int y = centerVert ? (buffer->h-charSize) / 2 : 0;
		stringRGBA(buffer, x, y, text, 0,0,0,255);

	}
	
	if(needRedraw || invalidate) {
		SDL_BlitSurface(buffer, NULL, screen, &screenPos);
		if(updateScreen)
			SDL_UpdateRect(screen, screenPos.x, screenPos.y, buffer->w, buffer->h);
	}

	invalidate = false;
}
