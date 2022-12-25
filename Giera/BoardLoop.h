#pragma once
#include "Board.h"
#include "Window.h"
class BoardLoop
{
public:
	
	BoardLoop(shared_ptr<Window> window);
	void start();

protected:
	Board board;
	shared_ptr<Window> window;
};

