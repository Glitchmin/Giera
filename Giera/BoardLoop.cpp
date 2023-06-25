#include "BoardLoop.h"
#include "Player.h"
#include "BaseItemHandler.h"
#include "FlightPath.h"
#include "SpellProjectile.h"
#include "InventoryUI.h"


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

	auto invUI = InventoryUI::createInventoryUI(window, player->getInventory());

	leftMouseButtonPressed = false;
	player->addDrawableObserver(boardRenderer);
}

void BoardLoop::handleInput(Time timeDiff) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			Logger::logInfo("user closed the window");
			break;
		case SDL_KEYDOWN:
			//Logger::logInfo("down", (int)event.key.keysym.scancode);
			keySet.insert(event.key.keysym.scancode);
			break;
		case SDL_KEYUP:
			//Logger::logInfo("up", (int)event.key.keysym.scancode);
			keySet.erase(event.key.keysym.scancode);
			break;
		case SDL_MOUSEBUTTONUP:
			Logger::logInfo((int)event.button.button, " button up");
			leftMouseButtonPressed = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			Logger::logInfo((int)event.button.button, " button down");
			leftMouseButtonPressed = true;
			break;
		}
	}
	if (!leftMouseButtonPressed) {
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

}

void BoardLoop::start()
{
	bool loopGoing = 1;
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
					1, 2*Calculator::getRandomInt(5, 17)), make_shared<ThrownSpell>(),player));
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
