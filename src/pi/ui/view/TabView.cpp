#include "TabView.hpp"
#include <SDL/SDL_gfxPrimitives.h>

using namespace std;


TabView::TabView(int x, int y, int w, int h) :
	View(x,y),
	buffer(SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0,0,0,0), [](SDL_Surface * s){SDL_FreeSurface(s);}),
	invalidate(true),
	selectedTab(-1),
	selectedBackgroundColor(0xffffffff),
	selectedTextColor(0x000000ff),
	unselectedBackgroundColor(0xbbbbbbff),
	unselectedTextColor(0x484848ff)
{

}

void TabView::addTab(string const& name) {
	names.push_back(name);
	invalidate = true;
}

void TabView::setSelectedTab(int n) {
	selectedTab = n;
	invalidate = true;
}

int TabView::handleEvent(SDL_Event & event) {
	int t = NO_CHANGE;
	switch(event.type) {
	case SDL_KEYDOWN: {
		int k = event.key.keysym.sym;
		if(SDLK_1 <= k && k <= SDLK_9)
			t = k - SDLK_1;
		else if(SDLK_KP1 <= k && k <= SDLK_KP9)
			t = k - SDLK_KP1;
		else if(k == SDLK_TAB) {
			if((SDL_GetModState() & KMOD_LCTRL) == KMOD_LCTRL)
				t = PREV_TAB;
			else
				t = NEXT_TAB;
		}
		} break;
	case SDL_MOUSEBUTTONDOWN: {
		int x = event.button.x;
		int y = event.button.y;
		if(screenPos.x <= x && x < screenPos.x + buffer->w
		 		&& screenPos.y <= y && y < screenPos.y + buffer->h)
		{
			t = (x - screenPos.x) * (int) names.size() / buffer->w;
		}
		} break;
	}

	return t;
}

void TabView::draw(SDL_Surface * screen, bool needRedraw, bool updateScreen) {
	SDL_Surface * buffer = this->buffer.get();
	
	if(invalidate) {
		int charSize = 8;
		int nTab = names.size();
		int hOffset = 4;

		SDL_FillRect(buffer, NULL, unselectedBackgroundColor);
		boxColor(buffer, 0, 0, buffer->w, hOffset-1, selectedBackgroundColor);
		boxColor(buffer, 0, hOffset, buffer->w, buffer->h, unselectedBackgroundColor);
		
		for(int i = 0; i < nTab; i++) {
			int x0 = buffer->w * i / nTab;
			int x1 = buffer->w * (i+1) / nTab - 1;
			int txtColor = unselectedTextColor;
			string txt = to_string(i+1)+"-"+names[i];
			if(selectedTab == i) {
				roundedBoxColor(buffer, x0, -hOffset*2, x1, buffer->h, hOffset*2, selectedBackgroundColor);
				txtColor = selectedTextColor;
			}
			int px = x0 + ((x1-x0) - txt.size()*charSize)/2;
			int py = hOffset + ((buffer->h-hOffset) - charSize) / 2;
			stringColor(buffer, px, py, txt.c_str(), txtColor);
		}
	}	

	if(needRedraw || invalidate) {
		SDL_BlitSurface(buffer, NULL, screen, &screenPos);
		if(updateScreen)
			SDL_UpdateRect(screen, screenPos.x, screenPos.y, buffer->w, buffer->h);
	}

	invalidate = false;
}
