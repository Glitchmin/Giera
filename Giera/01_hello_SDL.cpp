/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <memory>
#include "Position.h"
#include "GeneralTimer.h"
#include "Time.h"
#include "Logger.h"
#include "Coordinates.h"
#include "Map.h"
#include "GrasslandsGenerator.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main( int argc, char* args[] )
{
	std::shared_ptr <Map> map1(new Map(LandscapeTypes::GRASSLAND,MapTypes::QUEST_MAP, Directions::UP,10,10,50));
	GrasslandsGenerator generator;
	generator.generateMap(map1);
	GeneralTimer generalTimer;
	unsigned int time = SDL_GetTicks();
	while (SDL_GetTicks() < time + 200)
	{
		generalTimer.updateTime();
	}
	//The window we'll be rendering to
	SDL_Window* window = NULL;
	
	//The surface contained by the window
	SDL_Surface* screenSurface = NULL; 

	//Initialize SDL 
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}
	else
	{
		//Create window
		window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( window == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get window surface
			screenSurface = SDL_GetWindowSurface( window );

			//Fill the surface white
			SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
			
			//Update the surface
			SDL_UpdateWindowSurface( window );

			//Wait two seconds
			SDL_Delay( 2000 );
		}
	}

	//Destroy window
	SDL_DestroyWindow( window );

	//Quit SDL subsystems
	SDL_Quit();
	Logger::close();
	return 0;
}