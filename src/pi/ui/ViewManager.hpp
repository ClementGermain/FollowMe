#ifndef __VIEW_MANAGER_HPP__
#define __VIEW_MANAGER_HPP__

#include <unordered_map>
#include <string>
#include <vector>
#include "view/Layout.hpp"

class ViewManager {
	public:
		ViewManager();
		/** Create a new layout with the given name **/
		Layout & createLayout(const std::string & name);
		/** Return a reference to the currently active layout **/
		Layout & getActiveLayout();
		/** Return the name of the currently active layout **/
		std::string const& getActiveLayoutName() const;
		/** Indicate whether the given layout is currently active or not **/
		bool isActive(const std::string & name) const;
		/** Return a reference to the layout matching with the given name **/
		Layout & getLayout(const std::string & name);
		/** Switch to the next layout */
		void switchToNextLayout();
		/** Switch to the previous layout */
		void switchToPrevLayout();
		/** Update the title of the window according to the active layout **/
		void updateWindowTitle();
		/** Draw the currently active layout on the screen.
		 * The 'useLocalScreenUpdate' could improve performance by updating only changed area of the screen **/
		void drawActiveLayout(SDL_Surface * screen, bool useLocalScreeUpdate);
	private:
		std::unordered_map<std::string, Layout> layouts;
		std::vector<std::string> names;
		int activeLayoutIndex;
		bool invalidate;
};

#endif

