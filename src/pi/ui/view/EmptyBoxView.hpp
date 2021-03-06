#ifndef __EMPTY_BOX_VIEW_HPP__
#define __EMPTY_BOX_VIEW_HPP__

#include "View.hpp"
#include <SDL/SDL.h>

/**
 * Simple view to display a box to decorate the screen.
 * This view does not have an image buffer.
 */
class EmptyBoxView : public View {
	public:
		EmptyBoxView(int x, int y, int w, int h);
		void draw(SDL_Surface * screen, bool needRedraw=true, bool updateScreen=false);
	protected:
		int w, h;
};

#endif

