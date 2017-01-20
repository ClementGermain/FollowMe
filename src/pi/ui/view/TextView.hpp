#ifndef __TEXT_VIEW_HPP__
#define __TEXT_VIEW_HPP__

#include "View.hpp"
#include <memory>
#include <SDL/SDL.h>
#include <string>

/**
 * Text view to display a text.
 */
class TextView : public View {
public:
  TextView(const std::string & text, int x, int y, int w=0, int h=0, bool center=false);
  void draw(SDL_Surface * screen, bool needRedraw=true, bool updateScreen=false);
  void setText(const std::string & t);
protected:
  std::shared_ptr<SDL_Surface> buffer;
  std::string text;
  bool invalidate;
  bool center;
};

#endif

