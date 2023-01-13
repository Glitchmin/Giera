#pragma once
#include "Texture.h"
#include "DrawableBoardEntity.h"
#include "Window.h"

using drawables_multiset_t = typename std::multiset<Drawable>;

class BoardRenderer
{
public:
	BoardRenderer();
	BoardRenderer(unsigned int sizeX, unsigned int sizeY,
		shared_ptr<Window> window, double horizontalViewRangeM);
	void drawBoard();
	void addDrawableBoardEntity(shared_ptr<DrawableBoardEntity> entity);
	void removeDrawableBoardEntity(shared_ptr<DrawableBoardEntity> entity);
	void addToCameraPos(Position pos);
	void setCameraPos(Position pos);
private:
	drawables_multiset_t drawablesSet;
	shared_ptr<Texture> boardTexture;
	shared_ptr<Window> window;
	double pixelsPerMeter;
	Position leftUpperCameraPosition;
	Position rightLowerCameraPosition;
};

