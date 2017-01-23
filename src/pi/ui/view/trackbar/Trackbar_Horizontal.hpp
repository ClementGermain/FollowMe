#ifndef __TRACKBAR_HOR__
#define __TRACKBAR_HOR__

#include "Trackbar.hpp"

/**
 * Horizontal trackbar with cursor to display a value in a range.
 */
class Trackbar_Horizontal: public Trackbar{
public:
  Trackbar_Horizontal(float rangeMin, float rangeMax, int x, int y, int width=170, int height=16, enum sens_remplissage _sens = NORMAL);
  void draw(SDL_Surface * screen, bool needRedraw, bool updateScreen);
};

#endif
