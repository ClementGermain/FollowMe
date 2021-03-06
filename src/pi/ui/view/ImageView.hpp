#ifndef __IMAGE_VIEW_HPP__
#define __IMAGE_VIEW_HPP__

#include <SDL/SDL.h>
#include <memory>
#include "View.hpp"
#include <opencv2/opencv.hpp>

/**
 * Display an image (SDL_Surface or cv::Mat).
 * Can draw image with different scale type:
 * 		NORMAL: the image is centered but not rescaled.
 * 		FITXY: the image is scaled in x and y to perfectly fit the view.
 */
class ImageView : public View {
	public:
		enum ScaleType { NORMAL, FITXY };
		ImageView(int x, int y, int h, int w);
		// Draw the image in the view, the given image must be free by the caller
		void setImage(SDL_Surface * image, ScaleType mode = ImageView::NORMAL);
		void setImage(cv::Mat * img, ScaleType mode = ImageView::NORMAL);
		void draw(SDL_Surface * screen, bool needRedraw=true, bool updateScreen=false);
	private:
		std::shared_ptr<SDL_Surface> buffer;
		bool invalidate;
		
};

#endif

