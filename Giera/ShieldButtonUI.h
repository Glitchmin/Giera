#pragma once

#include "ButtonUI.h"
#include <Player.h>

class ShieldButtonUI : public ButtonUI {
public:
	ShieldButtonUI() = default;
	ShieldButtonUI(ButtonUI button, shared_ptr<Player> player);

	virtual bool handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff) override;

protected:
	shared_ptr<Player> player;
	bool isPressed;

};