#pragma once
#include "Board.h"
#include "Window.h"
#include "InputConfig.h"
#include "MouseButtonStateTypes.h"
#include "JoystickUIElement.h"
#include "AttackJoystickUIElement.h"
#include <optional>
#include <set>
#include "ButtonUI.h"
#include "ShieldButton.h"
#include "ChangeWeaponButton.h"

using std::set;

class InventoryUI;
class Player;

/**
 * @class BoardLoop
 * @brief Handles the game board logic, including input, updates, and rendering.
 *
 * This class manages the main loop for the game board, handling user input, updating
 * game entities (e.g., the player, AI characters, projectiles), and rendering the scene.
 *
 * The loop is split into handleInput() (SDL events), update() (pure game logic, can be
 * driven by tests with a fixed time step) and render().
 */
class BoardLoop
{
public:
    /// Counters describing what happened while the loop ran (used by tests / smoke runs).
    struct Stats {
        int framesRendered = 0;
        int updates = 0;
        int enemiesSpawned = 0;
        int enemiesKilled = 0;
        int playerDeaths = 0;
        int playerRespawns = 0;
        Time gameTime;
    };

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
     * @brief Runs the main game loop until the window is closed (or for @p runFor of game time).
     *
     * Each iteration processes input, advances the game with update() and renders at most
     * ~60 times per second.
     */
    Stats start(std::optional<Time> runFor = std::nullopt);

    /**
     * @brief Advances the game logic by @p timeDiff: attacks, AI, projectiles,
     * deaths, enemy respawns and player respawn. Does not read input or render.
     */
    void update(Time timeDiff);

    /// Draws the board and the UI and presents the frame.
    void render(Time timeDiff);

    /// When enabled the player automatically attacks the nearest enemy (smoke tests / demos).
    void setAutoplay(bool enabled);

    const Stats& getStats() const;
    std::shared_ptr<Board> getBoard() const;
    std::shared_ptr<Player> getPlayer() const;

    static inline const Position playerSpawnPosition = Position(1.5, 1.5, 0);
    static inline const Position enemySpawnPosition = Position(14, 4.7, 0);
    static inline const Time playerRespawnDelay = Time(3000);
    static inline const Time enemyRespawnDelay = Time(2000);
    /// Longest time step simulated at once (e.g. after the window was dragged).
    static inline const Time maxTimeStep = Time(100);

protected:
    /**
     * @brief A pointer to the player's inventory UI, if open.
     * A raw pointer is passed to the parent (WindowUI) so that it can destroy the unique pointer that holds InventoryUI.
     *
     * When this optional has a value, the inventory UI is displayed. When reset,
     * it is removed from the UI hierarchy.
     */
    std::optional<InventoryUI*> playerInventoryUI;

    /// On-screen movement joystick (touch controls only).
    std::optional<JoystickUIElement*> joystickUI;

    /// On-screen attack joystick (touch controls only).
    std::optional<AttackJoystickUIElement*> attackJoystickUI;

    /// Shield (parry) button (touch controls only).
    std::optional<ShieldButton*> shieldButton;

    /// Button switching between bow and melee setup (touch controls only).
    std::optional<ChangeWeaponButton*> changeWeaponButton;

    /**
     * @brief A shared pointer to the game board.
     *
     * Holds the map, AI characters, projectiles, and items.
     */
    std::shared_ptr<Board> board;

    /// Currently pressed keyboard scancodes.
    set<SDL_Scancode> keySet;

    /// Current state of mouse buttons.
    std::array<MouseButtonStateTypes, (int)MouseButtonTypes::COUNT> mouseButtonStates;

    std::shared_ptr<InputConfig> inputConfig;

    /// The main application window and the root of the UI tree.
    std::shared_ptr<Window> window;

    /// Draws the board and owns the camera.
    std::shared_ptr<BoardRenderer> boardRenderer;

    std::shared_ptr<Player> player;

    Stats stats;
    bool quitRequested = false;
    bool paused = false;       ///< Android: app is in the background
    bool autoplay = false;
    Time playerDeadFor;
    Time noEnemyFor;
    bool playerWasDead = false;

    /**
     * @brief Handles player and UI input events.
     * @param timeDiff The time elapsed since the last input handling.
     *
     * Processes SDL events, keyboard states, and mouse states. It also updates camera
     * targeting and passes mouse/keyboard/touch input to the UI.
     * Sets quitRequested when the window is closed.
     */
    void handleInput(Time timeDiff);

    /// Shows / hides the on-screen touch controls (no-op when touch controls are disabled).
    void showTouchControls();
    void hideTouchControls();
    void toggleInventory();
    void closeInventory();
    void spawnEnemy();
    void runAutoplay();
};
