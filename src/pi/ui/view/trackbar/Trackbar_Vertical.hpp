#ifndef __TRACKBAR_VER__
#define __TRACKBAR_VER__

#include <SDL/SDL.h>
#include "Trackbar.hpp"

/**
 * Vertical trackbar with cursor to display a value in a range.
 */
class Trackbar_Vertical: public Trackbar{
public:
  Trackbar_Vertical(float rangeMin, float rangeMax, int x, int y, int width=16, int height=170, enum sens_remplissage _sens = NORMAL);
  void draw(SDL_Surface * screen, bool needRedraw=true, bool updateScreen=false);
};

#endif
