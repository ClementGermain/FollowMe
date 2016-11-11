#ifndef __TRACKBAR_HPP__
#define __TRACKBAR_HPP__

#include <SDL/SDL.h>
#include <memory>

class Trackbar {
	public:
		Trackbar(float rangeMin, float rangeMax, int x, int y, int width=170, int height=15);
		~Trackbar();
		void setPosition(float pos);
		void draw(SDL_Surface * screen);
	private:
		float rangeMin, rangeMax;
		float position;

		std::shared_ptr<SDL_Surface> drawable;
		Uint32 transparent;
		bool invalidate;
		int borderSize;
		SDL_Rect screenPos;
};

#endif
