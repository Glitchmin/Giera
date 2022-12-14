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


SDL_Renderer* Texture::getRenderer() const
{
    return renderer;
}

void Texture::setRenderer(SDL_Renderer* renderer)
{
    this->renderer = renderer;
}

void Texture::draw(Texture& target, SDL_Rect srcRect, SDL_Rect dstRect)
{
	if (renderer == NULL) {
		Logger::logError("no renderer");
	}
	SDL_Texture* oldTarget = SDL_GetRenderTarget(renderer);
	SDL_SetRenderTarget(renderer, target.getTexture());
	SDL_RenderCopy(renderer, getTexture(), &srcRect, &dstRect);
	SDL_SetRenderTarget(renderer, oldTarget);
}

pair<int, int> Texture::getSize()
{
	int h, w;
	SDL_QueryTexture(getTexture(), NULL, NULL, &w, &h);
	return std::make_pair<>(h,w);
}
