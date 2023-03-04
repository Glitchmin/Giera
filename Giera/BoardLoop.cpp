#include "BoardLoop.h"
#include "Player.h"
#include "BaseItemHandler.h"

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
	this->board = make_shared<Board>(map, boardRenderer);
	board->addItem(Coordinates(5, 0), BaseItemHandler::generate<Food>(ItemTypes::FOOD, (int)FoodTypes::BERRIES));
	board->addNPC(player);
	player->addObserver(boardRenderer);
}

void BoardLoop::handleInput(Time timeDiff) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			Logger::logInfo("user closed the window");
		}
		if (event.type == SDL_KEYDOWN) {
			//Logger::logInfo("down", (int)event.key.keysym.scancode);
			keySet.insert(event.key.keysym.scancode);
		}
		if (event.type == SDL_KEYUP) {
			//Logger::logInfo("up", (int)event.key.keysym.scancode);
			keySet.erase(event.key.keysym.scancode);
		}
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
			boardRenderer->getCamera().setSecondaryTarget(board->getMap()->getMapTile(Coordinates(60,60)));
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

	while (loopGoing) {
		Time timeDiff = generalTimer.getTime() - lastInputHandling;
		lastInputHandling = generalTimer.getTime();
		handleInput(timeDiff);
		if (generalTimer.getTime() > lastGraphicUpdate + Time(16)) {
			Time renderTimeDiff = generalTimer.getTime() - lastGraphicUpdate;
			lastGraphicUpdate = generalTimer.getTime();

			boardRenderer->drawBoard(renderTimeDiff);
			window->updateRenderer();
			//Logger::logInfo((generalTimer.getTime()- lastGraphicUpdate).getTimeMs(),generalTimer.getTime().getTimeMs(),lastGraphicUpdate.getTimeMs());
		}
		generalTimer.updateTime();
	}
}
