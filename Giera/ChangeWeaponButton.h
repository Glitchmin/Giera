#pragma once

#include "ButtonUI.h"
#include <Player.h>

class ChangeWeaponButton : public ButtonUI {
public:
	ChangeWeaponButton() = default;
	ChangeWeaponButton(ButtonUI button, shared_ptr<Player> player);

	virtual bool handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff) override;

protected:
	shared_ptr<Player> player;
};