#ifndef __DIGITAL_HPP__
#define __DIGITAL_HPP__

#include <SDL/SDL.h>
#include <memory>
#include <string>
#include "View.hpp"

/**
 * Similar to TextView but display a numerical value with pre-formatted string.
 */
class Digital : public View {
	public:
		/** The string format must be like printf formatted string (see stdio.h).
		 * It will be used like : printf(format, value);
		 * example: format="Distance: %f meters"; the view will display something like "Distance 42.00000 meters"
		 */
		Digital(std::string const& format, int x, int y, int w, int h=15, bool centerTextHoriz=true, bool centerTextVert=true);
		void draw(SDL_Surface * screen, bool needRedraw=true, bool updateScreen=false);
		void setValue(float value);

	private:
		std::string format;
		float value;
		std::shared_ptr<SDL_Surface> buffer;
		bool invalidate;
		bool centerHoriz;
		bool centerVert;
};

#endif
