#pragma once
#include "Board.h"
#include "Window.h"
#include "InputConfig.h"
#include "MouseButtonStateTypes.h"
#include <set>
using std::set;

class Player;

class BoardLoop
{
public:
	
	BoardLoop(shared_ptr<Window> window, shared_ptr<InputConfig> inputConfig);
	void start();

protected:
	shared_ptr<Board> board;
	set <SDL_Scancode> keySet;
	array<MouseButtonStateTypes, (int)MouseButtonTypes::COUNT> mouseButtonStates;
	shared_ptr<InputConfig> inputConfig;
	shared_ptr<Window> window;
	shared_ptr<BoardRenderer> boardRenderer;
	shared_ptr<Player> player;
	void handleInput(Time timeDiff);
};

