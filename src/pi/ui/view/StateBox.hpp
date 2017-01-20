#ifndef __STATE_BOX__
#define __STATE_BOX__

#include "TextView.hpp"
#include <string>

using namespace std;

#define nb_max_states 8
/**
 * A multi-state box to display discrete states with text and color.
 */
class StateBox : public TextView {
	public: 
		StateBox(int x_, int y_, int w_ = 260, int h_ = 30);
		//! Add a new state with associated text and color
		void add_state(const std::string & text, int R_, int G_, int B_);
		//! Set the active state
		void set_state(int state_);
		void draw(SDL_Surface * screen, bool needRedraw, bool updateScreen);

	private:
		struct state{
			std::string text;
			int R,G,B;
		};

	protected:
		state list_states[nb_max_states];
		int nb_states;
		int current_state; //index of the table list_states
};

#endif
