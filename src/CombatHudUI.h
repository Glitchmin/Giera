#pragma once
#include "UIElement.h"
#include "Player.h"
#include <string>

/**
 * @brief Small always-visible combat status: weapon setup, arrows and food left,
 * STAGGERED / PARRY, active effects, and a bar for the shield (blue) or eating (green).
 * The bow has no bar: the aim arc shows the draw (AbstractCharacter::updateAimLine).
 */
class CombatHudUI : public UIElement
{
public:
	CombatHudUI(Rect<fr_pos_t> frRelPosRect, UIElement* parent, shared_ptr<Player> player);
	/// Call once per frame; redraws only when something visible changed.
	void refresh();
	virtual void drawInside() override;
	/// Status line as shown, e.g. "BOW  arrows 18  food 2" (also used by tests).
	std::string getStatusText() const;

protected:
	enum class BarKind { NONE, SHIELD, EAT };
	shared_ptr<Player> player;
	std::string shownText;
	int shownBar = -1;          ///< bar fill in % (quantised to avoid redrawing every frame)
	BarKind shownKind = BarKind::NONE;
	shared_ptr<Texture> textTexture;
	shared_ptr<Texture> barBackground = TextureLoader::makeUniColorTexture(4, 4, { 0, 0, 0, 140 });
	shared_ptr<Texture> shieldBar = TextureLoader::makeUniColorTexture(4, 4, { 90, 150, 235, 255 });
	shared_ptr<Texture> eatBar = TextureLoader::makeUniColorTexture(4, 4, { 90, 200, 90, 255 });
};
