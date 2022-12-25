#include "BoardLoop.h"

BoardLoop::BoardLoop(shared_ptr<Window> window)
{
	this->window = window;
	auto map = make_unique<Map>(LandscapeTypes::GRASSLAND, MapTypes::GIERA, Directions::NORTH, 64, 64, 0);
	this->board = Board(map, nullptr);
}

void BoardLoop::start()
{
	bool loopGoing = 1;
	GeneralTimer generalTimer;
	generalTimer.updateTime();
	Time lastGraphicUpdate(generalTimer.getTime());
	while (loopGoing) {
		if (generalTimer.getTime() > lastGraphicUpdate + Time(16) ) {
			lastGraphicUpdate = generalTimer.getTime();
			
			board.boardRenderer.drawBoard();
			window->updateRenderer();
			Logger::logInfo((generalTimer.getTime()- lastGraphicUpdate).getTimeMs(),generalTimer.getTime().getTimeMs(),lastGraphicUpdate.getTimeMs());
		}
		generalTimer.updateTime();
	}
}
