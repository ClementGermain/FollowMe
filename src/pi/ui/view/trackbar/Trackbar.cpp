#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <algorithm>
#include "Trackbar.hpp"

using namespace std;

Trackbar::Trackbar(float rangeMin, float rangeMax, int x, int y, int width, int height, enum sens_remplissage sens_) :
	View(x, y),
	rangeMin(rangeMin), rangeMax(rangeMax),
	position(rangeMin),
	drawable(SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 0,0,0,0), [](SDL_Surface * s){SDL_FreeSurface(s);}),
	invalidate(true),
	borderSize(2),
	sens(sens_)
{
}

void Trackbar::setPosition(float pos) {
	float newPosition = min(max(rangeMin, pos), rangeMax);
	if(newPosition != position) {
		position = newPosition;
		invalidate = true;
	}
}

