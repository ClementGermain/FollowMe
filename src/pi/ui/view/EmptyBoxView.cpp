#include "EmptyBoxView.hpp"
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

EmptyBoxView::EmptyBoxView(int x, int y, int w, int h) :
	View(x, y),
	w(w), h(h)
{

}

void EmptyBoxView::draw(SDL_Surface * screen, bool needRedraw, bool updateScreen) {
	if(needRedraw) {
		roundedRectangleRGBA(screen, screenPos.x, screenPos.y, screenPos.x+w, screenPos.y+h, 5, 0,0,0,255);
		if(updateScreen)
			SDL_UpdateRect(screen, screenPos.x, screenPos.y, w, h);
	}
}
