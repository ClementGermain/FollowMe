#ifndef __VIEW_MANAGER_HPP__
#define __VIEW_MANAGER_HPP__

#include <unordered_map>
#include <string>
#include <vector>
#include "view/Layout.hpp"

class ViewManager {
	public:
		ViewManager();
		Layout & createLayout(const std::string & name);
		Layout & getActiveLayout();
		std::string const& getActiveLayoutName() const;
		bool isActive(const std::string & name) const;
		Layout & getLayout(const std::string & name);
		void switchToNextLayout();
		void switchToPrevLayout();
		void drawActiveLayout(SDL_Surface * screen);
	private:
		std::unordered_map<std::string, Layout> layouts;
		std::vector<std::string> names;
		int activeLayoutIndex;
		bool invalidate;
};

#endif

