#include "Window.h"
#include "Logger.h"
#include "TextureLoader.h"
#include "Texture.h"
Window::Window()
{
	window = SDL_CreateWindow("Giera", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 640, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		Logger::logError("Couldn't create window", SDL_GetError());
	}
	else {
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		TextureLoader::setRenderer(renderer);
		Texture::setRenderer(renderer);
	}
}

Window::~Window()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}

SDL_Window* Window::getWindow() const
{
    return window;
}

SDL_Renderer* Window::getRenderer() const
{
    return renderer;
}

void Window::updateRenderer()
{
	SDL_RenderPresent(renderer);
}

