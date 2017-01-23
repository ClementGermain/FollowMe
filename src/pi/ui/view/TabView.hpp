#ifndef __TAB_VIEW_HPP__
#define __TAB_VIEW_HPP__

#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <memory>
#include "View.hpp"

#define NEXT_TAB	-1
#define PREV_TAB	-2
#define NO_CHANGE	-3

/**
 * An horizontal view to display a set of tabs. Can be associated to a ViewManager to display and change the current tabs.
 */
class TabView : public View {
public:
	TabView(int x, int y, int w, int h);
	//! Add a new named tabs
	void addTab(std::string const& name);
	//! Set the active tabs
	void setSelectedTab(int n);
	/** return the index of the tab newly selected, or NEXT_TAB, PREV_TAB or NO_CHANGE **/
	int handleEvent(SDL_Event & event);
	void draw(SDL_Surface * screen, bool needRedraw=true, bool updateScreen=false);
private:
	std::shared_ptr<SDL_Surface> buffer;
	std::vector<std::string> names;
	bool invalidate;
	int selectedTab;
	const int selectedBackgroundColor;
	const int selectedTextColor;
	const int unselectedBackgroundColor;
	const int unselectedTextColor;
};

#endif
