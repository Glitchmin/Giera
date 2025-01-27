#pragma once

#include "ButtonUI.h"
#include <Player.h>

class ShieldButton : public ButtonUI {
public:
	ShieldButton() = default;
	ShieldButton(ButtonUI button, shared_ptr<Player> player);

	virtual bool handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff) override;

protected:
	shared_ptr<Player> player;
	bool isPressed;
};