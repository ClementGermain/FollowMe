#ifndef __TRACKBAR_HPP__
#define __TRACKBAR_HPP__

#include <SDL/SDL.h>
#include <memory>
#include "../View.hpp"

class Trackbar : public View {
public:
  Trackbar(float rangeMin, float rangeMax, int x, int y, int width, int height);
  void setPosition(float pos);
protected:
  float rangeMin, rangeMax;
  float position;
  std::shared_ptr<SDL_Surface> drawable;
  Uint32 transparent;
  bool invalidate;
  int borderSize;
};

#endif
