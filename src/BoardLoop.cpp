#include "BoardLoop.h"
#include "Player.h"
#include "BaseItemHandler.h"
#include "CombatRules.h"
#include "CombatHudUI.h"
#include "ActionButton.h"
#include "FontTypes.h"
#include "InventoryUI.h"
#include "InventoryInputHandler.h"
#include "JoystickUIElement.h"
#include "AttackJoystickUIElement.h"
#include "ChangeWeaponButton.h"
#include "GeneralTimer.h"
#include "Platform.h"
#include <limits>

/**
 * @brief Constructs a new BoardLoop, setting up the board, the player, and initial states.
 * @param window A shared pointer to the main application window.
 * @param inputConfig A shared pointer to the input configuration.
 *
 * This constructor creates the map, initializes the player and renderer, and populates the
 * board with some initial items and the first enemy. Mouse states are set to NOT_PRESSED.
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
    spawnEnemy();
}

/**
 * @brief Processes SDL events, keyboard, and mouse input.
 * @param timeDiff The time elapsed since the last input handling.
 *
 * Reads and updates keyboard and mouse button states. Interacts with UI elements if
 * they are present and handles camera target logic for mouse dragging. If the user
 * opens an inventory, the touch controls are hidden, and vice versa.
 *
 * Side effects include moving the player, toggling UI windows, starting attacks and
 * requesting the loop to quit.
 */
