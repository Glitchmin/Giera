#include "BoardLoop.h"
#include "Player.h"
#include "BaseItemHandler.h"
#include "FlightPath.h"
#include "SpellProjectile.h"
#include "InventoryUI.h"
#include "InventoryInputHandler.h"
#include "JoystickUIElement.h"
#include "AttackJoystickUIElement.h"
#include <ChangeWeaponButton.h>

/**
 * @brief Constructs a new BoardLoop, setting up the board, the player, and initial states.
 * @param window A shared pointer to the main application window.
 * @param inputConfig A shared pointer to the input configuration.
 *
 * This constructor creates the map, initializes the player and renderer, and populates the
 * board with some initial items and characters. Mouse states are also set to NOT_PRESSED.
 */
BoardLoop::BoardLoop(std::shared_ptr<Window> window, std::shared_ptr<InputConfig> inputConfig)
{
    this->window = window;
    this->inputConfig = inputConfig;
    auto map = std::make_unique<Map>(LandscapeTypes::GRASSLAND, MapTypes::GIERA, Directions::NORTH, 64, 64, 0);
    double viewRangeM = 20;
    player = std::make_shared<Player>();
    boardRenderer = std::make_shared<BoardRenderer>(map->getSizeX(), map->getSizeY(), window, viewRangeM, player);
    for (int i = 0; i < map->getSizeX(); i++) {
        for (int j = 0; j < map->getSizeY(); j++) {
            boardRenderer->addDrawableBoardEntity(map->getMapTile(Coordinates(i, j)));
        }
    }
    this->board = std::make_shared<Board>(std::move(map), boardRenderer);
    board->addItem(Coordinates(5, 0), BaseItemHandler::generate<Food>(ItemTypes::FOOD, (int)FoodTypes::BERRIES));
    board->addPlayerCharacter(player);
    for (int i = 0; i < (int)MouseButtonTypes::COUNT; i++) {
        mouseButtonStates[i] = MouseButtonStateTypes::NOT_PRESSED;
    }
}

/**
 * @brief Processes SDL events, keyboard, and mouse input.
 * @param timeDiff The time elapsed since the last input handling.
 *
 * Reads and updates keyboard and mouse button states. Interacts with UI elements if
 * they are present and handles camera target logic for mouse dragging. If the user
 * opens an inventory, the joystick UI and combat UI elements are hidden, and vice versa.
 *
 * Side effects include moving the player, toggling UI windows, and starting attacks.
 */
