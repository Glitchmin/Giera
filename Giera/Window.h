#pragma once
#include <SDL.h>
#include <utility>
using std::pair;
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
	pair<int, int> getSize();
	double getXToYRatio();
	pair<int, int> getMousePos();
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
};

