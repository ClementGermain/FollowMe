#ifndef __DIGITAL_HPP__
#define __DIGITAL_HPP__

#include <SDL/SDL.h>
#include <memory>
#include <string>

class Digital {
	public:
		Digital(std::string const& format, int x, int y, int w, int h=15, bool centerTextHoriz=true, bool centerTextVert=true);
		void draw(SDL_Surface *screen);
		void setValue(float value);

	private:
		std::string format;
		float value;
		std::shared_ptr<SDL_Surface> buffer;
		bool invalidate;
		bool centerHoriz;
		bool centerVert;
		SDL_Rect screenPos;
};

#endif
