#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <string>
#include "View.hpp"
#include "ToggleBox.hpp"

ToggleBox::ToggleBox(const std::string & text_ok_, const std::string & text_ko_, int x_, int y_, int w_, int h_) :
  TextView(text_ok_, x_, y_, w_, h_, true), 
  text_ok(text_ok_), text_ko(text_ko_), 
  R(255), G(255), B(255)
{
  toggle_OK();
}

void ToggleBox::toggle_OK(){
  R = 0;
  G = 150;
  B = 0;
  text = text_ok;
  invalidate = true;
}

void ToggleBox::toggle_KO(){
  R = 150;
  G = 0;
  B = 0;
  text = text_ko;
  invalidate = true;
}

void ToggleBox::draw(SDL_Surface * screen, bool needRedraw, bool updateScreen){
  SDL_Surface * buffer = this->buffer.get();
	if(invalidate) {
	  // clear background
	  SDL_FillRect(buffer, NULL, 0xffffffff);
	  roundedBoxRGBA(buffer, 0, 0, buffer->w, buffer->h, 5, R, G, B,255);
	  
	  int charSize = 8;
	  int x = center ? (buffer->w-charSize*text.size()) / 2 : 0;
	  int y = center ? (buffer->h-charSize) / 2 : 0;
	  
	  // draw text
	  stringRGBA(buffer, x, y, text.c_str(), 255,255,255,255);
	  
	}
	
	if(needRedraw || invalidate) {
	  SDL_BlitSurface(buffer, NULL, screen, &screenPos);
	  if(updateScreen)
	    SDL_UpdateRect(screen, screenPos.x, screenPos.y, buffer->w, buffer->h);
	}
	
	invalidate = false;
}
