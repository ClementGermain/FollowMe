#ifndef __MAIN_VIEW_HPP__
#define __MAIN_VIEW_HPP__

#include <vector>
#include <thread>
#include <SDL/SDL.h>
#include "Digital.hpp"
#include "Trackbar.hpp"
#include "CPULoad.hpp"

class MainView {
	public:
		MainView();
		~MainView();
		void open();
	private:
		void run();
		std::thread * threadView;

		SDL_Surface * screen;
		SDL_Surface * car;
		SDL_Surface * arrowKeys;
		std::vector<Trackbar> trackbarMotors;
		std::vector<Digital> digitalValues;
		CPULoad cpuLoad;
};

#endif
