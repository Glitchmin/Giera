#pragma once

#include "ButtonUI.h"
#include <Player.h>

/**
 * @brief Represents a button used to control a shield (parry action) for a player.
 *
 * The `ShieldButton` extends `ButtonUI` and provides additional functionality
 * to interact with a player object, handling parry actions and managing button state.
 */
class ShieldButton : public ButtonUI {
public:
    /**
     * @brief Default constructor for `ShieldButton`.
     */
    ShieldButton() = default;

    /**
     * @brief Constructs a `ShieldButton` using a `ButtonUI` instance and a player.
     * @param button The `ButtonUI` instance to initialize the shield button.
     * @param player A shared pointer to the player object to be controlled by this button.
     */
    ShieldButton(ButtonUI button, shared_ptr<Player> player);

    /**
     * @brief Handles mouse input events specific to the shield button.
     * @param mouseEventType The type of mouse event (e.g., press, release).
     * @param pos The position of the mouse in screen coordinates.
     * @param timeDiff The time difference between the current and previous input events.
     * @return True if the event is processed, false otherwise.
     */
    virtual bool handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff) override;

    /**
     * @brief Handles SDL events for the shield button.
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

    /** @brief Tracks whether the shield button is currently pressed. */
    bool isPressed;
};
