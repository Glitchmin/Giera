#include "ChangeWeaponButton.h"

ChangeWeaponButton::ChangeWeaponButton(ButtonUI button, shared_ptr<Player> player) :
	ButtonUI(std::move(button)),
	player(player)
{
}

bool ChangeWeaponButton::handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff)
{
	auto baseHandled = ButtonUI::handleMouseInput(mouseEventType, pos, timeDiff);
	//if (baseHandled && mouseEventType != MouseEventTypes::HOVER) {
	//	Logger::logDebug("ChangeWeaponButton::handleMouseInput: mouseEventType: ", mouseEventType);
	//}
	if (baseHandled && mouseEventType == MouseEventTypes::PRESS_LEFT) {
		//Logger::logDebug("ChangeWeaponButton::handleMouseInput:changing mode");
		player->meleeMode = !(player->meleeMode);
	}
	return baseHandled;
}
