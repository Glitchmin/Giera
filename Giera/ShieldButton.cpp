#include "ShieldButton.h"

ShieldButton::ShieldButton(ButtonUI button, shared_ptr<Player> player):
	ButtonUI(std::move(button)),
	player(player)
{
}

bool ShieldButton::handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff)
{

    if(mouseEventType == MouseEventTypes::PRESS_LEFT){
        isPressed = true;
        player->parry(timeDiff);
        Logger::logInfo("shield button pressed");
    }
    return false;
}

UIElement::EventHandleResult
ShieldButton::handleEvent(const SDL_Event &e, Time timeDiff, int screenSizeX,
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
        player->cancelParry();
    }
    EventHandleResult ret{ false, {} };
    return ret;
}