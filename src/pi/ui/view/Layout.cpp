#include "Layout.hpp"
#include <SDL/SDL.h>
#include <memory>
#include <unordered_map>
#include <string>

using namespace std;

Layout::Layout() : View(0,0) {

}

void Layout::addView(const string & name, View * view) {
	if(views.emplace(name, shared_ptr<View>(view)).second) {
		// the view has been succesfully added, now register its position
		viewOrder.push_back(name);
	}
}

View * Layout::getView(const string & name) {
	return views.at(name).get();
}

Digital & Layout::getDigitalView(const string & name) {
	return *((Digital*) getView(name));
}

Trackbar & Layout::getTrackbarView(const string & name) {
	return *((Trackbar*) getView(name));
}

ToggleBox & Layout::getToggleBoxView(const string & name){
  return *((ToggleBox*) getView(name));
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
	if(needRedraw)
		SDL_FillRect(screen, NULL, 0xffffffff);

	for(string & v : viewOrder)
		getView(v)->draw(screen, needRedraw, !needRedraw && updateScreen);

	if(updateScreen)
		SDL_Flip(screen);
}