void BoardLoop::handleInput(Time timeDiff) {
    SDL_Event event;
    for (int i = 0; i < (int)MouseButtonTypes::COUNT; i++) {
        if (mouseButtonStates[i] == MouseButtonStateTypes::JUST_PRESSED) {
            mouseButtonStates[i] = MouseButtonStateTypes::PRESSED;
        }
        if (mouseButtonStates[i] == MouseButtonStateTypes::JUST_RELEASED) {
            mouseButtonStates[i] = MouseButtonStateTypes::NOT_PRESSED;
        }
    }
    set<SDL_Scancode> justPressedKeys;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            Logger::logInfo("user closed the window");
            break;
        case SDL_KEYDOWN:
            if (keySet.find(event.key.keysym.scancode) == keySet.end()) {
                justPressedKeys.insert(event.key.keysym.scancode);
            }
            keySet.insert(event.key.keysym.scancode);
            break;
        case SDL_KEYUP:
            keySet.erase(event.key.keysym.scancode);
            break;
        case SDL_MOUSEBUTTONUP:
        {
            Logger::logInfo((int)event.button.button, " button up");
            int buttonUpID = event.button.button == 1 ?
                (int)MouseButtonTypes::LEFT : (int)MouseButtonTypes::RIGHT;
            // mouseButtonStates[buttonUpID] = MouseButtonStateTypes::JUST_RELEASED;
        }
        break;
        case SDL_MOUSEBUTTONDOWN:
        {
            Logger::logInfo((int)event.button.button, " button down");
            int buttonDownID = event.button.button == 1 ?
                (int)MouseButtonTypes::LEFT : (int)MouseButtonTypes::RIGHT;
            // mouseButtonStates[buttonDownID] = MouseButtonStateTypes::JUST_PRESSED;
        }
        break;
        }
    }

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    window->handleEvent(event, timeDiff, window->getSize().first, window->getSize().second);

    // Temporary for Android:
    bool isMouseHandledByUI = false;

    if (mouseButtonStates[(int)MouseButtonTypes::LEFT] == MouseButtonStateTypes::JUST_PRESSED) {
        isMouseHandledByUI = window->handleMouseInput(UIElement::MouseEventTypes::PRESS_LEFT,
            std::make_pair(mouseX, mouseY), timeDiff) || isMouseHandledByUI;
    }
    if (mouseButtonStates[(int)MouseButtonTypes::RIGHT] == MouseButtonStateTypes::JUST_PRESSED) {
        isMouseHandledByUI = window->handleMouseInput(UIElement::MouseEventTypes::PRESS_RIGHT,
            std::make_pair(mouseX, mouseY), timeDiff) || isMouseHandledByUI;
    }
    if (mouseButtonStates[(int)MouseButtonTypes::LEFT] == MouseButtonStateTypes::JUST_RELEASED) {
        isMouseHandledByUI = window->handleMouseInput(UIElement::MouseEventTypes::RELEASE_LEFT,
            std::make_pair(mouseX, mouseY), timeDiff) || isMouseHandledByUI;
    }
    if (mouseButtonStates[(int)MouseButtonTypes::RIGHT] == MouseButtonStateTypes::JUST_RELEASED) {
        isMouseHandledByUI = window->handleMouseInput(UIElement::MouseEventTypes::RELEASE_RIGHT,
            std::make_pair(mouseX, mouseY), timeDiff) || isMouseHandledByUI;
    }
    if (!isMouseHandledByUI) {
        if (mouseButtonStates[(int)MouseButtonTypes::RIGHT] == MouseButtonStateTypes::NOT_PRESSED) {
            boardRenderer->getCamera().resetSecondaryTarget();
        }
        else {
            Camera& camera = boardRenderer->getCamera();
            camera.setMouseAsSecondaryTarget();
        }
        if (mouseButtonStates[(int)MouseButtonTypes::LEFT] == MouseButtonStateTypes::JUST_PRESSED) {
            Logger::logInfo(boardRenderer->getCamera().getLeftUpperPosition());
            player->startAttack(boardRenderer->getCamera().getBoardCursorPosition());
        }
    }

    bool isUIWindowOpen = playerInventoryUI.has_value();
    bool parryPressed = false;
    for (auto& key : keySet) {
        PlayerActionTypes action = inputConfig->getActionType(key);
        switch (action) {
            using PlAct = PlayerActionTypes;
        case PlAct::MOVE_LEFT:
            if (!isUIWindowOpen) {
                player->move(Position(-((double)timeDiff.getTimeMs() * 4.0 / 1000.0), 0, 0));
            }
            break;
        case PlAct::MOVE_DOWN:
            if (!isUIWindowOpen) {
                player->move(Position(0, ((double)timeDiff.getTimeMs() * 4.0 / 1000.0), 0));
            }
            break;
        case PlAct::MOVE_UP:
            if (!isUIWindowOpen) {
                player->move(Position(0, -((double)timeDiff.getTimeMs() * 4.0 / 1000.0), 0));
            }
            break;
        case PlAct::MOVE_RIGHT:
            if (!isUIWindowOpen) {
                player->move(Position(((double)timeDiff.getTimeMs() * 4.0 / 1000.0), 0, 0));
            }
            break;
        case PlAct::ADD_SECONDARY_CAMERA_TARGET:
            if (!isUIWindowOpen) {
                boardRenderer->getCamera().setSecondaryTarget(
                    board->getMap()->getMapTile(Coordinates(60, 60)));
            }
            break;
        case PlAct::PARRY:
            parryPressed = true;
            break;
        }
    }

    for (auto& key : justPressedKeys) {
        PlayerActionTypes action = inputConfig->getActionType(key);
        switch (action) {
            using PlAct = PlayerActionTypes;
        case PlAct::OPEN_EQUIPMENT:
            if (!playerInventoryUI.has_value()) {
                auto invUI = std::make_unique<InventoryUI>(
                    window, player->getInventory(),
                    std::make_shared<InventoryInputHandler>(player->getInventory()));
                playerInventoryUI = invUI.get();
                window->addChild(std::move(invUI));
                window->removeChild(joystickUI.value());
                joystickUI = std::nullopt;
                window->removeChild(shieldButton.value());
                shieldButton = std::nullopt;
                window->removeChild(changeWeaponButton.value());
                changeWeaponButton = std::nullopt;
            }
            else {
                window->removeChild(playerInventoryUI.value());
                playerInventoryUI = std::nullopt;
                auto joyUI = std::make_unique<JoystickUIElement>(
                    Rect<fr_pos_t>{0, .5, .5, .5}, window.get(), player);
                joystickUI = joyUI.get();
                window->addChild(std::move(joyUI));
                addShieldButton();
                addChangeWeaponButton();
            }
            break;
        case PlAct::CLOSE_WINDOW:
            if (playerInventoryUI.has_value()) {
                window->removeChild(playerInventoryUI.value());
                playerInventoryUI = std::nullopt;
                auto joyUI = std::make_unique<JoystickUIElement>(
                    Rect<fr_pos_t>{0, .5, .5, .5}, window.get(), player);
                joystickUI = joyUI.get();
                window->addChild(std::move(joyUI));
                addShieldButton();
                addChangeWeaponButton();
            }
            break;
        }
    }
}

