#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL.h>
#include "Trackbar_Vertical.hpp"

Trackbar_Vertical::Trackbar_Vertical(float rangeMin, float rangeMax, int x, int y, int width, int height) :
  Trackbar(rangeMin, rangeMax, x, y, width, height) 
{
}

void Trackbar_Vertical::draw(SDL_Surface * screen, bool needRedraw, bool updateScreen) {
	SDL_Surface * drawable = this->drawable.get();
	if(invalidate) {
	  int cursorPos = drawable->h/2 - ( borderSize + (drawable->h-borderSize*2) * (position-rangeMin) / (rangeMax-rangeMin));

		// border
		SDL_FillRect(drawable, NULL, 0x0);
		// before cursor
		boxRGBA(drawable, borderSize, drawable->h-borderSize-1 ,drawable->w-borderSize-1, cursorPos-1, 30, 200, 30, 255); // Flash Green zone thought the cursor
		// after cursor
		boxRGBA(drawable, borderSize, borderSize, drawable->w-borderSize-1, cursorPos, 220, 255, 220, 255); // Back color of the trackbar
		// cursor
		boxRGBA(drawable, 0,cursorPos-(borderSize-1), drawable->h, cursorPos+(borderSize-1)-1, 255, 20, 20, 255);
	}

	if(needRedraw || invalidate) {
		SDL_BlitSurface(drawable, NULL, screen, &screenPos);
		if(updateScreen)
			SDL_UpdateRect(screen, screenPos.x, screenPos.y, drawable->w, drawable->h);
	}
	invalidate = false;
}
