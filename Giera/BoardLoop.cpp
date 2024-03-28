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
	for (int i = 0; i < map->getSizeX();i++) {
		for (int j = 0; j < map->getSizeY();j++) {
			boardRenderer->addDrawableBoardEntity(map->getMapTile(Coordinates(i, j)));
		}
	}
	this->board = make_shared<Board>(std::move(map), boardRenderer);
	board->addItem(Coordinates(5, 0), BaseItemHandler::generate<Food>(ItemTypes::FOOD, (int)FoodTypes::BERRIES));
	board->addNPC(player);

	for (int i = 0; i < (int)MouseButtonTypes::COUNT;i++) {
		mouseButtonStates[i] = MouseButtonStateTypes::NOT_PRESSED;
	}
	player->addDrawableObserver(boardRenderer);
}

void BoardLoop::handleInput(Time timeDiff) {
	SDL_Event event;
	for (int i = 0; i < (int)MouseButtonTypes::COUNT;i++) {
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
		{Logger::logInfo((int)event.button.button, " button up");
		int buttonUpID = event.button.button == 1 ?
			(int)MouseButtonTypes::LEFT : (int)MouseButtonTypes::RIGHT;
		mouseButtonStates[buttonUpID] = MouseButtonStateTypes::JUST_RELEASED;}
			break;
		case SDL_MOUSEBUTTONDOWN:
		{Logger::logInfo((int)event.button.button, " button down");
		int buttonDownID = event.button.button == 1 ?
			(int)MouseButtonTypes::LEFT : (int)MouseButtonTypes::RIGHT;
		mouseButtonStates[buttonDownID] = MouseButtonStateTypes::JUST_PRESSED;}
			break;
		}
	}

	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	window->handleMouseInput(UIElement::MouseEventTypes::HOVER, make_pair(mouseX, mouseY), timeDiff);
	if (mouseButtonStates[(int)MouseButtonTypes::LEFT] == MouseButtonStateTypes::JUST_PRESSED) {
		window->handleMouseInput(UIElement::MouseEventTypes::PRESS_LEFT, make_pair(mouseX, mouseY), timeDiff);
	}
	if (mouseButtonStates[(int)MouseButtonTypes::RIGHT] == MouseButtonStateTypes::JUST_PRESSED) {
		window->handleMouseInput(UIElement::MouseEventTypes::PRESS_RIGHT, make_pair(mouseX, mouseY), timeDiff);
	}

	if (mouseButtonStates[(int)MouseButtonTypes::LEFT] == MouseButtonStateTypes::NOT_PRESSED) {
		boardRenderer->getCamera().resetSecondaryTarget();
	}
	else {
		Camera& camera = boardRenderer->getCamera();
		camera.setMouseAsSecondaryTarget();
	}

	for (auto& key : keySet) {
		PlayerActionTypes action = inputConfig->getActionType(key);
		switch (action) {
			using PlAct = PlayerActionTypes;
		case PlAct::MOVE_LEFT:
			player->move(Position(-((double)timeDiff.getTimeMs() * 4.0 / 1000.0), 0, 0));
			break;
		case PlAct::MOVE_DOWN:
			player->move(Position(0, ((double)timeDiff.getTimeMs() * 4.0 / 1000.0), 0));
			break;
		case PlAct::MOVE_UP:
			player->move(Position(0, -((double)timeDiff.getTimeMs() * 4.0 / 1000.0), 0));
			break;
		case PlAct::MOVE_RIGHT:
			player->move(Position(((double)timeDiff.getTimeMs() * 4.0 / 1000.0), 0, 0));
			break;
		case PlAct::ADD_SECONDARY_CAMERA_TARGET:
			boardRenderer->getCamera().setSecondaryTarget(board->getMap()->getMapTile(Coordinates(60, 60)));
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
		if (board->getProjectiles().empty()) {
			board->addProjectile(make_shared <SpellProjectile>(
				make_shared<FlightPath>(Position(1.5, 10.7, 0.1),
					Position(Calculator::getRandomInt(15, 20), 10.7, 0.1),
					1, 2 * Calculator::getRandomInt(5, 17)), make_shared<ThrownSpell>(), player));
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
