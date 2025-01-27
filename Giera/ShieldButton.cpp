#include "ShieldButton.h"

ShieldButton::ShieldButton(ButtonUI button, shared_ptr<Player> player):
	ButtonUI(std::move(button)),
	player(player)
{
}

bool ShieldButton::handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff)
{
	auto baseHandled = ButtonUI::handleMouseInput(mouseEventType, pos, timeDiff);
	/*if (baseHandled && mouseEventType != MouseEventTypes::HOVER) {
		Logger::logDebug("mouseEventType: ", mouseEventType);
	}*/
	if (baseHandled && 
		(mouseEventType == MouseEventTypes::PRESS_LEFT ||
			(isPressed && mouseEventType == MouseEventTypes::HOVER))) {
		isPressed = true;
		player->parry(timeDiff);
	}
	else if (!baseHandled || mouseEventType == MouseEventTypes::RELEASE_LEFT)  {
		isPressed = false;
		player->cancelParry();
	}
	return baseHandled;
}
