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

