#include "BoardRenderer.h"
#include <algorithm>

BoardRenderer::BoardRenderer() {

}
BoardRenderer::BoardRenderer(unsigned int sizeX, unsigned int sizeY, double pixelsPerMeter)
{
	boardTexture = TextureLoader::makeUniColorTexture(sizeX * pixelsPerMeter, sizeY * pixelsPerMeter, {0,0,0,255});
}

void BoardRenderer::drawBoard()
{
	for (auto& it : drawablesSet) {
		it.getSprite().lock()->draw(*boardTexture, pixelsPerMeter, it.getPos());
	}
}

void BoardRenderer::addDrawableBoardEntity(shared_ptr<DrawableBoardEntity> entity)
{
	for (auto& drawable : entity->getDrawables()) {
		drawablesSet.insert(drawable);
	}
}

void BoardRenderer::removeDrawableBoardEntity(shared_ptr<DrawableBoardEntity> entity)
{
	for (auto& drawable : entity->getDrawables()) {
		drawablesSet.erase(drawable);
	}
}
