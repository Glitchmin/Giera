#pragma once
#include "UIElement.h"
using std::pair;
class Window: public UIElement
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
	void render(shared_ptr <Texture>& textureToDrawOn) override;
	void renderUI();
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
};

