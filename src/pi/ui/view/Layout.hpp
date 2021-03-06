#ifndef __LAYOUT_HPP__
#define __LAYOUT_HPP__

#include <SDL/SDL.h>
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>
#include "View.hpp"
#include "Digital.hpp"
#include "trackbar/Trackbar.hpp"
#include "ImageView.hpp"
#include "KeyboardInput.hpp"
#include "LogView.hpp"
#include "ToggleBox.hpp"
#include "StateBox.hpp"

/**
 * A view container. The views inside it are identified by a name.
 * The position of this view is not used and does not influe on the position of the contained views.
 **/
class Layout : public View {
	public:
		Layout();
		/** Add a view to the layout, the layout will automatically delete when destroyed
		 * if the view is not used by an other layout. **/
		void addView(const std::string & name, View * v);
		View * getView(const std::string & name);
		Digital & getDigitalView(const std::string & name);
		Trackbar & getTrackbarView(const std::string & name);
		ToggleBox & getToggleBoxView(const std::string & name);
		StateBox & getStateBoxView(const std::string & name);
		ImageView & getImageView(const std::string & name);
		LogView & getLogView(const std::string & name);
		KeyboardInput & getKeyboardView(const std::string & name);
		void draw(SDL_Surface * screen, bool needRedraw=true, bool updateScreen=false);
	private:
		std::unordered_map<std::string, std::shared_ptr<View>> views;
		std::vector<std::string> viewOrder;
};


#endif
