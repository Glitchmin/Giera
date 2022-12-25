#include "BoardLoop.h"

BoardLoop::BoardLoop(shared_ptr<Window> window, shared_ptr<InputConfig> inputConfig)
{
	this->window = window;
	this->inputConfig = inputConfig;
	auto map = make_unique<Map>(LandscapeTypes::GRASSLAND, MapTypes::GIERA, Directions::NORTH, 64, 64, 0);
	this->board = Board(map, nullptr);
}

void BoardLoop::handleInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN) {
			SDL_Keycode key = event.key.keysym.scancode;
			PlayerActionTypes action = inputConfig->getActionType(key);
			Logger::logInfo(key, (int)action);
			switch (action) {
				using PlAct = PlayerActionTypes;
			case PlAct::MOVE_LEFT:
				board.getBoardRenderer().addToCameraPos(Position(-0.5, 0, 0));
				break;
			case PlAct::MOVE_RIGHT:
				board.getBoardRenderer().addToCameraPos(Position(0.5, 0, 0));
				break;
			}
		}
	}
}

void BoardLoop::start()
{
	bool loopGoing = 1;
	GeneralTimer generalTimer;
	generalTimer.updateTime();
	Time lastGraphicUpdate(generalTimer.getTime());

	while (loopGoing) {
		handleInput();
		if (generalTimer.getTime() > lastGraphicUpdate + Time(16)) {
			lastGraphicUpdate = generalTimer.getTime();

			board.getBoardRenderer().drawBoard();
			window->updateRenderer();
			//Logger::logInfo((generalTimer.getTime()- lastGraphicUpdate).getTimeMs(),generalTimer.getTime().getTimeMs(),lastGraphicUpdate.getTimeMs());
		}
		generalTimer.updateTime();
	}
}
