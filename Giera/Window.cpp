#include "Window.h"
#include "Logger.h"
#include "TextureLoader.h"
#include "Texture.h"

using std::make_pair;
using std::make_shared;

Window::Window() :
	UIElement(Rect<real_pos_t> (0, 0, 800, 640))
{
	window = SDL_CreateWindow("Giera", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		800, 640, SDL_WINDOW_SHOWN);
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

pair<int, int> Window::getSize()
{
	SDL_Point windowSize;
	SDL_GetWindowSize(window, &windowSize.x, &windowSize.y);
	return std::make_pair(windowSize.x, windowSize.y);
}

double Window::getXToYRatio()
{
	SDL_Point windowSize;
	SDL_GetWindowSize(window, &windowSize.x, &windowSize.y);
	return (double)windowSize.x / (double)windowSize.y;
}

pair<int, int> Window::getMousePos()
{
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	return make_pair(mouseX, mouseY);
}

void Window::render(shared_ptr<Texture>& textureToDrawOn)
{
	renderUI();
}

void Window::renderUI()
{
	if (texture == nullptr) {
		texture = TextureLoader::makeUniColorTexture(getSize().first,
			getSize().second, { 0,0,0,0 });
	}
	else {
		texture->fillWithColor({ 0,0,0,0 });
	}
	for (auto& child : children) {
		child->render(texture);
	}
	auto screenTexture = make_shared<Texture>();
	texture->draw(*screenTexture, { 0,0,texture->getSize().first, texture->getSize().second },
		{ 0,0,getSize().first, getSize().second });
}

