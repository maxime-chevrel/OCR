#include <SDL2/SDL.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL_image.h>
#include <math.h>
SDL_Surface * rotation(double angle,SDL_Surface* surface){
	angle = angle * M_PI /180;
	int w = surface -> w;
	int h = surface ->h;
	int center_X = h/2;
	int center_Y = w/2;
	SDL_PixelFormat* format = surface -> format;
	SDL_Surface * res= SDL_CreateRGBSurface(0,w,h,32,0,0,0,0);
	if(res == NULL){
		errx(-1,"could not create surface in rotation");
	}
	Uint32 * pixels_base = surface->pixels;
	Uint32 * pixels_res = res->pixels; 
	for(int i = 0;i<h;i++){
		for(int j = 0;j<w;j++){
			int X = cos(angle)*((i-center_X)
					-sin(angle)*(j-center_Y))+center_X;
			int Y = cos(angle)*(j-center_Y)
					+sin(angle)*(i-center_X)+center_Y;
			if(X>0 && X<h && Y>0 && Y<w){
				pixels_res[i*w+j] = pixels_base[X*w+Y];
			}
			else{
				pixels_res[i*w+j] = SDL_MapRGB(format,0,0,0);
			}
		}
	}
	return res;
}
int main(int argc, char** argv)
{
    if (argc != 3)
        errx(EXIT_FAILURE, "Not enough args");

    if(SDL_Init(SDL_INIT_VIDEO)!=0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Window* window = SDL_CreateWindow("rotated", 0, 0, 0, 0,SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Surface* t = IMG_Load(argv[1]);
    if (t == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface *surface = SDL_ConvertSurfaceFormat(t, SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(t);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_SetWindowSize(window,  surface->w, surface->h);

    surface = rotation(atof(argv[2]),surface);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_RenderCopy(renderer,texture,NULL,NULL);
    SDL_RenderPresent(renderer);

    SDL_Event event;
    while (1)
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
            case SDL_QUIT:
                SDL_FreeSurface(surface);
                SDL_DestroyTexture(texture);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return EXIT_SUCCESS;

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                        SDL_RenderCopy(renderer,texture,NULL,NULL);
                        SDL_RenderPresent(renderer);
                }
                break;
        }
    }
    return EXIT_SUCCESS;
}
