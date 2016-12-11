#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <string>
#include "View.hpp"
#include "StateBox.hpp"

StateBox::StateBox(int x_, int y_, int w_, int h_):
  TextView("",x_, y_, w_, h_, true),
  nb_states(0),
  current_state(0)
{
}

void StateBox::add_state(const std::string & text_, int R_, int G_, int B_){
  list_states[nb_states].text = text_;
  list_states[nb_states].R = R_;
  list_states[nb_states].G = G_;
  list_states[nb_states].B = B_;
  nb_states++;
}

void StateBox::set_state(int state_){
  current_state = state_;
  text = list_states[current_state].text;
  invalidate = true;
}

void StateBox::draw(SDL_Surface * screen, bool needRedraw, bool updateScreen){
  int red = list_states[current_state].R;
  int green = list_states[current_state].G;
  int blue = list_states[current_state].B;
  SDL_Surface * buffer = this->buffer.get();
	if(invalidate) {
	  // clear background
	  SDL_FillRect(buffer, NULL, 0xffffffff);
	  roundedBoxRGBA(buffer, 0, 0, buffer->w, buffer->h, 5, red, green, blue,255);
	  
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
