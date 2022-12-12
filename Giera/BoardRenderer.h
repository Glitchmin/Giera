#pragma once
#include "Texture.h"
#include "Board.h"
#include "Drawable.h"
class Board;


class BoardRenderer
{
public:
	void drawBoard();
private:
	std::multiset<pair<double,weak_ptr<Drawable>>> drawablesSet;
	Texture boardTexture;
	double pixelsPerMeter = 20;

};

