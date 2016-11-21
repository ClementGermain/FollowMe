#include <SDL/SDL.h>
#include "Trackbar.hpp"

class Trackbar_Vertical: public Trackbar{
public:
  Trackbar_Vertical(float rangeMin, float rangeMax, int x, int y, int width=16, int height=170);
  void draw(SDL_Surface * screen, bool needRedraw=true, bool updateScreen=false);
};
