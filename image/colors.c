#include <SDL2/SDL.h>
#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL_image.h>
void gray_scale(SDL_Surface *surface){
	Uint32* pixels = surface->pixels;
	int a = SDL_LockSurface(surface);
	if(a==-1){
		errx(-1,"could not lock the surface for grayscale");
	}
	int w = surface->w;
	SDL_PixelFormat *format = surface->format;
	for(int i =0 ;i<surface->h;i++){
		for(int j = 0; j<w;j++){
			Uint32 pixel = *(pixels+i*w+j);
			Uint8 r;
			Uint8 g;
			Uint8 b;
			SDL_GetRGB(pixel,format, &r,&g,&b);
			pixels[i*surface->w+j]=SDL_MapRGB(format,0.2125*r,0.7154*g,0.0721*b);
		}
	}
	SDL_UnlockSurface(surface);
}
//transform all the pixels of a surface into grayscale

void black_and_white(SDL_Surface *surface){
	Uint32* pixels = surface->pixels;
	int a = SDL_LockSurface(surface);
	if(a==-1){
		errx(-1,"could not lock the surface for black and white");
	}
	SDL_PixelFormat *format = surface->format;
	int w = surface->w;
	for(int i =0 ;i<surface->h;i++){
		for(int j = 0; j<surface->w;j++){
			Uint32 pixel = *(pixels+i*w+j);
			Uint8 r;
			Uint8 g;
			Uint8 b;
			SDL_GetRGB(pixel,format, &r,&g,&b);
			if((r+g+b)>=127){
				pixels[i*surface->w+j]=SDL_MapRGB(format,255,255,255);
			}
			else {
				pixels[i*surface->w+j]=SDL_MapRGB(format,0,0,0);
			}
		}
	}
	SDL_UnlockSurface(surface);
}
Uint8 new_color (Uint8 c, int n){
	return (Uint8)(255*SDL_pow((double)c/255,n));
}
void contrast(SDL_Surface *surface){
	Uint32* pixels = surface->pixels;
	int a = SDL_LockSurface(surface);
	if(a==-1){
		errx(-1,"could not lock the surface for contrast");
	}
	SDL_PixelFormat *format = surface->format;
	int w = surface->w;
	for(int i =0 ;i<surface->h;i++){
		for(int j = 0; j<surface->w;j++){
			Uint32 pixel = *(pixels+i*w+j);
			Uint8 r;
			Uint8 g;
			Uint8 b;
			SDL_GetRGB(pixel,format, &r,&g,&b);
			r = new_color(r,3);
			g = new_color(g,3);
			b = new_color(b,3);
			pixels[i*w+j] = SDL_MapRGB(format,r,g,b);
		}
	}
	SDL_UnlockSurface(surface);
}

	
//transform all the pixels into black or white pixels*/
int main(int argc, char** argv)
{
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    if(SDL_Init(SDL_INIT_VIDEO)!=0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Window* window = SDL_CreateWindow("Surface_to_no_green", 0, 0, 0, 0,SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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

    contrast(surface);
	gray_scale(surface);
	black_and_white(surface);

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

