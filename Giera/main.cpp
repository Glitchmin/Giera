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


int main( int argc, char* args[] )
{
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		Logger::logError ("SDL could not initialize", SDL_GetError() );
	}
	else{
		SDL_Window* window = SDL_CreateWindow( "Giera", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 640, SDL_WINDOW_SHOWN );
		if( window == NULL ){
			Logger::logError( "Couldn't create window", SDL_GetError());
		}else{
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
			SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			TextureLoader::setRenderer(renderer);
			string textureName = "../../test.bmp";
			SDL_Rect dsRect{0,0,800,640};
			SDL_RenderCopy(renderer, TextureLoader::getTexturePtr(textureName)->getTexture(),NULL,&dsRect);
			SDL_RenderPresent(renderer);
			//Wait two seconds
			SDL_Delay( 2000 );
			SDL_DestroyWindow( window );
		}
	}


	SDL_Quit();
	Logger::logInfo("end of the program");
	Logger::close();
	return 0;
}