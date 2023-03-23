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
	Texture(Texture& texture) = delete;
	Texture(Texture&& texture) = delete;
    SDL_Texture* getTexture() const;
    void setTexture(SDL_Texture* texture);
    static SDL_Renderer* getRenderer();
	static void setRenderer(SDL_Renderer* renderer);
	void draw(Texture& target, SDL_Rect srcRect, SDL_Rect dstRect);
	void fillWithColor(SDL_Color color);
	pair<int, int> getSize();

private:
	static inline SDL_Renderer* renderer = NULL;
	SDL_Texture* texture;

};

