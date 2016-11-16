#include <SDL/SDL.h>
#include <opencv2/opencv.hpp>
#include "ImageView.hpp"


ImageView::ImageView(int x, int y, int w, int h) :
	View(x, y),
	buffer(SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0,0,0,0), [](SDL_Surface * s){SDL_FreeSurface(s);})
{

}

void ImageView::setImage(SDL_Surface * image, ScaleType mode) {
	switch(mode) {
		case ImageView::NORMAL:
			SDL_BlitSurface(image, NULL, buffer.get(), NULL);
			break;
			// TODO center and resize image
	}
}

void ImageView::setImage(cv::Mat * mat, ScaleType mode) {
	if(mat != NULL) {
		IplImage image(*mat);
		SDL_Surface * img = SDL_CreateRGBSurfaceFrom((void*)image.imageData,
				image.width,
				image.height,
				image.depth * image.nChannels,
				image.widthStep,
				0xff0000, 0x00ff00, 0x0000ff, 0
				);

		setImage(img, mode);

		SDL_FreeSurface(img);
	}
}

void ImageView::draw(SDL_Surface * screen, bool needRedraw, bool updateScreen) {
	SDL_Surface * buffer = this->buffer.get();
	// the buffer has been updated in setImage
	//
	if(needRedraw) {
		SDL_BlitSurface(buffer, NULL, screen, &screenPos);
		if(updateScreen)
			SDL_UpdateRect(screen, screenPos.x, screenPos.y, buffer->w, buffer->h);
	}
}
