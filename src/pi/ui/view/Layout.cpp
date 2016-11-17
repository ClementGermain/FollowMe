#include "Layout.hpp"
#include <SDL/SDL.h>
#include <memory>
#include <unordered_map>
#include <string>

using namespace std;

Layout::Layout() : View(0,0) {

}

void Layout::addView(const string & name, View * view) {
	views.emplace(name, shared_ptr<View>(view));
}

View * Layout::getView(const string & name) {
	return views[name].get();
}

Digital & Layout::getDigitalView(const string & name) {
	return *((Digital*) getView(name));
}

Trackbar & Layout::getTrackbarView(const string & name) {
	return *((Trackbar*) getView(name));
}

ImageView & Layout::getImageView(const string & name) {
	return *((ImageView*) getView(name));
}

LogView & Layout::getLogView(const string & name) {
	return *((LogView*) getView(name));
}

KeyboardInput & Layout::getKeyboardView(const string & name) {
	return *((KeyboardInput*) getView(name));
}

void Layout::draw(SDL_Surface * screen, bool needRedraw, bool updateScreen) {
	for(auto & it : views)
		it.second.get()->draw(screen, needRedraw, updateScreen);
}


