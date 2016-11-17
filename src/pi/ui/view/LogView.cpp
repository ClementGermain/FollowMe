#include "LogView.hpp"
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>


LogView::LogView(int x, int y, int w, int h) :
	View(x, y),
	buffer(SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0,0,0,0), [](SDL_Surface * s){SDL_FreeSurface(s);}),
	prevEndCursor(Log.getCursor(true))
{
}

void LogView::draw(SDL_Surface * screen, bool needRedraw, bool updateScreen) {
	SDL_Surface * buffer = this->buffer.get();
	bool invalidate = Log.hasNext(prevEndCursor);
	// if new lines have been added: redraw
	if(invalidate) {
		// clear background
		SDL_FillRect(buffer, NULL, 0x0);

		int charSize = 8, hLine = charSize * 3 / 2;
		int xMargin = 2;
		int ly = buffer->h - hLine - (hLine-charSize)/2;
		int charPerLine = (buffer->w-2*xMargin) / charSize;
		LogStream::Cursor cur(Log.getCursor(false));
		prevEndCursor = cur;
	
		// draw text line while visible and existing
		while(Log.hasPrevious(cur) && ly + charSize > 0) {
			// get previous log line
			LogLine const& l(Log.readPrevious(cur));
			int color = l.getColor();

			// draw over as many row as needed
			int offset = l.str.size() - l.str.size() % charPerLine;
			while(offset >= 0) {
				stringColor(buffer, xMargin, ly, l.str.substr(offset, offset+charPerLine).c_str(), color);
				ly -= hLine;
				offset -= charPerLine;
			}
		}
	}

	if(needRedraw || invalidate) {
		SDL_BlitSurface(buffer, NULL, screen, &screenPos);
		if(updateScreen)
			SDL_UpdateRect(screen, screenPos.x, screenPos.y, buffer->w, buffer->h);
	}
}
