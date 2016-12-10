#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL.h>
#include "Trackbar_Vertical.hpp"

Trackbar_Vertical::Trackbar_Vertical(float rangeMin, float rangeMax, int x, int y, int width, int height, enum sens_remplissage sens_) :
  Trackbar(rangeMin, rangeMax, x, y, width, height, sens_) 
{
}

void Trackbar_Vertical::draw(SDL_Surface * screen, bool needRedraw, bool updateScreen) {
	SDL_Surface * drawable = this->drawable.get();
	if(invalidate) {
		int cursorPos = drawable->h - (borderSize + (drawable->h-borderSize*2) * (position-rangeMin) / (rangeMax-rangeMin));

		// border
		SDL_FillRect(drawable, NULL, 0x0);

		// after cursor
		boxRGBA(drawable, borderSize, borderSize, drawable->w-borderSize-1, drawable->h-borderSize-1, 220, 255, 220, 255); // Back color of the trackbar
		
		// before cursor // Flash Green zone thought the cursor
		switch (sens){
		case NORMAL:
		  boxRGBA(drawable, borderSize, drawable->h-borderSize-1 ,drawable->w-borderSize-1, cursorPos-1, 30, 200, 30, 255);
		  break;
		case INVERSE:
		  boxRGBA(drawable, borderSize, borderSize ,drawable->w-borderSize-1, cursorPos-1, 30, 200, 30, 255);
		  break;
		case CENTREE:
		  boxRGBA(drawable, borderSize, (drawable->h-borderSize-1)/2 ,drawable->w-borderSize-1, cursorPos-1, 30, 200, 30, 255);
		  break;
		case ENABLE:
		  break;
		}

		// cursor
		boxRGBA(drawable, 0,cursorPos-(borderSize-1), drawable->h, cursorPos+(borderSize-1)-1, 255, 20, 20, 255);

		// Inner bounds (black cursors)
		if(hasInnerBounds) {
			int boundMinPos = drawable->h - (borderSize + (drawable->h-borderSize*2) * (innerBoundMin-rangeMin) / (rangeMax-rangeMin));
			int boundMaxPos = drawable->h - (borderSize + (drawable->h-borderSize*2) * (innerBoundMax-rangeMin) / (rangeMax-rangeMin));

			boxRGBA(drawable, 0,boundMinPos-(borderSize/2-1), drawable->h, boundMinPos+(borderSize/2-1)-1, 100, 100, 100, 255);
			boxRGBA(drawable, 0,boundMaxPos-(borderSize/2-1), drawable->h, boundMaxPos+(borderSize/2-1)-1, 100, 100, 100, 255);
		}
	}

	if(needRedraw || invalidate) {
		SDL_BlitSurface(drawable, NULL, screen, &screenPos);
		if(updateScreen)
			SDL_UpdateRect(screen, screenPos.x, screenPos.y, drawable->w, drawable->h);
	}
	invalidate = false;
}