/**
 * @brief Creates a ShieldButton UI element and adds it to the window.
 *
 * If the player has a selected shield, its texture is shown. Otherwise,
 * a default texture is used.
 *
 * Side effects include UI allocation and state change.
 */
void BoardLoop::addShieldButton()
{
    auto shieldTx = player->getSelectedShield() ?
        player->getSelectedShield()->getTexture() :
        TextureLoader::getTexturePtr(
            std::string(SAVE_FILES_PATH) + "/tx/items/" + "shield/shield" + "0" + ".png"
        );
    auto shieldButtonUniquePtr = std::make_unique<ShieldButton>(
        ButtonUI(Rect<fr_pos_t>(0.85, 0.35, 0.12, 0.12),
            shieldTx,
            window.get(),
            0.1),
        player
    );
    shieldButton = shieldButtonUniquePtr.get();
    window->addChild(std::move(shieldButtonUniquePtr));
}

/**
 * @brief Creates a ChangeWeaponButton UI element and adds it to the window.
 *
 * If the player has a selected weapon, its texture is used. Otherwise,
 * a default texture is shown.
 *
 * Side effects include UI allocation and state change.
 */
void BoardLoop::addChangeWeaponButton()
{
    auto weaponTx = player->getSelectedWeapon() ?
        player->getSelectedWeapon()->getTexture() :
        TextureLoader::getTexturePtr(
            std::string(SAVE_FILES_PATH) + "/tx/items/" + "arrows/arrow" + "0" + ".png"
        );
    auto changeWeaponButtonUniquePtr = std::make_unique<ChangeWeaponButton>(
        ButtonUI(
            Rect<fr_pos_t>(0.85, 0.15, 0.12, 0.12),
            weaponTx,
            window.get(),
            0.8
        ),
        player
    );
    changeWeaponButton = changeWeaponButtonUniquePtr.get();
    window->addChild(std::move(changeWeaponButtonUniquePtr));
}

/**
 * @brief Starts the main game loop.
 *
 * This function initializes UI elements (e.g., ShieldButton, ChangeWeaponButton,
 * and joysticks), then repeatedly processes input and updates the board state (e.g.,
 * AI, projectiles). The board is rendered at regular intervals.
 *
 * Side effects include continuously running the loop until broken,
 * managing projectiles, spawning AI, and updating the UI.
 */
void BoardLoop::start()
{
    bool loopGoing = true;
    GeneralTimer generalTimer;
    generalTimer.updateTime();
    Time lastGraphicUpdate(generalTimer.getTime());
    Time lastInputHandling(generalTimer.getTime());
    Time lastProjectileHandling(generalTimer.getTime());

    addShieldButton();
    addChangeWeaponButton();

    auto joyUI = std::make_unique<JoystickUIElement>(
        Rect<fr_pos_t>{0, .5, .5, .5}, window.get(), player);
    joystickUI = joyUI.get();
    window->addChild(std::move(joyUI));

    auto attackJoyUI = std::make_unique<AttackJoystickUIElement>(
        Rect<fr_pos_t>{.5, .5, .5, .5}, window.get(), player);
    attackJoystickUI = attackJoyUI.get();
    window->addChild(std::move(attackJoyUI));

    while (loopGoing) {
        Time inputTimeDiff = generalTimer.getTime() - lastInputHandling;
        lastInputHandling = generalTimer.getTime();
        handleInput(inputTimeDiff);

        while (board->getAiCharacters().size() < 1) {
            auto aiChar = std::make_shared<AiCharacter>(CharacterTypes::BANDIT_THUG, Position(14, 4.7, 0), 1);
            board->addAiCharacter(aiChar);
        }
        player->updateAttack(inputTimeDiff);
        for (auto& aiChar : board->getAiCharacters()) {
            aiChar->updateBehaviour(inputTimeDiff);
        }

        Time projectileTimeDiff = generalTimer.getTime() - lastProjectileHandling;
        lastProjectileHandling = generalTimer.getTime();
        board->calculateProjectiles(projectileTimeDiff);

        if (generalTimer.getTime() > lastGraphicUpdate + Time(16)) {
            Time renderTimeDiff = generalTimer.getTime() - lastGraphicUpdate;
            lastGraphicUpdate = generalTimer.getTime();
            boardRenderer->drawBoard(renderTimeDiff);

            window->renderUI();
            window->updateRenderer();
        }
        generalTimer.updateTime();
    }
}