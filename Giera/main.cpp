#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <memory>
#include <sstream>
#include "Position.h"
#include "GeneralTimer.h"
#include "Time.h"
#include "BoardLoop.h"


int main( int argc, char* args[] )
{
	ios_base::sync_with_stdio(0);
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		Logger::logError ("SDL could not initialize", SDL_GetError() );
	}
	else{
		auto window = make_shared<Window>();
		auto inputConfig = make_shared<InputConfig>();
		inputConfig->setActionType(SDL_SCANCODE_A, PlayerActionTypes::MOVE_LEFT);
		inputConfig->setActionType(SDL_SCANCODE_D, PlayerActionTypes::MOVE_RIGHT);
		BoardLoop boardLoop(window, inputConfig);
		Logger::logInfo(window->getSize().first, "window", window->getSize().second);
		boardLoop.start();

	}


	Logger::logInfo("end of the program");
	Logger::close();
	SDL_Quit();
	return 0;
}