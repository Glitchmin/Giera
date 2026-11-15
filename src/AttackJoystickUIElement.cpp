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
    bool ans = false;
    for (auto& child : children) {
        ans = ans || child->handleMouseInput(mouseEventType, pos, timeDiff);
    }
    if (mouseEventType == MouseEventTypes::PRESS_LEFT && pxRealPosRect.isPointInside(pos.first, pos.second)) {
        press(pos);
        return true;
    }
    // Mouse (desktop --touch-ui): button released -> shoot. Fingers are released in handleEvent().
    if (mouseEventType == MouseEventTypes::RELEASE_LEFT && pressed && !fingerDown) {
        release();
        return true;
    }
    return false;
}

/**
 * Press / hold: aims in the knob direction. A new press starts an attack (melee swing or
 * drawing the bow); holding keeps re-aiming the bow, or swings again in melee mode.
 */
void AttackJoystickUIElement::press(std::pair<int, int> pos)
{
    float x = pos.first - (pxRealPosRect.x + pxRealPosRect.w / 2);
    float y = pos.second - (pxRealPosRect.y + pxRealPosRect.h / 2);
    float length = sqrt(x * x + y * y);
    if (length < 1.f) {
        x = 1.f; // finger exactly in the middle: keep a valid direction
        y = 0.f;
        length = 1.f;
    }
    xDeflection = x / length;
    yDeflection = y / length;
    updateNeeded = true;
    if (!pressed || !player->isDrawing()) {
        player->startAttack(aimTarget());
    }
    else {
        player->aimAt(aimTarget());
    }
    pressed = true;
}

/// Finger lifted / mouse released: shoots a drawn bow.
void AttackJoystickUIElement::release()
{
    if (pressed) {
        player->releaseAttack(aimTarget());
    }
    pressed = false;
    updateNeeded = true;
}

Position AttackJoystickUIElement::aimTarget() const
{
    return player->getPosition() + Position(aimDistance * xDeflection, aimDistance * yDeflection, 0);
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
            fingerDown = true;
            press(f);
            break; // one finger steers the attack
        }
    }
    if (!atLeastOneFingerInside && fingerDown) {
        fingerDown = false;
        release();
    }
    EventHandleResult ret{ false, {} };
    return ret;
}
