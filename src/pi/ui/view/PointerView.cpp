#include "PointerView.hpp"
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <algorithm>

using namespace std;


PointerView::PointerView(int fromX, int fromY, int toImageX, int toImageY, int pointedImageX, int pointedImageY) :
	View(fromX, fromY),
	toX(pointedImageX+toImageX),
	toY(pointedImageY+toImageY)
{

}

void PointerView::draw(SDL_Surface * screen, bool needRedraw, bool updateScreen) {
	if(needRedraw) {
		aalineRGBA(screen, screenPos.x, screenPos.y, toX, toY, 20,20,200,255);
		int radius = 3;
		filledCircleRGBA(screen, toX, toY, radius, 20,20,200,255);
		if(updateScreen)
			SDL_UpdateRect(screen, min((int)screenPos.x, toX-radius), min((int)screenPos.y, toY-radius), abs((int)screenPos.x-toX)+radius, abs((int)screenPos.y-toY)+radius);
	}
}
