#include "PlotsView.hpp"
#include <SDL/SDL_gfxPrimitives.h>
#include <cmath>
#include <cstdio>

using namespace std;

PlotsView::PlotsView(int x, int y, int w, int h) : 
	View(x, y),
	buffer(SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0,0,0,0), [](SDL_Surface * s){SDL_FreeSurface(s);}),
	invalidate(true),
	plotCount(0),
	plotIndexStart(0),
	maxPlotCount(w),
	plotValues(maxPlotCount)
{

}

void PlotsView::clearPlots() {
	plotIndexStart = 0;
	plotCount = 0;
	invalidate = true;
}

void PlotsView::addPlot(float value) {
	plotValues[(plotIndexStart+plotCount)%maxPlotCount] = value;
	if(plotCount != maxPlotCount)
		plotCount++;
	else
		plotIndexStart = (plotIndexStart+1)%plotCount;
	invalidate = true;
}

void PlotsView::draw(SDL_Surface * screen, bool needRedraw, bool updateScreen) {
	SDL_Surface * buffer = this->buffer.get();
	if(invalidate) {
		// Clear background
		SDL_FillRect(buffer, NULL, 0xffffff);

		if(plotCount > 0) {
			float minValue = plotValues[plotIndexStart], maxValue = plotValues[plotIndexStart];
			// get min and max values in plots data
			for(int i = 1; i < plotCount; i++) {
				minValue = min(plotValues[(i+plotIndexStart)%maxPlotCount], minValue);
				maxValue = max(plotValues[(i+plotIndexStart)%maxPlotCount], maxValue);
			}

			// Find best y range
			float minY = 0;
			float scale = maxValue > 0 ? pow(10, floor(log10(maxValue))) : 1;
			// maxY is smallest value greater than maxValue that looks like 10^n (n is integer)
			float maxY = scale * ceil(maxValue / scale);

			float rangeWidthY = maxY-minY;
			float yFactor = buffer->h / rangeWidthY;
			float yOffset = minY;

			// Find best vertical graduation with human readable values
			const int minLinesCount = 3;
			float horizontalLinesScale;
			if(ceil(rangeWidthY / (scale * 2)) > minLinesCount)
				horizontalLinesScale = scale * 2;
			else if(ceil(rangeWidthY / (scale * 1)) > minLinesCount)
				horizontalLinesScale = scale * 1;
			else if(ceil(rangeWidthY / (scale / 2)) > minLinesCount)
				horizontalLinesScale = scale / 2;
			else
				horizontalLinesScale = scale / 5;

			// Draw graduation lines with text value
			float yStart = horizontalLinesScale * ceil(minY / horizontalLinesScale);
			for(float y = yStart; y < maxY; y += horizontalLinesScale) {
				int py = buffer->h - (int) ((y-yOffset) * yFactor);
				int charSize = 8;
				hlineRGBA(buffer, 0, buffer->w, py, 180, 180, 180, 255);
				char txt[20]; sprintf(txt, "%.1e", y);
				stringRGBA(buffer, 5, py-charSize-1,  txt, 128,128,128,255);
			}

			// Draw plots with continuous line
			float prev;
			float current = plotValues[plotIndexStart];
			float next = plotValues[plotIndexStart];
			for(int i = 0; i < plotCount; i++) {
				prev = current;
				current = next;
				next = i+1 < plotCount ? plotValues[(plotIndexStart+i+1)%maxPlotCount] : next;

				vlineRGBA(buffer, i, buffer->h - (int) ((current-yOffset) * yFactor), buffer->h - (int) ((min(min(current,prev),next)-yOffset) * yFactor), 42,42,42, 255);
			}
		}
	}

	if(needRedraw || invalidate) {
		SDL_BlitSurface(buffer, NULL, screen, &screenPos);
		if(updateScreen)
			SDL_UpdateRect(screen, screenPos.x, screenPos.y, buffer->w, buffer->h);
	}

	invalidate = false;
}
