#ifndef __PLOTS_VIEW_HPP__
#define __PLOTS_VIEW_HPP__

#include <SDL/SDL.h>
#include <memory>
#include <vector>
#include "View.hpp"

/**
 * Plots view to display continuous data as a chronogram.
 * The scale and the range are automatically adjusted to have human readable values.
 */
class PlotsView : public View {
	public:
		PlotsView(int x, int y, int w, int h);
		void draw(SDL_Surface * screen, bool needRedraw=true, bool updateScreen=false);
		/** Add a new value to the graph. The value is appended to the end,
		 * and all the previous data are consequently shifted to the left in the view. **/
		void addPlot(float value);
		/**
		 * Remove all the plots. The graph will be empty and the scale reinitialized **/
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
