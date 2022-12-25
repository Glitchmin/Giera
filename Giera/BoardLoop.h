#pragma once
#include "Board.h"
#include "Window.h"
#include "InputConfig.h"
class BoardLoop
{
public:
	
	BoardLoop(shared_ptr<Window> window, shared_ptr<InputConfig> inputConfig);
	void start();

protected:
	Board board;
	shared_ptr<InputConfig> inputConfig;
	shared_ptr<Window> window;
	void handleInput();
};

