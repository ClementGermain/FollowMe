#ifndef __MAIN_VIEW_HPP__
#define __MAIN_VIEW_HPP__

#include <vector>
#include <thread>
#include <SDL/SDL.h>
#include "../car/Camera.hpp"
#include "view/Digital.hpp"
#include "view/Trackbar.hpp"
#include "CPULoad.hpp"

class MainView {
	public:
		MainView(Camera & camera);
		~MainView();
		void open();
		bool isOpen();
	private:
		void run();
		void drawStaticViews();
		void initializeViews();
		void updateViews();
		void drawPointerLine(int x, int y, int x2, int y2, SDL_Rect & carPos);
		std::thread * threadView;
		bool isThreadTerminated;

		SDL_Surface * screen;
		SDL_Surface * car;
		SDL_Surface * arrowKeys;
		std::vector<Trackbar> trackbarMotors;
		std::vector<Digital> digitalValues;
		CPULoad cpuLoad;
		Camera & camera;
		bool showCamera;
};

#endif