void BoardLoop::handleInput(Time timeDiff) {
    for (int i = 0; i < (int)MouseButtonTypes::COUNT; i++) {
        if (mouseButtonStates[i] == MouseButtonStateTypes::JUST_PRESSED) {
            mouseButtonStates[i] = MouseButtonStateTypes::PRESSED;
        }
        if (mouseButtonStates[i] == MouseButtonStateTypes::JUST_RELEASED) {
            mouseButtonStates[i] = MouseButtonStateTypes::NOT_PRESSED;
        }
    }
    set<SDL_Scancode> justPressedKeys;
    SDL_Event event{};
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_APP_TERMINATING:
            Logger::logInfo("user closed the window");
            quitRequested = true; // used to be only logged: the window could not be closed
            break;
        case SDL_APP_WILLENTERBACKGROUND:
            paused = true; // Android: GL context may go away, stop rendering
            break;
        case SDL_APP_DIDENTERFOREGROUND:
            paused = false;
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
        case SDL_MOUSEBUTTONDOWN:
        {
            // SDL turns every touch into a fake mouse click as well. Those used to trigger
            // attacks while steering the joysticks, so mouse handling had been commented out
            // entirely (breaking the desktop). Ignore only the synthetic ones, except in the
            // inventory, which is driven by clicks.
            if (event.button.which == SDL_TOUCH_MOUSEID && !playerInventoryUI.has_value()) {
                break;
            }
            int buttonID;
            if (event.button.button == SDL_BUTTON_LEFT) {
                buttonID = (int)MouseButtonTypes::LEFT;
            }
            else if (event.button.button == SDL_BUTTON_RIGHT) {
                buttonID = (int)MouseButtonTypes::RIGHT;
            }
            else {
                break;
            }
            mouseButtonStates[buttonID] = event.type == SDL_MOUSEBUTTONDOWN ?
                MouseButtonStateTypes::JUST_PRESSED : MouseButtonStateTypes::JUST_RELEASED;
        }
        break;
        }
    }

    // Touch controls poll the current finger positions once per frame.
    SDL_Event frameEvent{};
    window->handleEvent(frameEvent, timeDiff, window->getSize().first, window->getSize().second);

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    auto mousePos = std::make_pair(mouseX, mouseY);

    bool isMouseHandledByUI = window->handleMouseInput(UIElement::MouseEventTypes::HOVER, mousePos, timeDiff);

    if (mouseButtonStates[(int)MouseButtonTypes::LEFT] == MouseButtonStateTypes::JUST_PRESSED) {
        isMouseHandledByUI = window->handleMouseInput(UIElement::MouseEventTypes::PRESS_LEFT,
            mousePos, timeDiff) || isMouseHandledByUI;
    }
    if (mouseButtonStates[(int)MouseButtonTypes::RIGHT] == MouseButtonStateTypes::JUST_PRESSED) {
        isMouseHandledByUI = window->handleMouseInput(UIElement::MouseEventTypes::PRESS_RIGHT,
            mousePos, timeDiff) || isMouseHandledByUI;
    }
    if (mouseButtonStates[(int)MouseButtonTypes::LEFT] == MouseButtonStateTypes::JUST_RELEASED) {
        isMouseHandledByUI = window->handleMouseInput(UIElement::MouseEventTypes::RELEASE_LEFT,
            mousePos, timeDiff) || isMouseHandledByUI;
    }
    if (mouseButtonStates[(int)MouseButtonTypes::RIGHT] == MouseButtonStateTypes::JUST_RELEASED) {
        isMouseHandledByUI = window->handleMouseInput(UIElement::MouseEventTypes::RELEASE_RIGHT,
            mousePos, timeDiff) || isMouseHandledByUI;
    }
    if (!isMouseHandledByUI && !playerInventoryUI.has_value()) {
        if (mouseButtonStates[(int)MouseButtonTypes::RIGHT] == MouseButtonStateTypes::NOT_PRESSED) {
            boardRenderer->getCamera().resetSecondaryTarget();
        }
        else {
            boardRenderer->getCamera().setMouseAsSecondaryTarget();
        }
        if (mouseButtonStates[(int)MouseButtonTypes::LEFT] == MouseButtonStateTypes::JUST_PRESSED) {
            // melee: swing; bow: start drawing (shoots on release)
            player->startAttack(boardRenderer->getCamera().getBoardCursorPosition());
            attackHeldInWorld = true;
        }
    }
    // Holding the left button aims the drawn bow at the cursor; releasing shoots
    // (also when the button is released over a UI element).
    if (attackHeldInWorld) {
        auto cursor = boardRenderer->getCamera().getBoardCursorPosition();
        auto leftState = mouseButtonStates[(int)MouseButtonTypes::LEFT];
        if (leftState == MouseButtonStateTypes::JUST_RELEASED || leftState == MouseButtonStateTypes::NOT_PRESSED) {
            player->releaseAttack(cursor);
            attackHeldInWorld = false;
        }
        else {
            player->aimAt(cursor);
        }
    }

    bool isUIWindowOpen = playerInventoryUI.has_value();
    bool parryPressed = false;
    const double step = timeDiff.getTimeS() * CombatRules::get().playerWalkSpeed;
    for (auto& key : keySet) {
        PlayerActionTypes action = inputConfig->getActionType(key);
        switch (action) {
            using PlAct = PlayerActionTypes;
        case PlAct::MOVE_LEFT:
            if (!isUIWindowOpen) {
                player->move(Position(-step, 0, 0));
            }
            break;
        case PlAct::MOVE_DOWN:
            if (!isUIWindowOpen) {
                player->move(Position(0, step, 0));
            }
            break;
        case PlAct::MOVE_UP:
            if (!isUIWindowOpen) {
                player->move(Position(0, -step, 0));
            }
            break;
        case PlAct::MOVE_RIGHT:
            if (!isUIWindowOpen) {
                player->move(Position(step, 0, 0));
            }
            break;
        case PlAct::ADD_SECONDARY_CAMERA_TARGET:
            if (!isUIWindowOpen) {
                boardRenderer->getCamera().setSecondaryTarget(
                    board->getMap()->getMapTile(Coordinates(60, 60)));
            }
            break;
        case PlAct::PARRY:
            parryPressed = !isUIWindowOpen;
            break;
        default:
            break;
        }
    }
    // Keyboard parry (was read into an unused variable before). The touch shield button
    // drives parry on its own, so only cancel here when that button isn't shown.
    if (parryPressed) {
        player->parry(timeDiff);
    }
    else if (!shieldButton.has_value()) {
        player->cancelParry();
    }

    for (auto& key : justPressedKeys) {
        PlayerActionTypes action = inputConfig->getActionType(key);
        switch (action) {
            using PlAct = PlayerActionTypes;
        case PlAct::OPEN_EQUIPMENT:
            toggleInventory();
            break;
        case PlAct::CLOSE_WINDOW:
            closeInventory();
            break;
        case PlAct::CHANGE_WEAPON:
            if (!isUIWindowOpen) {
                player->toggleWeaponMode(); // stays in melee if no bow is equipped
            }
            break;
        case PlAct::DASH:
            if (!isUIWindowOpen) {
                player->startDash();
            }
            break;
        case PlAct::USE_SATCHEL_1:
        case PlAct::USE_SATCHEL_2:
        case PlAct::USE_SATCHEL_3:
        case PlAct::USE_SATCHEL_4:
            if (!isUIWindowOpen) {
                player->eatFromSatchel((int)action - (int)PlAct::USE_SATCHEL_1);
            }
            break;
        default:
            break;
        }
    }
}

