 #pragma once
#include <unordered_map>
#include <memory>
#include <SDL.h>	
#include <SDL_ttf.h>
#include <string_view>
#include <array>
#include "Logger.h"
#include "FontTypes.h"

using std::unordered_map;
using std::shared_ptr;
using std::string_view;
using std::array;

class Texture;

class TextureLoader
{
public:
	static shared_ptr<Texture> getTexturePtr(string_view textureName);
	static shared_ptr<Texture> getTextureCopy(string_view textureName);
    static void setRenderer(SDL_Renderer* renderer);
	/// Releases cached textures and fonts. Call before the renderer / SDL is shut down
	/// (the static cache was otherwise destroyed after SDL_Quit at program exit).
	static void clearCache();
	static shared_ptr<Texture> makeUniColorTexture(int x, int y, SDL_Color color);
	/// A round dot with a thin dark outline (transparent corners).
	static shared_ptr<Texture> makeCircleTexture(int diameter, SDL_Color color);
	static shared_ptr<Texture> makeTextTexture(FontTypes fontType, int fontSize,
		string_view text, SDL_Color color);
private:
	static inline unordered_map <string, shared_ptr <Texture> > textureMap{};
	static inline unordered_map <string, TTF_Font*> fontMap{};
	static inline array<string, (int)FontTypes::COUNT> fontPaths = 
	{ string(SAVE_FILES_PATH) + "/fonts/MedievalSharp-Regular.ttf" };
	static inline SDL_Renderer* renderer = NULL;
	static shared_ptr <Texture> loadTexture(string_view textureName);
	static TTF_Font* loadTTF_Font(FontTypes fontType, int fontSize);
	friend class Texture;
};

