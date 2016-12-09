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
		void close();
		bool isOpen();
	private:
		float SpeedCommand;
		void run();
		void initializeViews(ViewManager & mgr);
		void updateViews(ViewManager & mgr);

		std::thread * threadView;
		bool isThreadTerminated;

		SDL_Surface * screen;
		CPULoad cpuLoad;
};

#endif
