#include "ViewManager.hpp"
#include <SDL/SDL.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <cassert>
#include "view/Layout.hpp"

using namespace std;

ViewManager::ViewManager(TabView * tabView) : activeLayoutIndex(-1), invalidate(true), tabView(tabView) {

}

/** Create a new layout with the given name **/
Layout & ViewManager::createLayout(const string & name) {
	// Check if the name already exist
	assert(layouts.count(name) == 0);

	// Add the name in the list
	names.push_back(name);
	if(tabView != NULL)
		tabView->addTab(name);
	if(activeLayoutIndex == -1)
		switchToLayout(0);
	else
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
	int t = activeLayoutIndex + 1;
	if(t >= (int) names.size())
		t = 0;
	switchToLayout(t);
}

/** Switch to the previous layout */
void ViewManager::switchToPrevLayout() {
	assert(names.size() > 0);
	int t = activeLayoutIndex - 1;
	if(t < 0)
		t = (int) names.size()-1;
	switchToLayout(t);
}

/** Switch to the given layout, return true upon success, usually false means invalid number 'n' **/
bool ViewManager::switchToLayout(int n) {
	if(n < 0 || n >= (int)names.size())
		return false;
	if(activeLayoutIndex != n) {
		activeLayoutIndex = n;
		invalidate = true;
		updateWindowTitle();
		if(tabView != NULL)
			tabView->setSelectedTab(activeLayoutIndex);
	}
	return true;
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
		// Draw the tab view
		if(tabView != NULL)
			tabView->draw(screen, true, false);
		// Update the whole screen
		SDL_Flip(screen);
		invalidate = false;
	}
	else {
		// Draw the layout
		getActiveLayout().draw(screen, false, useLocalScreenUpdate);
		// Draw the tab view
		if(tabView != NULL)
			tabView->draw(screen, false, useLocalScreenUpdate);
		// Update the whole screen if required
		if(!useLocalScreenUpdate)
			SDL_Flip(screen);
	}
}
