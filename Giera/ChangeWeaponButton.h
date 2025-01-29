#pragma once

#include "ButtonUI.h"
#include <Player.h>

/**
 * @brief Represents a button for changing the player's weapon mode.
 *
 * The `ChangeWeaponButton` extends `ButtonUI` and allows toggling between melee
 * and ranged weapon modes for the associated player object.
 */
class ChangeWeaponButton : public ButtonUI {
public:
    ChangeWeaponButton() = default;

    /**
     * @brief Constructs a `ChangeWeaponButton` using a `ButtonUI` instance and a player object.
     * @param button The `ButtonUI` instance to initialize the change weapon button.
     * @param player A shared pointer to the player object whose weapon mode will be toggled.
     */
    ChangeWeaponButton(ButtonUI button, shared_ptr<Player> player);

    /**
     * @brief Handles mouse input events for toggling the weapon mode.
     * @param mouseEventType The type of mouse event (e.g., press, release).
     * @param pos The position of the mouse in screen coordinates.
     * @param timeDiff The time difference between the current and previous input events.
     * @return True if the event is processed, false otherwise.
     */
    virtual bool handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff) override;

    /**
     * @brief Handles SDL events for the change weapon button.
     * @param e The SDL event to be processed.
     * @param timeDiff The time difference between the current and previous events.
     * @param screenSizeX The width of the screen in pixels.
     * @param screenSizeY The height of the screen in pixels.
     * @return The result of the event handling.
     */
    UIElement::EventHandleResult handleEvent(const SDL_Event& e, Time timeDiff, int screenSizeX,
        int screenSizeY) override;

protected:
    /** @brief A shared pointer to the player object controlled by this button. */
    shared_ptr<Player> player;

    /** @brief Tracks whether the button is currently pressed to avoid repeated toggling. */
    bool isPressed = false;
};
