#pragma once
#include "Texture.h"
#include "DrawableBoardEntity.h"
#include "Window.h"
#include "DrawableEntityObserver.h"
#include "Camera.h"

using drawables_multiset_t = typename std::multiset<Drawable>;

class Player;

class BoardRenderer: public DrawableEntityObserver
{
public:
	BoardRenderer();
	BoardRenderer(unsigned int sizeX, unsigned int sizeY,shared_ptr<Window> window,
		double horizontalViewRangeM, shared_ptr<Player> player);
	void drawBoard(Time timeDiff);
	void addDrawableBoardEntity(shared_ptr<DrawableBoardEntity> entity);
	void removeDrawableBoardEntity(shared_ptr<DrawableBoardEntity> entity);
	Camera& getCamera();
	virtual void notify(DrawableBoardEntity* entity, Change change) override;
private:
	class Layer {
	public:
		drawables_multiset_t drawablesSet;
		shared_ptr<Texture> layerTexture;
	};
	vector <Layer> layers;
	shared_ptr<Window> window;
	double pixelsPerMeter;
	Camera camera;

	void addDrawableBoardEntity(DrawableBoardEntity* entity);
	void removeDrawableBoardEntity(DrawableBoardEntity* entity);
};