void BoardLoop::toggleInventory()
{
    if (playerInventoryUI.has_value()) {
        closeInventory();
        return;
    }
    player->cancelDrawing();
    player->cancelParry();
    attackHeldInWorld = false;
    auto invUI = std::make_unique<InventoryUI>(
        window, player->getInventory(),
        std::make_shared<InventoryInputHandler>(player->getInventory()));
    playerInventoryUI = invUI.get();
    window->addChild(std::move(invUI));
    hideTouchControls();
}

void BoardLoop::closeInventory()
{
    if (!playerInventoryUI.has_value()) {
        return;
    }
    window->removeChild(playerInventoryUI.value());
    playerInventoryUI = std::nullopt;
    showTouchControls();
}

void BoardLoop::showTouchControls()
{
    if (!Platform::touchControlsEnabled()) {
        return; // desktop: keyboard + mouse
    }
    if (!joystickUI.has_value()) {
        auto joyUI = std::make_unique<JoystickUIElement>(
            Rect<fr_pos_t>{0, .5, .5, .5}, window.get(), player);
        joystickUI = joyUI.get();
        window->addChild(std::move(joyUI));
    }
    if (!attackJoystickUI.has_value()) {
        auto attackJoyUI = std::make_unique<AttackJoystickUIElement>(
            Rect<fr_pos_t>{.5, .5, .5, .5}, window.get(), player);
        attackJoystickUI = attackJoyUI.get();
        window->addChild(std::move(attackJoyUI));
    }
    if (!shieldButton.has_value()) {
        addShieldButton();
    }
    if (!changeWeaponButton.has_value()) {
        addChangeWeaponButton();
    }
    if (!dashButton.has_value()) {
        auto label = TextureLoader::makeTextTexture(FontTypes::SMALL, 48, "DASH", { 255, 255, 255, 255 });
        auto button = std::make_unique<ActionButton>(
            ButtonUI(Rect<fr_pos_t>(0.70, 0.35, 0.12, 0.12), label, window.get(), 0.1,
                ImageResizeTypes::KEEP_ASPECT_RATIO, VerticalAlignmentTypes::CENTER, HorizontalAlignmentTypes::CENTER),
            [this]() { player->startDash(); });
        dashButton = button.get();
        window->addChild(std::move(button));
    }
    if (!eatButton.has_value()) {
        auto icon = TextureLoader::getTexturePtr(std::string(SAVE_FILES_PATH) + "/tx/items/food/food0.png");
        auto button = std::make_unique<ActionButton>(
            ButtonUI(Rect<fr_pos_t>(0.70, 0.15, 0.12, 0.12), icon, window.get(), 0.1,
                ImageResizeTypes::KEEP_ASPECT_RATIO, VerticalAlignmentTypes::CENTER, HorizontalAlignmentTypes::CENTER),
            [this]() { player->eatAnyFood(); });
        eatButton = button.get();
        window->addChild(std::move(button));
    }
}

