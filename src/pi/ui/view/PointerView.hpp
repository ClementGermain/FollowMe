#ifndef __POINTER_VIEW_HPP__
#define __POINTER_VIEW_HPP__

#include "View.hpp"
#include <SDL/SDL.h>

class PointerView : public View {
	public:
		PointerView(int fromX, int fromY, int toImageX, int toImageY, int pointedImageX, int pointedImageY);
		void draw(SDL_Surface * screen, bool needRedraw=true, bool updateScreen=false);
	private:
		int toX, toY;
};

#endif

