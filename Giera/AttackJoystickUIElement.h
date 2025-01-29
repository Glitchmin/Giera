#pragma once
#include "UIElement.h"
#include "Player.h"

/**
 * @brief A UI element representing an attack joystick, allowing the player to aim and attack in a specific direction.
 */
class AttackJoystickUIElement : public UIElement
{
public:
    /**
     * @brief Constructs the AttackJoystickUIElement.
     *
     * @param frRelPosRect The area where the touch is intercepted to steer the joystick. Defined as fractional coordinates
     * relative to the parent.
     * @param parent The parent UI element.
     * @param player A shared pointer to the Player object associated with the joystick.
     */
    AttackJoystickUIElement(
        Rect<fr_pos_t> frRelPosRect,
        UIElement* parent, shared_ptr<Player> player)
        :UIElement(frRelPosRect, nullptr, parent, bgColor = { 255,255,255,12 }),
        player(player) {};

    /**
     * @brief Handles mouse input events for the joystick.
     *
     * @param mouseEventType The type of mouse event (e.g., press, release).
     * @param pos The mouse position in pixels.
     * @param timeDiff The time elapsed since the last input.
     * @return True if the event was handled; otherwise, false.
     */
    virtual bool handleMouseInput(MouseEventTypes mouseEventType, std::pair<int, int> pos, Time timeDiff) override;

    /**
     * @brief Handles SDL events for the joystick.
     *
     * @param e The SDL event.
     * @param timeDiff The time elapsed since the last event.
     * @param screenSizeX The width of the screen in pixels.
     * @param screenSizeY The height of the screen in pixels.
     * @return The result of the event handling.
     */
    virtual UIElement::EventHandleResult handleEvent(const SDL_Event& e, Time timeDiff, int screenSizeX, int screenSizeY) override;

    /**
     * @brief Draws the joystick and its knob inside the element.
     */
    virtual void drawInside() override;

protected:
    shared_ptr<Texture> image = TextureLoader::makeUniColorTexture(10, 10, { 255,255,255,255 }); /**< Texture for the joystick background. */
    shared_ptr<Texture> knobImage = TextureLoader::makeUniColorTexture(10, 10, { 0,0,0,255 }); /**< Texture for the joystick knob. */
    float xDeflection = 0; /**< Horizontal deflection of the joystick knob. */
    float yDeflection = 0; /**< Vertical deflection of the joystick knob. */
    bool pressed = false; /**< Whether the joystick is currently pressed. */
    shared_ptr<Player> player; /**< Reference to the player interacting with the joystick. */
};
