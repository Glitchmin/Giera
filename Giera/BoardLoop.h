#pragma once
#include "Board.h"
#include "Window.h"
#include "InputConfig.h"
#include "MouseButtonStateTypes.h"
#include "JoystickUIElement.h"
#include "AttackJoystickUIElement.h"
#include <set>
#include <ButtonUI.h>
#include <ShieldButton.h>
#include <ChangeWeaponButton.h>

using std::set;

class InventoryUI;
class Player;

/**
 * @class BoardLoop
 * @brief Handles the game board logic, including input, updates, and rendering.
 *
 * This class manages the main loop for the game board, handling user input, updating
 * game entities (e.g., the player, AI characters, projectiles), and rendering the scene.
 */
class BoardLoop
{
public:
    /**
     * @brief Constructs a new BoardLoop object.
     * @param window A shared pointer to the main application window.
     * @param inputConfig A shared pointer to the input configuration.
     *
     * Initializes the board, renderer, and player, and sets up input states.
     */
    BoardLoop(std::shared_ptr<Window> window, std::shared_ptr<InputConfig> inputConfig);

    /**
     * @brief Adds a shield button UI element to the window.
     *
     * Side effects include creating a ShieldButton and adding it to the UI hierarchy.
     */
    void addShieldButton();

    /**
     * @brief Adds a change weapon button UI element to the window.
     *
     * Side effects include creating a ChangeWeaponButton and adding it to the UI hierarchy.
     */
    void addChangeWeaponButton();

    /**
     * @brief Starts the main game loop.
     *
     * This function runs a loop that continuously processes input, updates AI characters,
     * manages projectiles, and renders the game board until the loop is broken.
     */
    void start();

protected:
    /**
     * @brief A pointer to the player’s inventory UI, if open. 
     A raw pointer is passed to the parent (WindowUI) so that it can destroy the unique pointer that holds InventoryUI.
     *
     * When this optional has a value, the inventory UI is displayed. When reset,
     * it is removed from the UI hierarchy.
     */
    std::optional<InventoryUI*> playerInventoryUI;

    /**
     * @brief A pointer to the on-screen movement joystick UI element.
     *
     * When this optional has a value, the joystick is displayed in the UI.
     */
    std::optional<JoystickUIElement*> joystickUI;

    /**
     * @brief A pointer to the on-screen attack joystick UI element.
     *
     * When this optional has a value, the joystick for attacks is displayed in the UI.
     */
    std::optional<AttackJoystickUIElement*> attackJoystickUI;

    /**
     * @brief A pointer to the shield button UI element.
     *
     * When this optional has a value, the shield button is displayed.
     */
    std::optional<ShieldButton*> shieldButton;

    /**
     * @brief A pointer to the change weapon button UI element, responsible for changing between bow and melee setup.
     *
     * When this optional has a value, the weapon change button is displayed.
     */
    std::optional<ChangeWeaponButton*> changeWeaponButton;

    /**
     * @brief A shared pointer to the game board.
     *
     * Holds the map, AI characters, projectiles, and items.
     */
    std::shared_ptr<Board> board;

    /**
     * @brief A set of currently pressed keyboard scancodes.
     */
    set<SDL_Scancode> keySet;

    /**
     * @brief An array describing the current state of mouse buttons.
     */
    std::array<MouseButtonStateTypes, (int)MouseButtonTypes::COUNT> mouseButtonStates;

    /**
     * @brief A shared pointer to the input configuration.
     */
    std::shared_ptr<InputConfig> inputConfig;

    /**
     * @brief A shared pointer to the main application window and UI elements.
     */
    std::shared_ptr<Window> window;

    /**
     * @brief A shared pointer to the board renderer.
     *
     * Responsible for drawing the board and managing the camera.
     */
    std::shared_ptr<BoardRenderer> boardRenderer;

    /**
     * @brief A shared pointer to the player character.
     */
    std::shared_ptr<Player> player;

    /**
     * @brief Handles player and UI input events.
     * @param timeDiff The time elapsed since the last input handling.
     *
     * This function processes SDL events, keyboard states, and mouse states. It also
     * updates camera targeting and passes mouse/keyboard input to the UI.
     *
     * Side effects include modifying the player’s movement, toggling UI windows,
     * and changing mouse button states.
     */
    void handleInput(Time timeDiff);
};
