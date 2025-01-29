#include "ShieldButton.h"

/**
 * Constructs a `ShieldButton` using an existing `ButtonUI` instance and a player object.
 *
 * Initializes the shield button with the provided button UI configuration and links it
 * to the player for handling parry actions.
 */
ShieldButton::ShieldButton(ButtonUI button, shared_ptr<Player> player)
    : ButtonUI(std::move(button)),
    player(player)
{
}

/**
 * Handles mouse input events for the shield button.
 *
 * If the left mouse button is pressed, the shield button triggers the player's parry action.
 * Logs the press event and updates the internal pressed state.
 */
bool ShieldButton::handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff)
{
    if (mouseEventType == MouseEventTypes::PRESS_LEFT) {
        isPressed = true;
        player->parry(timeDiff);
        Logger::logInfo("shield button pressed");
    }
    return false;
}

/**
 * Handles SDL events for the shield button, including multitouch input.
 *
 * Processes events for multiple fingers, checking if at least one finger is inside
 * the button's bounds. If no fingers are inside, the shield button resets and cancels
 * the player's parry action.
 *
 * @return An `EventHandleResult` indicating the outcome of event handling.
 */
UIElement::EventHandleResult ShieldButton::handleEvent(const SDL_Event& e, Time timeDiff, int screenSizeX,
    int screenSizeY)
{
    bool atLeastOneFingerInside = false;
    for (auto f : fingerPositions) {
        if (pxRealPosRect.isPointInside(f.first, f.second)) {
            atLeastOneFingerInside = true;
            handleMouseInput(UIElement::MouseEventTypes::PRESS_LEFT, f, timeDiff);
        }
    }
    if (!atLeastOneFingerInside) {
        isPressed = false;
        player->cancelParry();
    }
    EventHandleResult ret{ false, {} };
    return ret;
}
