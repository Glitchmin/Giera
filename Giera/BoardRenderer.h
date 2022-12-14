#pragma once
#include "Texture.h"
#include "Board.h"
#include "DrawableBoardEntity.h"
class Board;

using drawables_multiset_t = typename std::multiset<Drawable>;

class BoardRenderer
{
public:
	void drawBoard();
	void addDrawableBoardEntity(shared_ptr<DrawableBoardEntity> entity);
	void removeDrawableBoardEntity(shared_ptr<DrawableBoardEntity> entity);
private:
	drawables_multiset_t drawablesSet;
	Texture boardTexture;
	double pixelsPerMeter = 20;

};
