#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <memory>
#include <sstream>
#include "Position.h"
#include "GeneralTimer.h"
#include "Time.h"
#include "Logger.h"
#include "Coordinates.h"
#include "Map.h"
#include "GrasslandsGenerator.h"
#include "MapFileHandler.h"
#include "BaseItemHandler.h"
#include "TextureLoader.h"
#include "Window.h"


int main( int argc, char* args[] )
{
	ios_base::sync_with_stdio(0);
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		Logger::logError ("SDL could not initialize", SDL_GetError() );
	}
	else{
		auto window = make_shared<Window>();
		auto map = make_unique<Map>(LandscapeTypes::GRASSLAND, MapTypes::GIERA, Directions::NORTH, 64, 64, 0);
		Board board(map,nullptr);
		board.boardRenderer.drawBoard();
		window->updateRenderer();

		getchar();
	}


	Logger::logInfo("end of the program");
	Logger::close();
	SDL_Quit();
	return 0;
}