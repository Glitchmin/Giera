#include "CombatHudUI.h"
#include "DamageEffect.h"
#include "FontTypes.h"
#include "Shield.h"
#include <algorithm>
#include <cmath>

CombatHudUI::CombatHudUI(Rect<fr_pos_t> frRelPosRect, UIElement* parent, shared_ptr<Player> player)
	: UIElement(frRelPosRect, nullptr, parent, SDL_Color{ 0, 0, 0, 0 }), player(player)
{
}

std::string CombatHudUI::getStatusText() const
{
	if (player->isDead()) {
		return "dead";
	}
	std::string text = player->meleeMode ? "MELEE" : "BOW";
	if (player->getSelectedBow()) {
		text += "  arrows " + std::to_string(player->getArrowCount());
	}
	else {
		text += "  no bow";
	}
	text += "  food " + std::to_string(player->getFoodCount());
	if (player->isStaggered()) {
		text += "  STAGGERED";
	}
	else if (player->isParryWindowOpen()) {
		text += "  PARRY";
	}
	for (auto& effect : player->getActiveEffects()) {
		auto damageEffect = std::dynamic_pointer_cast<DamageEffect>(effect);
		if (damageEffect) {
			text += damageEffect->getDamage()->getDamageType() == DamageTypes::HEALING ? "  healing" : "  bleeding";
		}
	}
	return text;
}

void CombatHudUI::refresh()
{
	std::string text = getStatusText();
	BarKind kind = BarKind::NONE;
	double fill = 0.;
	// (no bar for the bow: the aim arc's dots turn green when releasing would shoot)
	if (player->isEating()) {
		kind = BarKind::EAT;
		fill = player->getEatProgress();
	}
	else if (player->getParryCompleteness() > 0.) {
		kind = BarKind::SHIELD;
		fill = player->getParryCompleteness();
	}
	int bar = (int)std::lround(std::clamp(fill, 0., 1.) * 50) * 2; // 2% steps
	if (text != shownText || bar != shownBar || kind != shownKind) {
		if (text != shownText) {
			textTexture = TextureLoader::makeTextTexture(FontTypes::SMALL,
				std::max(8, pxRealPosRect.h / 3), text, { 255, 255, 255, 255 });
		}
		shownText = text;
		shownBar = bar;
		shownKind = kind;
		updateNeeded = true;
	}
}

void CombatHudUI::drawInside()
{
	auto size = texture->getSize();
	if (textTexture) {
		auto textSize = textTexture->getSize();
		textTexture->draw(*texture, std::nullopt, SDL_Rect{ 0, 0,
			std::min(textSize.first, size.first), std::min(textSize.second, size.second * 6 / 10) });
	}
	if (shownBar > 0 && shownKind != BarKind::NONE) {
		SDL_Rect back{ 0, size.second * 7 / 10, size.first * 2 / 3, size.second * 2 / 10 };
		barBackground->draw(*texture, std::nullopt, back);
		SDL_Rect fill = back;
		fill.w = back.w * shownBar / 100;
		auto& colour = shownKind == BarKind::SHIELD ? shieldBar : eatBar;
		colour->draw(*texture, std::nullopt, fill);
	}
}
