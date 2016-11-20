#ifndef __MAIN_VIEW_HPP__
#define __MAIN_VIEW_HPP__

#include <string>
#include <thread>
#include <SDL/SDL.h>
#include "CPULoad.hpp"
#include "ViewManager.hpp"

class MainView {
	public:
		MainView();
		~MainView();
		void open();
		bool isOpen();
	private:
		void run();
		void drawStaticViews();
		void initializeViews(ViewManager & mgr);
		void updateViews(ViewManager & mgr);
		void drawPointerLine(int x, int y, int x2, int y2, SDL_Rect & carPos);

		std::thread * threadView;
		bool isThreadTerminated;

		SDL_Surface * screen;
		CPULoad cpuLoad;
};

#endif
