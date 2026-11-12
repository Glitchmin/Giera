#include "HpBarDrawable.h"

HpBarDrawable::HpBarDrawable(Position pos, AbstractCharacter* ch)
	: Drawable(pos, nullptr, DrawableLayer::ENTITIES, make_pair(1,.2), .1)
{
	this->hp = ch->getHpPtr();
	this->maxHp = ch->getMaxHpPtr();
}

void HpBarDrawable::draw(Texture& textureToDrawOn, const double& pixelToMeterRatio)
{
	float hpRatio = (float)*hp / (float)*maxHp;
	if ((*hp) == 0) {
		(*hp) = (*maxHp);
	}
	texture = TextureLoader::makeUniColorTexture(pixelToMeterRatio,0.2 * pixelToMeterRatio, {0,0,0,128});
	TextureLoader::makeUniColorTexture(1,1, { 255,0,0,224 })->draw(*texture.get(), nullopt,
		SDL_Rect{ 0,0,(int)(hpRatio * pixelToMeterRatio), (int)(.2 * pixelToMeterRatio) });
	Drawable::draw(textureToDrawOn, pixelToMeterRatio);
}
