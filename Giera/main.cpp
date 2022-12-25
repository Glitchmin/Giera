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
		BoardLoop boardLoop(window);
		boardLoop.start();

		getchar();
	}


	Logger::logInfo("end of the program");
	Logger::close();
	SDL_Quit();
	return 0;
}