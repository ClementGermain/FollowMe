#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <algorithm>
#include "Trackbar.hpp"

using namespace std;

Trackbar::Trackbar(float rangeMin, float rangeMax, int x, int y, int width, int height) :
	View(x, y),
	rangeMin(rangeMin), rangeMax(rangeMax),
	position(rangeMin),
	drawable(SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 0,0,0,0), [](SDL_Surface * s){SDL_FreeSurface(s);}),
	invalidate(true),
	borderSize(2)
{
}

void Trackbar::setPosition(float pos) {
	float newPosition = min(max(rangeMin, pos), rangeMax);
	if(newPosition != position) {
		position = newPosition;
		invalidate = true;
	}
}

void Trackbar::draw(SDL_Surface * screen, bool needRedraw, bool updateScreen) {
	SDL_Surface * drawable = this->drawable.get();
	if(invalidate) {
		int cursorPos = borderSize + (drawable->w-borderSize*2) * (position-rangeMin) / (rangeMax-rangeMin);

		// border
		SDL_FillRect(drawable, NULL, 0x0);
		// before cursor
		boxRGBA(drawable, borderSize, borderSize, cursorPos-1, drawable->h-borderSize-1, 30, 200, 30, 255);
		// after cursor
		boxRGBA(drawable, cursorPos, borderSize, drawable->w-borderSize-1, drawable->h-borderSize-1, 220, 255, 220, 255);
		// cursor
		boxRGBA(drawable, cursorPos-(borderSize-1), 0, cursorPos+(borderSize-1)-1, drawable->h, 255, 20, 20, 255);
		
	}

	if(needRedraw || invalidate) {
		SDL_BlitSurface(drawable, NULL, screen, &screenPos);
		if(updateScreen)
			SDL_UpdateRect(screen, screenPos.x, screenPos.y, drawable->w, drawable->h);
	}
	invalidate = false;
}

