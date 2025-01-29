#include "AttackJoystickUIElement.h"

/**
 * @brief Handles mouse input events for the joystick, calculates deflection, and triggers an attack.
 *
 * @param mouseEventType The type of mouse event (e.g., press, release).
 * @param pos The mouse position in pixels.
 * @param timeDiff The time elapsed since the last input.
 * @return True if the event was handled; otherwise, false.
 */
bool AttackJoystickUIElement::handleMouseInput(MouseEventTypes mouseEventType, std::pair<int, int> pos, Time timeDiff)
{
    auto fieldSize = texture->getSize();
    bool ans = false;
    for (auto& child : children) {
        ans = ans || child->handleMouseInput(mouseEventType, pos, timeDiff);
    }
    if (pxRealPosRect.isPointInside(pos.first, pos.second)) {
        xDeflection = pos.first - (pxRealPosRect.x + pxRealPosRect.w / 2);
        yDeflection = pos.second - (pxRealPosRect.y + pxRealPosRect.h / 2);
        float deflectionsLength = sqrt(xDeflection * xDeflection + yDeflection * yDeflection);
        xDeflection /= deflectionsLength;
        yDeflection /= deflectionsLength;
        pressed = true;
        updateNeeded = true;
        player->startAttack(player->getPosition() + Position(20 * xDeflection, 20 * yDeflection, 0));
        return true;
    }
    if (pressed) {
        updateNeeded = true;
    }
    pressed = false;
    return false;
}

/**
 * @brief Draws the joystick and its knob based on the current deflection and pressed state.
 */
void AttackJoystickUIElement::drawInside()
{
    auto fieldSize = texture->getSize();
    image->draw(*texture, nullopt, SDL_Rect{
        (int)(fieldSize.first * .4), (int)(fieldSize.second * .4),
        (int)(fieldSize.first * .2), (int)(fieldSize.second * .2)
        });
    knobImage->draw(*texture, nullopt, SDL_Rect{
        (int)(fieldSize.first * (.45 + pressed * xDeflection * .1)),
        (int)(fieldSize.second * (.45 + pressed * yDeflection * .1)),
        (int)(fieldSize.first * .1), (int)(fieldSize.second * .1)
        });
}

/**
 * @brief Handles SDL touch events for the joystick, including multi-touch interactions.
 *
 * @param e The SDL event.
 * @param timeDiff The time elapsed since the last event.
 * @param screenSizeX The width of the screen in pixels.
 * @param screenSizeY The height of the screen in pixels.
 * @return The result of the event handling.
 */
UIElement::EventHandleResult
AttackJoystickUIElement::handleEvent(const SDL_Event& e, Time timeDiff, int screenSizeX, int screenSizeY) {
    bool atLeastOneFingerInside = false;
    for (auto f : fingerPositions) {
        if (pxRealPosRect.isPointInside(f.first, f.second)) {
            atLeastOneFingerInside = true;
            handleMouseInput(UIElement::MouseEventTypes::PRESS_LEFT, f, timeDiff);
        }
    }
    if (!atLeastOneFingerInside) {
        pressed = false;
        updateNeeded = true;
    }
    EventHandleResult ret{ false, {} };
    return ret;
}
