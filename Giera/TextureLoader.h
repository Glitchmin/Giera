#pragma once
#include <unordered_map>
#include <memory>
#include <SDL.h>
#include <string_view>
#include "Texture.h"
#include "Logger.h"

using std::unordered_map;
using std::shared_ptr;
using std::string_view;

class Texture;
class TextureLoader
{
public:
	static shared_ptr<Texture> getTexturePtr(string_view textureName);
	static shared_ptr<Texture> getTextureCopy(string_view textureName);
    static void setRenderer(SDL_Renderer* renderer);
	static shared_ptr<Texture> makeUniColorTexture(int x, int y, SDL_Color color);
private:
	static inline unordered_map <string, shared_ptr <Texture> > textureMap{};
	static inline SDL_Renderer* renderer = NULL;
	static shared_ptr <Texture> loadTexture(string_view textureName);
	friend class Texture;
};

