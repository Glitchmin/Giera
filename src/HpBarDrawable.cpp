#include "HpBarDrawable.h"
#include "AbstractEffect.h"

HpBarDrawable::HpBarDrawable(Position pos, AbstractCharacter* ch)
	: Drawable(pos, nullptr, DrawableLayer::ENTITIES, make_pair(1,.2), .1)
{
	this->character = ch;
	this->hp = ch->getHpPtr();
	this->maxHp = ch->getMaxHpPtr();
}

void HpBarDrawable::draw(Texture& textureToDrawOn, const double& pixelToMeterRatio)
{
	// (This used to set hp back to max whenever it was 0 - drawing changed the game state.)
	float hpRatio = *maxHp > 0 ? (float)*hp / (float)*maxHp : 0.f;
	// 0.3 m tall: a 0.1 m strip of effect markers above the 0.2 m hp bar
	int width = std::max(1, (int)pixelToMeterRatio);
	int markerH = std::max(1, (int)(.1 * pixelToMeterRatio));
	int barH = std::max(1, (int)(.2 * pixelToMeterRatio));
	texture = TextureLoader::makeUniColorTexture(width, markerH + barH, { 0,0,0,0 });
	TextureLoader::makeUniColorTexture(1, 1, { 0,0,0,128 })->draw(*texture.get(), nullopt,
		SDL_Rect{ 0, markerH, width, barH });
	TextureLoader::makeUniColorTexture(1, 1, { 255,0,0,224 })->draw(*texture.get(), nullopt,
		SDL_Rect{ 0, markerH, (int)(hpRatio * width), barH });
	// one square per active effect: red = harmful (bleeding), green = helpful (healing)
	int x = 0;
	for (auto& effect : character->getActiveEffects()) {
		SDL_Color colour = effect->IsBuff() ? SDL_Color{ 80, 220, 80, 255 } : SDL_Color{ 200, 30, 30, 255 };
		TextureLoader::makeUniColorTexture(1, 1, colour)->draw(*texture.get(), nullopt,
			SDL_Rect{ x, 0, markerH, markerH - 1 });
		x += markerH + 2;
	}
	Drawable::draw(textureToDrawOn, pixelToMeterRatio);
}
