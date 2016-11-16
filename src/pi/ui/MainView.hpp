#ifndef __MAIN_VIEW_HPP__
#define __MAIN_VIEW_HPP__

#include <string>
#include <unordered_map>
#include <thread>
#include <memory>
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
		View & getView(const std::string & name);
		Digital & getDigitalView(const std::string & name);
		Trackbar & getTrackbarView(const std::string & name);
		void addView(const std::string & name, View * v);

		std::thread * threadView;
		bool isThreadTerminated;

		SDL_Surface * screen;
		SDL_Surface * car;
		SDL_Surface * arrowKeys;
		std::unordered_map<std::string, std::shared_ptr<View>> views;
		CPULoad cpuLoad;
		Camera & camera;
		bool showCamera;
};

#endif
