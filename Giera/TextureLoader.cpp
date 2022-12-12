#include <SDL_image.h>
#include "TextureLoader.h"

using std::make_shared;

shared_ptr<Texture> TextureLoader::getTexturePtr(string& textureName)
{
	return loadTexture(textureName);
}

shared_ptr<Texture> TextureLoader::getTextureCopy(string& textureName)
{
	int h, w;
	SDL_QueryTexture(loadTexture(textureName)->getTexture(), NULL, NULL, &w, &h);
	SDL_Texture* newCopy = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
	SDL_SetTextureBlendMode(newCopy, SDL_BLENDMODE_BLEND);
	SDL_Texture* oldTarget = SDL_GetRenderTarget(renderer);
	Uint8 r, g, b, a;
	SDL_SetRenderTarget(renderer, newCopy);
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_Rect dstRect = { 0,0,w,h };

	SDL_RenderCopy(renderer, loadTexture(textureName)->getTexture(), NULL, &dstRect);

	SDL_SetRenderTarget(renderer, oldTarget);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	return make_shared<Texture>(newCopy);
}

shared_ptr <Texture> TextureLoader::loadTexture(string& textureName)
{
	if (textureMap.find(textureName) != textureMap.end()) {
		return textureMap[textureName];
	}
	SDL_Surface* surface = IMG_Load(textureName.c_str());
	if (surface == NULL) {
		Logger::logError("couldn't find ", textureName);
	}
	if (renderer == NULL) {
		Logger::logError("no renderer");
	}

	SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);
	SDL_Texture* newSDLTexture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_SetTextureBlendMode(newSDLTexture, SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(surface);
	auto texture = make_shared<Texture>(newSDLTexture);
	textureMap[textureName] = texture;
	return texture;
}

void TextureLoader::setRenderer(SDL_Renderer* renderer)
{
	TextureLoader::renderer = renderer;
}

