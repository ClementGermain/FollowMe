#ifndef __POINTER_VIEW_HPP__
#define __POINTER_VIEW_HPP__

#include "View.hpp"
#include <SDL/SDL.h>

/**
 * A deprecated view to point a position in an ImageView. It appears as a line with a small circle at the extrmity (image side).
 * this view does not have internal image buffer.
 */
class PointerView : public View {
	public:
		//! Create pointer view. Line starts at (fromX, fromY) in screen space, ends at (toImageX, toImageU) in image space.
		PointerView(int fromX, int fromY, int toImageX, int toImageY, int pointedImageX, int pointedImageY);
		void draw(SDL_Surface * screen, bool needRedraw=true, bool updateScreen=false);
	private:
		int toX, toY;
};

#endif

