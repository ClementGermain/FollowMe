#include "ViewManager.hpp"
#include <SDL/SDL.h>
#include <string>
#include <unordered_map>
#include <vector>
#include "view/Layout.hpp"

using namespace std;

ViewManager::ViewManager() : activeLayoutIndex(-1), invalidate(true) {

}

Layout & ViewManager::createLayout(const string & name) {
	// Check if the name already exist
	if(layouts.count(name) == 0) {
		names.push_back(name);
		if(activeLayoutIndex == -1)
			activeLayoutIndex = 0;
	}
	// return a new Layout or the existing one that matches with the given name
	return layouts[name];
}

Layout & ViewManager::getActiveLayout() {
	return layouts.at(names[activeLayoutIndex]);
}

string const& ViewManager::getActiveLayoutName() const {
	return names[activeLayoutIndex];
}

bool ViewManager::isActive(const string & name) const {
	return getActiveLayoutName().compare(name) == 0;
}

Layout & ViewManager::getLayout(const string & name) {
	return layouts.at(name);
}

void ViewManager::switchToNextLayout() {
	if(++activeLayoutIndex == (int) names.size())
		activeLayoutIndex = 0;
	invalidate = true;
	updateWindowTitle();
}

void ViewManager::switchToPrevLayout() {
	if(--activeLayoutIndex < 0)
		activeLayoutIndex = (int) names.size()-1;
	invalidate = true;
	updateWindowTitle();
}

void ViewManager::updateWindowTitle() {
	char title[256];
	sprintf(title, "FollowMe - %s (%d/%lu)", getActiveLayoutName().c_str(), activeLayoutIndex+1, names.size());	
	SDL_WM_SetCaption(title, NULL);
}

void ViewManager::drawActiveLayout(SDL_Surface * screen, bool useLocalScreeUpdate) {
	if(invalidate) {
		SDL_FillRect(screen, NULL, 0xffffffff);
		getActiveLayout().draw(screen, true, false);
		SDL_Flip(screen);
		invalidate = false;
	}
	else {
		getActiveLayout().draw(screen, false, useLocalScreeUpdate);
		if(!useLocalScreeUpdate)
			SDL_Flip(screen);
	}
}
