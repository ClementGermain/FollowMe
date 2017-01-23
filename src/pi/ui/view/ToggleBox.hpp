#ifndef __TOGGLE_BOX__
#define __TOGGLE_BOX__

#include "TextView.hpp"
#include <string>

using namespace std;

/**
 * Toggle bos (2 states view)  to display a boolean value with 2 texts and 2 colors.
 */
class ToggleBox : public TextView {
	public: 
		ToggleBox(const std::string & text_ok, const std::string & texte_ko, int x, int y, int w = 260, int h = 30);
		//! Toggle state
		void toggle(bool newstate);
		//! Change state to OK
		void toggle_OK();
		//! Change state to KO
		void toggle_KO();
		void draw(SDL_Surface * screen, bool needRedraw, bool updateScreen);
	protected:
		int R,G,B ;
		bool state;
		const std::string text_ok, text_ko;
};

#endif
