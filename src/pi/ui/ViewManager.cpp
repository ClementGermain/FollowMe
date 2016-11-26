#include "ViewManager.hpp"
#include <SDL/SDL.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <cassert>
#include "view/Layout.hpp"

using namespace std;

ViewManager::ViewManager() : activeLayoutIndex(-1), invalidate(true) {

}

/** Create a new layout with the given name **/
Layout & ViewManager::createLayout(const string & name) {
	// Check if the name already exist
	assert(layouts.count(name) == 0);

	// Add the name in the list
	names.push_back(name);
	if(activeLayoutIndex == -1)
		activeLayoutIndex = 0;
	updateWindowTitle();

	// Create and return a new layout
	return layouts[name];
}

/** Return a reference to the currently active layout **/
Layout & ViewManager::getActiveLayout() {
	return layouts.at(getActiveLayoutName());
}

/** Return the name of the currently active layout **/
string const& ViewManager::getActiveLayoutName() const {
	assert(activeLayoutIndex >= 0);
	assert(activeLayoutIndex < (int) names.size());
	return names[activeLayoutIndex];
}

/** Indicate whether the given layout is currently active or not **/
bool ViewManager::isActive(const string & name) const {
	return getActiveLayoutName().compare(name) == 0;
}

/** Return a reference to the layout matching with the given name **/
Layout & ViewManager::getLayout(const string & name) {
	// check if the name exists
	assert(layouts.count(name) != 0);
	// Return the layout
	return layouts.at(name);
}

/** Switch to the next layout */
void ViewManager::switchToNextLayout() {
	assert(names.size() > 0);
	if(++activeLayoutIndex == (int) names.size())
		activeLayoutIndex = 0;
	invalidate = true;
	updateWindowTitle();
}

/** Switch to the previous layout */
void ViewManager::switchToPrevLayout() {
	assert(names.size() > 0);
	if(--activeLayoutIndex < 0)
		activeLayoutIndex = (int) names.size()-1;
	invalidate = true;
	updateWindowTitle();
}

/** Update the title of the window according to the active layout **/
void ViewManager::updateWindowTitle() {
	// Make a title with format: "FollowMe - Layout name (Layout Position/Layout Count)"
	char title[256];
	sprintf(title, "FollowMe - %s (%d/%lu)", getActiveLayoutName().c_str(), activeLayoutIndex+1, names.size());	
	// Commit title
	SDL_WM_SetCaption(title, NULL);
}

/** Draw the currently active layout on the screen.
 * The 'useLocalScreenUpdate' could improve performance by updating only changed area of the screen **/
void ViewManager::drawActiveLayout(SDL_Surface * screen, bool useLocalScreenUpdate) {
	if(invalidate) {
		// Fill background with white
		SDL_FillRect(screen, NULL, 0xffffffff);
		// Draw tle layout
		getActiveLayout().draw(screen, true, false);
		// Update the whole screen
		SDL_Flip(screen);
		invalidate = false;
	}
	else {
		// Draw the layout
		getActiveLayout().draw(screen, false, useLocalScreenUpdate);
		// Update the whole screen if required
		if(!useLocalScreenUpdate)
			SDL_Flip(screen);
	}
}
