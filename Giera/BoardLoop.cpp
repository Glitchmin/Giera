#include "BoardLoop.h"

BoardLoop::BoardLoop(shared_ptr<Window> window, shared_ptr<InputConfig> inputConfig)
{
	this->window = window;
	this->inputConfig = inputConfig;
	auto map = make_unique<Map>(LandscapeTypes::GRASSLAND, MapTypes::GIERA, Directions::NORTH, 64, 64, 0);
	double viewRangeM = 20;
	auto player = make_shared<AbstractNPC>();
	boardRenderer = make_shared<BoardRenderer>(map->getSizeX(), map->getSizeY(), window, viewRangeM);
	for (int i = 0; i < map->getSizeX();i++) {
		for (int j = 0; j < map->getSizeY();j++) {
			boardRenderer->addDrawableBoardEntity(map->getMapTile(Coordinates(i, j)));
		}
	}
	player->addObserver(boardRenderer);
	this->board = Board(map, player , boardRenderer);
}

void BoardLoop::handleInput(Time timeDiff) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			Logger::logInfo("user closed the window");
		}
		if (event.type == SDL_KEYDOWN) {
			Logger::logInfo("down", (int)event.key.keysym.scancode);
			keySet.insert(event.key.keysym.scancode);
		}
		if (event.type == SDL_KEYUP) {
			Logger::logInfo("up", (int)event.key.keysym.scancode);
			keySet.erase(event.key.keysym.scancode);
		}
	}
	for (auto& key : keySet) {
		PlayerActionTypes action = inputConfig->getActionType(key);
		switch (action) {
			using PlAct = PlayerActionTypes;
		case PlAct::MOVE_LEFT:
			boardRenderer->addToCameraPos(Position(-timeDiff.getTimeS() * 4.0, 0, 0));
			break;
		case PlAct::MOVE_RIGHT:
			boardRenderer->addToCameraPos(Position(timeDiff.getTimeS() * 4.0, 0, 0));
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
		handleInput(timeDiff);
		lastInputHandling = generalTimer.getTime();
		if (generalTimer.getTime() > lastGraphicUpdate + Time(16)) {
			lastGraphicUpdate = generalTimer.getTime();

			boardRenderer->drawBoard(timeDiff);
			window->updateRenderer();
			//Logger::logInfo((generalTimer.getTime()- lastGraphicUpdate).getTimeMs(),generalTimer.getTime().getTimeMs(),lastGraphicUpdate.getTimeMs());
		}
		generalTimer.updateTime();
	}
}
