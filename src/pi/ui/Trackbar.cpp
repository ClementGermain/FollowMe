#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <algorithm>
#include "Trackbar.hpp"

using namespace std;

Trackbar::Trackbar(float rangeMin, float rangeMax, int x, int y, int width, int height) :
	rangeMin(rangeMin), rangeMax(rangeMax),
	position(rangeMin),
	drawable(SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 0,0,0,0), [](SDL_Surface * s){SDL_FreeSurface(s);}),
	transparent(SDL_MapRGB(drawable->format, 255,255,255)),
	invalidate(true),
	borderSize(2)
{
	screenPos.x = x;
	screenPos.y = y;
	//SDL_SetColorKey(drawable.get(), SDL_SRCCOLORKEY, transparent);
}

Trackbar::~Trackbar() {
}

void Trackbar::setPosition(float pos) {
	float newPosition = min(max(rangeMin, pos), rangeMax);
	if(newPosition != position) {
		position = newPosition;
		invalidate = true;
	}
}

void Trackbar::draw(SDL_Surface * screen) {
	SDL_Surface * drawable = this->drawable.get();
	if(invalidate) {
		int cursorPos = borderSize + (drawable->w-borderSize*2) * (position-rangeMin) / (rangeMax-rangeMin);

		// clear
		SDL_FillRect(drawable, NULL, transparent);
		// border
		boxRGBA(drawable, 0,0, drawable->w, drawable->h, 0,0,0, 255);
		// before cursor
		boxRGBA(drawable, borderSize, borderSize, cursorPos-1, drawable->h-borderSize-1, 30, 200, 30, 255);
		// after cursor
		boxRGBA(drawable, cursorPos, borderSize, drawable->w-borderSize-1, drawable->h-borderSize-1, 220, 255, 220, 255);
		// cursor
		boxRGBA(drawable, cursorPos-(borderSize-1), 0, cursorPos+(borderSize-1)-1, drawable->h, 255, 20, 20, 255);
		
		invalidate = false;
	}

	SDL_BlitSurface(drawable, NULL, screen, &screenPos);
}