void BoardLoop::hideTouchControls()
{
    // has_value() checks: the old code called .value() unconditionally and threw
    // std::bad_optional_access when an element wasn't there.
    if (joystickUI.has_value()) {
        window->removeChild(joystickUI.value());
        joystickUI = std::nullopt;
    }
    if (attackJoystickUI.has_value()) {
        window->removeChild(attackJoystickUI.value());
        attackJoystickUI = std::nullopt;
    }
    if (shieldButton.has_value()) {
        window->removeChild(shieldButton.value());
        shieldButton = std::nullopt;
    }
    if (changeWeaponButton.has_value()) {
        window->removeChild(changeWeaponButton.value());
        changeWeaponButton = std::nullopt;
    }
    if (dashButton.has_value()) {
        window->removeChild(dashButton.value());
        dashButton = std::nullopt;
    }
    if (eatButton.has_value()) {
        window->removeChild(eatButton.value());
        eatButton = std::nullopt;
    }
}

/**
 * @brief Creates a ShieldButton UI element and adds it to the window.
 *
 * If the player has a selected shield, its texture is shown. Otherwise,
 * a default texture is used.
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
 */
void BoardLoop::addChangeWeaponButton()
{
    auto weaponTx = player->getSelectedWeapon() ?
        player->getSelectedWeapon()->getTexture() :
        TextureLoader::getTexturePtr(
            std::string(SAVE_FILES_PATH) + "/tx/items/" + "arrow/arrow" + "0" + ".png"
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

void BoardLoop::spawnEnemy()
{
    // Find a walkable spot near the default spawn point (random walls could cover it).
    const double halfWidth = 0.4;
    auto isFree = [this, halfWidth](const Position& p) {
        return board->isStepablePosition(p - Position(halfWidth, 0, 0)) &&
            board->isStepablePosition(p + Position(halfWidth, 0, 0));
    };
    Position spawn = enemySpawnPosition;
    for (int radius = 0; radius < 10 && !isFree(spawn); radius++) {
        for (int dx = -radius; dx <= radius && !isFree(spawn); dx++) {
            for (int dy = -radius; dy <= radius; dy++) {
                Position candidate = enemySpawnPosition + Position(dx, dy, 0);
                if (isFree(candidate)) {
                    spawn = candidate;
                    break;
                }
            }
        }
    }
    auto aiChar = std::make_shared<AiCharacter>(CharacterTypes::BANDIT_THUG, spawn, 1);
    board->addAiCharacter(aiChar);
    stats.enemiesSpawned++;
    noEnemyFor = Time(0);
}

/**
 * Demo / smoke-test player: shoots fully drawn arrows at the nearest enemy while it is
 * far away and switches to the sword when it comes into melee range (or the quiver is empty).
 */
void BoardLoop::runAutoplay()
{
    if (player->isDead()) {
        return;
    }
    std::shared_ptr<AiCharacter> nearest;
    double best = std::numeric_limits<double>::max();
    for (auto& ai : board->getAiCharacters()) {
        double d = (ai->getPosition() - player->getPosition()).grounded().getNorm();
        if (!ai->isDead() && d < best) {
            best = d;
            nearest = ai;
        }
    }
    if (!nearest) {
        player->cancelDrawing();
        return;
    }
    // aim at the chest so the arrow meets the body before the ground
    Position target = nearest->getPosition() + Position(0, 0, 0.6);
    bool close = best < CombatRules::get().meleeReach + 0.6;
    if (close || !player->canShoot()) {
        if (!player->meleeMode) {
            player->setMeleeMode(true);
        }
        if (best <= CombatRules::get().meleeReach + 0.2 && player->canAttack()) {
            player->startAttack(nearest->getPosition());
        }
        return;
    }
    if (player->meleeMode) {
        player->setMeleeMode(false);
    }
    if (!player->isDrawing()) {
        player->startAttack(target);
    }
    else if (player->getDrawProgress() >= 1.0) {
        player->releaseAttack(target);
    }
    else {
        player->aimAt(target);
    }
}

void BoardLoop::update(Time timeDiff)
{
    stats.updates++;
    stats.gameTime += timeDiff;

    if (autoplay) {
        runAutoplay();
    }

    player->updateAttack(timeDiff);
    player->pickUpArrows();
    // Iterate over a copy: behaviour code may kill characters.
    auto aiCharacters = board->getAiCharacters();
    for (auto& aiChar : aiCharacters) {
        aiChar->updateBehaviour(timeDiff);
    }
    board->calculateProjectiles(timeDiff);

    int killed = board->removeDeadCharacters();
    if (killed > 0) {
        stats.enemiesKilled += killed;
        Logger::logInfo("enemies killed so far:", stats.enemiesKilled);
    }

    if (board->getAiCharacters().empty()) {
        noEnemyFor += timeDiff;
        if (!(noEnemyFor < enemyRespawnDelay)) {
            spawnEnemy();
        }
    }

    if (player->isDead()) {
        if (!playerWasDead) {
            playerWasDead = true;
            playerDeadFor = Time(0);
            stats.playerDeaths++;
            Logger::logInfo("You died. Respawning in", playerRespawnDelay);
            closeInventory();
        }
        playerDeadFor += timeDiff;
        if (!(playerDeadFor < playerRespawnDelay)) {
            player->revive(playerSpawnPosition);
            playerWasDead = false;
            stats.playerRespawns++;
            Logger::logInfo("Player respawned");
        }
    }
}

void BoardLoop::render(Time timeDiff)
{
    if (combatHud.has_value()) {
        combatHud.value()->refresh();
    }
    boardRenderer->drawBoard(timeDiff);
    window->renderUI();
    window->updateRenderer();
    stats.framesRendered++;
}

/**
 * @brief Runs the main game loop.
 *
 * Shows the touch controls (Android or --touch-ui), then repeatedly processes input,
 * updates the game and renders ~60 times per second until the window is closed or
 * @p runFor game time elapsed.
 */
BoardLoop::Stats BoardLoop::start(std::optional<Time> runFor)
{
    GeneralTimer generalTimer;
    generalTimer.updateTime();
    Time lastUpdate(generalTimer.getTime());
    Time lastGraphicUpdate(generalTimer.getTime());
    const Time frameTime(16);

    showTouchControls();
    if (!combatHud.has_value()) {
        auto hud = std::make_unique<CombatHudUI>(Rect<fr_pos_t>{ .35, .01, .3, .1 }, window.get(), player);
        combatHud = hud.get();
        window->addChild(std::move(hud));
    }

    while (!quitRequested) {
        generalTimer.updateTime();
        Time now = generalTimer.getTime();
        Time timeDiff = now - lastUpdate;
        lastUpdate = now;
        if (timeDiff > maxTimeStep) {
            timeDiff = maxTimeStep;
        }

        handleInput(timeDiff);
        if (paused) {
            SDL_Delay(50);
            continue;
        }
        update(timeDiff);

        if (!(now - lastGraphicUpdate < frameTime)) {
            render(now - lastGraphicUpdate);
            lastGraphicUpdate = now;
        }
        else {
            SDL_Delay(1); // the loop used to spin at 100% CPU between frames
        }

        if (runFor.has_value() && !(stats.gameTime < *runFor)) {
            break;
        }
    }
    return stats;
}

void BoardLoop::setAutoplay(bool enabled)
{
    autoplay = enabled;
}

const BoardLoop::Stats& BoardLoop::getStats() const
{
    return stats;
}

std::shared_ptr<Board> BoardLoop::getBoard() const
{
    return board;
}

std::shared_ptr<Player> BoardLoop::getPlayer() const
{
    return player;
}
