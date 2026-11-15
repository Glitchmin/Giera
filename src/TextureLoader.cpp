#include <SDL_image.h>
#include "TextureLoader.h"
#include "Texture.h"
#include <filesystem>
#include <algorithm>
#include <cmath>

using std::make_shared;
using std::to_string;

shared_ptr<Texture> TextureLoader::getTexturePtr(string_view textureName)
{
	return loadTexture(textureName);
}

shared_ptr<Texture> TextureLoader::getTextureCopy(string_view textureName)
{
	int h, w;
	SDL_QueryTexture(loadTexture(textureName)->getSDLTexture(), NULL, NULL, &w, &h);
	SDL_Texture* newCopy = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

	SDL_SetTextureBlendMode(newCopy, SDL_BLENDMODE_BLEND);
	SDL_Texture* oldTarget = SDL_GetRenderTarget(renderer);
	Uint8 r, g, b, a;
	SDL_SetRenderTarget(renderer, newCopy);
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_Rect dstRect = { 0,0,w,h };

	SDL_RenderCopy(renderer, loadTexture(textureName)->getSDLTexture(), NULL, &dstRect);

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
		Logger::logWarning("sizeX and sizeY are 0");
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

shared_ptr<Texture> TextureLoader::makeCircleTexture(int diameter, SDL_Color color)
{
	diameter = std::max(2, diameter);
	SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, diameter, diameter, 32, SDL_PIXELFORMAT_RGBA32);
	if (surface == nullptr) {
		return makeUniColorTexture(diameter, diameter, color);
	}
	SDL_LockSurface(surface);
	const double r = diameter / 2.0;
	for (int y = 0; y < diameter; y++) {
		Uint32* row = (Uint32*)((Uint8*)surface->pixels + y * surface->pitch);
		for (int x = 0; x < diameter; x++) {
			double dx = x + 0.5 - r, dy = y + 0.5 - r;
			double d = std::sqrt(dx * dx + dy * dy);
			// dark outline ring, so the dot reads on grass and on dirt
			bool inside = d <= r - 0.5;
			bool outline = d > r - 0.5 - std::max(1.0, r * 0.25);
			SDL_Color c = !inside ? SDL_Color{ 0, 0, 0, 0 } : outline ? SDL_Color{ 20, 20, 20, 200 } : color;
			row[x] = SDL_MapRGBA(surface->format, c.r, c.g, c.b, c.a);
		}
	}
	SDL_UnlockSurface(surface);
	SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);
	SDL_Texture* sdlTexture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_SetTextureBlendMode(sdlTexture, SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(surface);
	return make_shared<Texture>(sdlTexture);
}

shared_ptr<Texture> TextureLoader::makeTextTexture(FontTypes fontType, int fontSize, string_view text, SDL_Color color)
{
	TTF_Font* font = loadTTF_Font(fontType, fontSize);
	if (font == nullptr) {
		return makeUniColorTexture(1, 1, { 0, 0, 0, 0 });
	}
	string textStr(text); // string_view::data() is not guaranteed to be null-terminated
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textStr.c_str(), color);
	if (textSurface == nullptr) {
		Logger::logError("TTF_RenderText_Solid failed:", TTF_GetError());
		return makeUniColorTexture(1, 1, { 0, 0, 0, 0 });
	}
	// (The old code created the SDL texture twice and leaked the first one on every call.)
	SDL_SetSurfaceBlendMode(textSurface, SDL_BLENDMODE_BLEND);
	SDL_Texture* newSDLTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_SetTextureBlendMode(newSDLTexture, SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(textSurface);

	return make_shared<Texture>(newSDLTexture);
}

shared_ptr <Texture> TextureLoader::loadTexture(string_view textureName)
{
	string name(textureName);
	auto cached = textureMap.find(name);
	if (cached != textureMap.end()) {
		return cached->second;
	}
	if (renderer == NULL) {
		Logger::logError("no renderer");
	}
	SDL_Surface* surface = IMG_Load(name.c_str());
	if (surface == NULL) {
		// Missing/broken asset: use a visible magenta placeholder instead of
		// passing a NULL surface on (which crashed later).
		Logger::logError("couldn't load texture", name, ":", IMG_GetError());
		auto placeholder = makeUniColorTexture(16, 16, { 255, 0, 255, 255 });
		textureMap[name] = placeholder;
		return placeholder;
	}

	// Normalise to 32-bit RGBA. 24-bit images without alpha (e.g. the terrain tiles,
	// which are BMPs) became X888 textures that rendered black when blended on
	// Android's OpenGL ES renderer (Direct3D on Windows hid the problem).
	if (surface->format->format != SDL_PIXELFORMAT_RGBA32) {
		SDL_Surface* converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
		if (converted) {
			SDL_FreeSurface(surface);
			surface = converted;
		}
	}
	SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);
	SDL_Texture* newSDLTexture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_SetTextureBlendMode(newSDLTexture, SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(surface);
	auto texture = make_shared<Texture>(newSDLTexture);
	textureMap[name] = texture;
	Logger::logDebug("loaded texture ", name, texture->getSize().first, texture->getSize().second);
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
		Logger::logError("couldn't load font", fontPaths[(int)fontType], ":", TTF_GetError());
		return nullptr; // don't cache failures
	}
	fontMap[fontKey] = font;
	return font;
}

void TextureLoader::clearCache()
{
	textureMap.clear();
	for (auto& [key, font] : fontMap) {
		if (font) {
			TTF_CloseFont(font);
		}
	}
	fontMap.clear();
}
