#ifndef __PLOTS_VIEW_HPP__
#define __PLOTS_VIEW_HPP__

#include <SDL/SDL.h>
#include <memory>
#include <vector>
#include "View.hpp"

class PlotsView : public View {
	public:
		PlotsView(int x, int y, int w, int h);
		void draw(SDL_Surface * screen, bool needRedraw=true, bool updateScreen=false);
		void addPlot(float value);
		void clearPlots();

	private:
		std::shared_ptr<SDL_Surface> buffer;
		bool invalidate;
		int plotCount;
		int plotIndexStart;
		const int maxPlotCount;
		std::vector<float> plotValues;
};

#endif
