#include "ChangeWeaponButton.h"

ChangeWeaponButton::ChangeWeaponButton(ButtonUI button, shared_ptr<Player> player) :
	ButtonUI(std::move(button)),
	player(player)
{
}

bool ChangeWeaponButton::handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff)
{

	if (mouseEventType == MouseEventTypes::PRESS_LEFT && !isPressed) {
		Logger::logDebug("ChangeWeaponButton::handleMouseInput:changing mode");
		player->meleeMode = !(player->meleeMode);
        isPressed = true;
	}
	return false;
}

UIElement::EventHandleResult
ChangeWeaponButton::handleEvent(const SDL_Event &e, Time timeDiff, int screenSizeX,
                          int screenSizeY) {
    bool atLeastOneFingerInside = false;
    for (auto f:fingerPositions){
        if (pxRealPosRect.isPointInside(f.first,f.second)){
            atLeastOneFingerInside = true;
            handleMouseInput(UIElement::MouseEventTypes::PRESS_LEFT,f,timeDiff);
        }
    }
    if (!atLeastOneFingerInside){
        isPressed = false;
    }
    EventHandleResult ret{ false, {} };
    return ret;
}
