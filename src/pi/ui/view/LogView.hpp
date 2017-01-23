#ifndef __LOG_VIEW_HPP__
#define __LOG_VIEW_HPP__

#include <SDL/SDL.h>
#include <memory>
#include "utils/Log.hpp"
#include "View.hpp"

/**
 * Multiple line text view to display the last data writen in the Log (utils/Log.hpp).
 * Display the lines with the matching color.
 */
class LogView : public View {
	public:
		LogView(int x, int y, int w, int h);
		void draw(SDL_Surface * screen, bool needRedraw=true, bool updateScreen=false);
	private:
		std::shared_ptr<SDL_Surface> buffer;
		LogStream::Cursor prevEndCursor;
};

#endif

