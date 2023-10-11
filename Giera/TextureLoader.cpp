#include <SDL_image.h>
#include "TextureLoader.h"
#include "Texture.h"
#include <filesystem>

using std::make_shared;
using std::to_string;

shared_ptr<Texture> TextureLoader::getTexturePtr(string_view textureName)
{
	return loadTexture(textureName);
}

shared_ptr<Texture> TextureLoader::getTextureCopy(string_view textureName)
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


void TextureLoader::setRenderer(SDL_Renderer* renderer)
{
	TextureLoader::renderer = renderer;
}

shared_ptr<Texture> TextureLoader::makeUniColorTexture(int sizeX, int sizeY, SDL_Color color)
{
	if (renderer == NULL) {
		Logger::logError("no renderer");
	}
	if (sizeX + sizeY == 0) {
		return nullptr;
	}
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	SDL_Texture* oldTarget = SDL_GetRenderTarget(renderer);

	SDL_Texture* newTx = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, sizeX, sizeY);
	SDL_SetTextureBlendMode(newTx, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_SetRenderTarget(renderer, newTx);
	SDL_RenderClear(renderer);

	SDL_SetRenderTarget(renderer, oldTarget);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	return make_shared<Texture>(newTx);
}

shared_ptr<Texture> TextureLoader::makeTextTexture(FontTypes fontType, int fontSize, string_view text, SDL_Color color)
{
	TTF_Font* font = loadTTF_Font(fontType, fontSize);
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.data(), color);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	
	SDL_SetSurfaceBlendMode(textSurface, SDL_BLENDMODE_BLEND);
	SDL_Texture* newSDLTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_SetTextureBlendMode(newSDLTexture, SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(textSurface);

	return make_shared<Texture>(newSDLTexture);
}

shared_ptr <Texture> TextureLoader::loadTexture(string_view textureName)
{
	if (textureMap.find(textureName.data()) != textureMap.end()) {
		return textureMap[textureName.data()];
	}
	SDL_Surface* surface = IMG_Load(textureName.data());
	if (surface == NULL) {
		Logger::logError("couldn't find ", std::filesystem::absolute(textureName));
	}
	if (renderer == NULL) {
		Logger::logError("no renderer");
	}

	SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);
	SDL_Texture* newSDLTexture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_SetTextureBlendMode(newSDLTexture, SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(surface);
	auto texture = make_shared<Texture>(newSDLTexture);
	textureMap[textureName.data()] = texture;
	return texture;
}

TTF_Font* TextureLoader::loadTTF_Font(FontTypes fontType, int fontSize)
{
	string fontKey = to_string((int)fontType) + "_" + to_string(fontSize);
	if (fontMap.find(fontKey) != fontMap.end()) {
		return fontMap[fontKey];
	}
	TTF_Font* font = TTF_OpenFont(fontPaths[(int)fontType].c_str(), fontSize);
	if (font == NULL) {
		Logger::logError("couldn't find ", std::filesystem::absolute(fontPaths[(int)fontType]));
	}
	fontMap[fontKey] = font;
	return font;
}
