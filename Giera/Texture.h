#pragma once
#include "TextureLoader.h"
#include <string>
#include <SDL.h>

using std::string;

class Texture
{
public:
	Texture(string& textureName);
	Texture();
	Texture(SDL_Texture* texture);
	~Texture();
    SDL_Texture* getTexture() const;
    void setTexture(SDL_Texture* texture);
private:
	SDL_Texture* texture;
};

