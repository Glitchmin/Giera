#pragma once
#include "TextureLoader.h"
#include <string>
#include <SDL.h>

using std::string;
using std::pair;


class Texture
{
public:
	Texture(string& textureName);
	Texture();
	Texture(SDL_Texture* texture);
	~Texture();
    SDL_Texture* getTexture() const;
    void setTexture(SDL_Texture* texture);
    SDL_Renderer* getRenderer() const;
    void setRenderer(SDL_Renderer* renderer);
	void draw(Texture& target, SDL_Rect srcRect, SDL_Rect dstRect);
	pair<int, int> getSize();

private:
	static inline SDL_Renderer* renderer = NULL;
	SDL_Texture* texture;

};

