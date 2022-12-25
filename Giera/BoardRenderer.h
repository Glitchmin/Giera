#pragma once
#include "Texture.h"
#include "DrawableBoardEntity.h"

using drawables_multiset_t = typename std::multiset<Drawable>;

class BoardRenderer
{
public:
	BoardRenderer();
	BoardRenderer(unsigned int sizeX, unsigned int sizeY, double pixelsPerMeter);
	void drawBoard();
	void addDrawableBoardEntity(shared_ptr<DrawableBoardEntity> entity);
	void removeDrawableBoardEntity(shared_ptr<DrawableBoardEntity> entity);
	void addToCameraPos(Position pos);
	void setCameraPos(Position pos);
private:
	drawables_multiset_t drawablesSet;
	shared_ptr<Texture> boardTexture;
	double pixelsPerMeter;
	Position leftUpperCameraPosition;

};

