#ifndef __TOGGLE_BOX__
#define __TOGGLE_BOX__

#include "TextView.hpp"
#include <string>

using namespace std;

class ToggleBox : public TextView {
public: 
ToggleBox(const std::string & text_ok, const std::string & texte_ko, int x, int y, int w = 260, int h = 30);
void toggle_OK();
void toggle_KO();
void draw(SDL_Surface * screen, bool needRedraw, bool updateScreen);
protected:
int R,G,B ;
const std::string text_ok, text_ko;
};

#endif
