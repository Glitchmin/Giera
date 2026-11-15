#include "ChangeWeaponButton.h"

/**
 * Constructs a `ChangeWeaponButton` using an existing `ButtonUI` instance and a player object.
 *
 * Initializes the button with the provided `ButtonUI` configuration and links it to the player
 * for toggling weapon modes.
 */
ChangeWeaponButton::ChangeWeaponButton(ButtonUI button, shared_ptr<Player> player) :
    ButtonUI(std::move(button)),
    player(player)
{
}

/**
 * Handles mouse input events to toggle the player's weapon mode.
 *
 * If the left mouse button is pressed and the button is not already pressed, the function
 * toggles the player's weapon mode between melee and ranged. Logs the action and updates
 * the internal pressed state.
 */
bool ChangeWeaponButton::handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff)
{
    // Only presses inside the button count (any left click anywhere used to switch weapons).
    if (mouseEventType == MouseEventTypes::PRESS_LEFT && !isPressed && pxRealPosRect.isPointInside(pos.first, pos.second)) {
        Logger::logDebug("ChangeWeaponButton::handleMouseInput:changing mode");
        player->toggleWeaponMode(); // stays in melee if no bow is equipped
        isPressed = true;
    }
    return false;
}

/**
 * Handles SDL events for the change weapon button, including multitouch input.
 *
 * Processes events for multiple fingers, checking if at least one finger is inside
 * the button's bounds. If no fingers are inside, the button resets its pressed state.
 *
 * @return An `EventHandleResult` indicating the outcome of event handling.
 */
UIElement::EventHandleResult ChangeWeaponButton::handleEvent(const SDL_Event& e, Time timeDiff,
    int screenSizeX, int screenSizeY)
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
    }
    EventHandleResult ret{ false, {} };
    return ret;
}
