/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
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

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 640;

int main( int argc, char* args[] )
{
	//The window we'll be rendering to
	SDL_Window* window = NULL;
	
	//The surface contained by the window
	SDL_Surface* screenSurface = NULL; 

	//Initialize SDL 
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		Logger::logError ("SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}
	else
	{
		window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( window == NULL )
		{
			Logger::logError( "Couldn't create window!", SDL_GetError());
		}
		else
		{
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
			SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			TextureLoader::setRenderer(renderer);
			string textureName = "../../test.bmp";
			SDL_Rect dsRect{0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
			SDL_RenderCopy(renderer, TextureLoader::getTexturePtr(textureName)->getTexture(),NULL,&dsRect);
			SDL_RenderPresent(renderer);
			//Wait two seconds
			SDL_Delay( 2000 );
		}
	}

	SDL_DestroyWindow( window );

	SDL_Quit();
	Logger::close();
	Logger::logInfo("end of the program");
	return 0;
}