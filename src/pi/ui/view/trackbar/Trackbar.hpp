#ifndef __TRACKBAR_HPP__
#define __TRACKBAR_HPP__

#include <SDL/SDL.h>
#include <memory>
#include "../View.hpp"

enum sens_remplissage {NORMAL, INVERSE, CENTREE, ENABLE};

class Trackbar : public View {
public:
  Trackbar(float rangeMin, float rangeMax, int x, int y, int width, int height, enum sens_remplissage sens_ = NORMAL);
  void setPosition(float pos);
  void setInnerBounds(float min, float max);
protected:
  float rangeMin, rangeMax;
  float position;
  bool hasInnerBounds;
  float innerBoundMin, innerBoundMax;
  std::shared_ptr<SDL_Surface> drawable;
  Uint32 transparent;
  bool invalidate;
  int borderSize;
  enum sens_remplissage sens;
};

#endif
