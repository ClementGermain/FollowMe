#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL.h>
#include "Trackbar_Horizontal.hpp"

Trackbar_Horizontal::Trackbar_Horizontal(float rangeMin, float rangeMax, int x, int y, int width, int height) :
  Trackbar(rangeMin, rangeMax, x, y, width, height) 
{
}

void Trackbar_Horizontal::draw(SDL_Surface * screen, bool needRedraw, bool updateScreen) {
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
