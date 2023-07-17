#include "Texture.h"

Texture::Texture(string& textureName)
{
	this->texture = TextureLoader::loadTexture(textureName)->getTexture();
}

Texture::Texture()
{
	this->texture = nullptr;
}

Texture::Texture(SDL_Texture* texture)
{
	this->texture = texture;
}

Texture::~Texture()
{
	SDL_DestroyTexture(texture);
}

SDL_Texture* Texture::getTexture() const
{
	return texture;
}

void Texture::setTexture(SDL_Texture* texture)
{
	SDL_DestroyTexture(this->texture);
	this->texture = texture;
}


SDL_Renderer* Texture::getRenderer()
{
	return renderer;
}

void Texture::setRenderer(SDL_Renderer* renderer)
{
	Texture::renderer = renderer;
}

void Texture::draw(Texture& target, optional<SDL_Rect> srcRect, optional<SDL_Rect> dstRect)
{
	if (renderer == NULL) {
		Logger::logError("no renderer");
	}
	SDL_Texture* oldTarget = SDL_GetRenderTarget(renderer);
	SDL_SetRenderTarget(renderer, target.getTexture());
	SDL_Rect* srcRect_p = srcRect.has_value() ? &(*srcRect) : NULL;
	SDL_Rect* dstRect_p = dstRect.has_value() ? &(*dstRect) : NULL;
	SDL_RenderCopy(renderer, texture, srcRect_p, dstRect_p);
	SDL_SetRenderTarget(renderer, oldTarget);
}
void Texture::fillWithColor(SDL_Color color)
{
	if (renderer == NULL) {
		Logger::logError("no renderer");
	}
	SDL_Texture* oldTarget = SDL_GetRenderTarget(renderer);
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_SetRenderTarget(renderer, texture);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_SetRenderTarget(renderer, oldTarget);
}

pair<int, int> Texture::getSize()
{
	int h, w;
	SDL_QueryTexture(getTexture(), NULL, NULL, &w, &h);
	return std::make_pair<>(w, h);
}

