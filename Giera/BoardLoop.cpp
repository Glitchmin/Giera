#include "BoardLoop.h"
#include "Player.h"
#include "BaseItemHandler.h"
#include "FlightPath.h"
#include "SpellProjectile.h"
#include "InventoryUI.h"
#include "InventoryInputHandler.h"


BoardLoop::BoardLoop(shared_ptr<Window> window, shared_ptr<InputConfig> inputConfig)
{
	this->window = window;
	this->inputConfig = inputConfig;
	auto map = make_unique<Map>(LandscapeTypes::GRASSLAND, MapTypes::GIERA, Directions::NORTH, 64, 64, 0);
	double viewRangeM = 20;
	player = make_shared<Player>();
	boardRenderer = make_shared<BoardRenderer>(map->getSizeX(), map->getSizeY(), window, viewRangeM, player);
	for (int i = 0; i < map->getSizeX(); i++) {
		for (int j = 0; j < map->getSizeY(); j++) {
			boardRenderer->addDrawableBoardEntity(map->getMapTile(Coordinates(i, j)));
		}
	}
	this->board = make_shared<Board>(std::move(map), boardRenderer);
	board->addItem(Coordinates(5, 0), BaseItemHandler::generate<Food>(ItemTypes::FOOD, (int)FoodTypes::BERRIES));
	board->addPlayerCharacter(player);
	for (int i = 0; i < (int)MouseButtonTypes::COUNT; i++) {
		mouseButtonStates[i] = MouseButtonStateTypes::NOT_PRESSED;
	}
}

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
			//Logger::logInfo("down", (int)event.key.keysym.scancode);
			if (keySet.find(event.key.keysym.scancode) == keySet.end()) {
				justPressedKeys.insert(event.key.keysym.scancode);
			}
			keySet.insert(event.key.keysym.scancode);
			break;
		case SDL_KEYUP:
			//Logger::logInfo("up", (int)event.key.keysym.scancode);
			keySet.erase(event.key.keysym.scancode);
			break;
		case SDL_MOUSEBUTTONUP:
		{
			Logger::logInfo((int)event.button.button, " button up");
			int buttonUpID = event.button.button == 1 ?
				(int)MouseButtonTypes::LEFT : (int)MouseButtonTypes::RIGHT;
			mouseButtonStates[buttonUpID] = MouseButtonStateTypes::JUST_RELEASED;
		}
		break;
		case SDL_MOUSEBUTTONDOWN:
		{
			Logger::logInfo((int)event.button.button, " button down");
			int buttonDownID = event.button.button == 1 ?
				(int)MouseButtonTypes::LEFT : (int)MouseButtonTypes::RIGHT;
			mouseButtonStates[buttonDownID] = MouseButtonStateTypes::JUST_PRESSED;
		}
		break;
		}
	}

	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	bool isMouseHandledByUI = window->handleMouseInput(UIElement::MouseEventTypes::HOVER, make_pair(mouseX, mouseY), timeDiff);

	if (mouseButtonStates[(int)MouseButtonTypes::LEFT] == MouseButtonStateTypes::JUST_PRESSED) {
		isMouseHandledByUI = window->handleMouseInput(UIElement::MouseEventTypes::PRESS_LEFT,
			make_pair(mouseX, mouseY), timeDiff) || isMouseHandledByUI;
	}
	if (mouseButtonStates[(int)MouseButtonTypes::RIGHT] == MouseButtonStateTypes::JUST_PRESSED) {
		isMouseHandledByUI = window->handleMouseInput(UIElement::MouseEventTypes::PRESS_RIGHT,
			make_pair(mouseX, mouseY), timeDiff) || isMouseHandledByUI;
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
				boardRenderer->getCamera().setSecondaryTarget(board->getMap()->getMapTile(Coordinates(60, 60)));
			}
			break;
		}
	}
	for (auto& key : justPressedKeys) {
		PlayerActionTypes action = inputConfig->getActionType(key);
		switch (action) {
			using PlAct = PlayerActionTypes;
		case PlAct::OPEN_EQUIPMENT:
			if (!playerInventoryUI.has_value()) {
				auto invUI = make_unique <InventoryUI>(window, player->getInventory(),
					make_shared<InventoryInputHandler>(player->getInventory()));
				playerInventoryUI = invUI.get();
				window->addChild(std::move(invUI));
			}
			else {
				window->removeChild(playerInventoryUI.value());
				playerInventoryUI = nullopt;
			}
			break;
		case PlAct::CLOSE_WINDOW:
			if (playerInventoryUI.has_value()) {
				window->removeChild(playerInventoryUI.value());
				playerInventoryUI = nullopt;
			}
			break;
		}

	}
}

void BoardLoop::start()
{
	bool loopGoing = true;
	GeneralTimer generalTimer;
	generalTimer.updateTime();
	Time lastGraphicUpdate(generalTimer.getTime());
	Time lastInputHandling(generalTimer.getTime());
	Time lastProjectileHandling(generalTimer.getTime());

	while (loopGoing) {
		Time inputTimeDiff = generalTimer.getTime() - lastInputHandling;
		lastInputHandling = generalTimer.getTime();
		handleInput(inputTimeDiff);
		
		if (board->getAiCharacters().empty()) {
			auto aiChar = make_shared<AiCharacter>(CharacterTypes::PLAYER, Position(14, 4.7, 0), 1);
			board->addAiCharacter(aiChar);
			//player character type is needed to obtain npc0 texture
		}
		else {
			auto aiChar = board->getAiCharacters().front();
			if (aiChar->getPosition().getY() < 10.7) {
				aiChar->move(Position(0, inputTimeDiff.getTimeS(), 0));
			}
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
