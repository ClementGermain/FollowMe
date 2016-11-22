#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>
#include <opencv2/opencv.hpp>
#include "ImageView.hpp"
#include <iostream>

ImageView::ImageView(int x, int y, int w, int h) :
	View(x, y),
	buffer(SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0,0,0,0), [](SDL_Surface * s){SDL_FreeSurface(s);}),
	invalidate(true)
{

}

void ImageView::setImage(SDL_Surface * image, ScaleType mode) {
	switch(mode) {
		case ImageView::NORMAL:
			
			SDL_Rect testRect{	(Sint16)((buffer->w - image->w) / 2),
								(Sint16)((buffer->h - image->h) / 2),
								(Uint16)image->w,
								(Uint16)image->h };
			
			SDL_BlitSurface(image, 
							NULL, 
							buffer.get(),
							&testRect
							);

			break;
		case ImageView::FITXY:
			if (image->w != 0 && image->h != 0)
			{
				
				SDL_Surface * zoomed = rotozoomSurfaceXY(image, 0.0, buffer->w / image->w, buffer->h / image->h, 0);
			
				SDL_BlitSurface(zoomed,
								NULL,
								buffer.get(),
								NULL);

				SDL_FreeSurface(zoomed);
			}
			break;
	}
	invalidate = true;
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
	if(needRedraw || invalidate) {
		SDL_BlitSurface(buffer, NULL, screen, &screenPos);
		if(updateScreen)
			SDL_UpdateRect(screen, screenPos.x, screenPos.y, buffer->w, buffer->h);
	}

	invalidate = false;
}
