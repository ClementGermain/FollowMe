#ifndef __MAIN_VIEW_HPP__
#define __MAIN_VIEW_HPP__

#include <string>
#include <thread>
#include <SDL/SDL.h>
#include "CPULoad.hpp"
#include "ViewManager.hpp"

/**
 * The MainView is the graphical user interface (GUI) that display information about the car and the raspberry pi program, it runs in a thread.
 * It creates a windows, displays many views, and handle input event.
 **/
class MainView {
	public:
		MainView();
		~MainView();
		void open();
		void close();
		bool isOpen();
	private:
		void run();
		void initializeViews(ViewManager & mgr);
		void updateViews(ViewManager & mgr);

		std::thread * threadView;
		bool isThreadTerminated;

		SDL_Surface * screen;
		CPULoad cpuLoad;
};

#endif
