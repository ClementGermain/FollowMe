#ifndef __VIEW_HPP__
#define __VIEW_HPP__

#include <SDL/SDL.h>

/**
 * Simple abstract to implement graphical widget for the GUI.
 * It might uses a local image buffer to store the previous rendering, and redraw in the buffer only if its state has changed.
 */
class View {
	public:
		/** Create a view with position (x, y) on the screen. **/
		View(int x, int y);
		virtual ~View();
		/** Draw the view on the given surface. For optimization, the view is not necessarly redraw (except if asked). It must also update its area on the screen if asked. */
		virtual void draw(SDL_Surface * screen, bool needRedraw=true, bool updateScreen=false) = 0;
	protected:
		SDL_Rect screenPos;
		
};

#endif

