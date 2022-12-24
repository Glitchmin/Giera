#pragma once
#include <SDL.h>
class Window
{
public:
	Window();
	~Window();
	Window(Window& window) = delete;
	Window(Window&& window) = delete;

	SDL_Window* getWindow() const;
	SDL_Renderer* getRenderer() const;
	void updateRenderer();
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
};

