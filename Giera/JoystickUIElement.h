#pragma once
#include <UIElement.h>
#include <Player.h>

/**
 * @brief Represents a joystick user interface (UI) element for interacting with a player object.
 *
 * The `JoystickUIElement` class is a specialized UI element that provides functionality
 * for handling mouse input, rendering a joystick graphic, and controlling player movement
 * based on joystick interaction. It is derived from the `UIElement` base class.
 */
class JoystickUIElement : public UIElement
{
public:
    /**
     * @brief Constructs a `JoystickUIElement` with the given parameters.
     *
     * Initializes the joystick with its relative position, parent element, and associated player object.
     *
     * @param frRelPosRect The relative position and size of the joystick in fractional coordinates.
     * @param parent The parent `UIElement` that this joystick is part of.
     * @param player A shared pointer to the player object that this joystick controls.
     */
    JoystickUIElement(
        Rect<fr_pos_t> frRelPosRect,
        UIElement* parent,
        shared_ptr<Player> player);

    /**
     * @brief Handles mouse input events for the joystick.
     *
     * Processes mouse input (e.g., clicks or movements) and calculates joystick deflection
     * to control the player's movement accordingly.
     *
     * @param mouseEventType The type of mouse event (e.g., click, move).
     * @param pos A pair representing the mouse position (x, y) in screen coordinates.
     * @param timeDiff The time difference between the current and the previous input event.
     *
     * @return True if the mouse event was handled within the joystick's bounds, false otherwise.
     */
    virtual bool handleMouseInput(MouseEventTypes mouseEventType, std::pair<int, int> pos, Time timeDiff) override;

    /**
     * @brief Handles general SDL events for the joystick UI element.
     *
     * Processes input events, such as keyboard or window events, and performs
     * any updates required for the joystick UI element.
     *
     * @param e The SDL event to handle.
     * @param timeDiff The time difference between the current and previous events.
     * @param screenSizeX The width of the screen.
     * @param screenSizeY The height of the screen.
     *
     * @return The result of the event handling.
     */
    virtual UIElement::EventHandleResult handleEvent(const SDL_Event& e, Time timeDiff, int screenSizeX, int screenSizeY) override;

    /**
     * @brief Draws the joystick's internal components.
     *
     * Handles rendering of the joystick's background, knob, and any additional visual elements.
     */
    virtual void drawInside() override;

protected:
    /**
     * @brief The texture for the joystick's base.
     */
    shared_ptr<Texture> image = TextureLoader::makeUniColorTexture(10, 10, { 255, 255, 255, 255 });

    /**
     * @brief The texture for the joystick's knob.
     */
    shared_ptr<Texture> knobImage = TextureLoader::makeUniColorTexture(10, 10, { 0, 0, 0, 255 });

    /**
     * @brief The horizontal deflection of the joystick.
     *
     * A value between -1 and 1 representing the horizontal offset of the joystick knob from its center.
     */
    float xDeflection = 0;

    /**
     * @brief The vertical deflection of the joystick.
     *
     * A value between -1 and 1 representing the vertical offset of the joystick knob from its center.
     */
    float yDeflection = 0;

    /**
     * @brief Indicates whether the joystick is currently pressed.
     */
    bool pressed = false;

    /**
     * @brief A shared pointer to the player object controlled by the joystick.
     */
    shared_ptr<Player> player;
};
