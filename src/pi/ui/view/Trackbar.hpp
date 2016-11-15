#ifndef __TRACKBAR_HPP__
#define __TRACKBAR_HPP__

#include <SDL/SDL.h>
#include <memory>
#include "View.hpp"

class Trackbar : public View {
	public:
		Trackbar(float rangeMin, float rangeMax, int x, int y, int width=170, int height=16);
		void setPosition(float pos);
		void draw(SDL_Surface * screen, bool needRedraw=true, bool updateScreen=false);
	private:
		float rangeMin, rangeMax;
		float position;

		std::shared_ptr<SDL_Surface> drawable;
		Uint32 transparent;
		bool invalidate;
		int borderSize;
};

#endif
