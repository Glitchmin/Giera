#include "pch.h"

#include "BoardLoop.h"


int main( int argc, char* args[] )
{
	ios_base::sync_with_stdio(0);
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		Logger::logError ("SDL could not initialize", SDL_GetError() );
	}
	else{
		if (TTF_Init() == -1) {
			Logger::logError("TTF could not initialize", TTF_GetError());
		}
		auto window = make_shared<Window>();
		auto inputConfig = make_shared<InputConfig>();
		inputConfig->setActionType(SDL_SCANCODE_A, PlayerActionTypes::MOVE_LEFT);
		inputConfig->setActionType(SDL_SCANCODE_D, PlayerActionTypes::MOVE_RIGHT);
		inputConfig->setActionType(SDL_SCANCODE_W, PlayerActionTypes::MOVE_UP);
		inputConfig->setActionType(SDL_SCANCODE_S, PlayerActionTypes::MOVE_DOWN);
		inputConfig->setActionType(SDL_SCANCODE_C, PlayerActionTypes::ADD_SECONDARY_CAMERA_TARGET);
		inputConfig->setActionType(SDL_SCANCODE_I, PlayerActionTypes::OPEN_EQUIPMENT);
		inputConfig->setActionType(SDL_SCANCODE_ESCAPE, PlayerActionTypes::CLOSE_WINDOW);
		BoardLoop boardLoop(window, inputConfig);
		Logger::logInfo(window->getSize().first, "window", window->getSize().second);
		boardLoop.start();

	}


	Logger::logInfo("end of the program");
	Logger::close();
	SDL_Quit();
	return 0;
}