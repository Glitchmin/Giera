#pragma once
#include "Board.h"
#include "Window.h"
#include "InputConfig.h"
using std::set;
class BoardLoop
{
public:
	
	BoardLoop(shared_ptr<Window> window, shared_ptr<InputConfig> inputConfig);
	void start();

protected:
	Board board;
	set <SDL_Scancode> keySet;
	shared_ptr<InputConfig> inputConfig;
	shared_ptr<Window> window;
	shared_ptr<BoardRenderer> boardRenderer;
	void handleInput(Time timeDiff);
};

