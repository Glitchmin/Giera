#include "BoardRenderer.h"
#include <algorithm>

BoardRenderer::BoardRenderer() {
	
}
BoardRenderer::BoardRenderer(unsigned int sizeX, unsigned int sizeY, double pixelsPerMeter)
{
	boardTexture = TextureLoader::makeUniColorTexture(sizeX * pixelsPerMeter, sizeY * pixelsPerMeter, {0,0,0,255});
	this->pixelsPerMeter = pixelsPerMeter;
	leftUpperCameraPosition = Position(0, 0, 0);
}

void BoardRenderer::drawBoard()
{
	SDL_RenderClear(Texture::getRenderer());
	for (auto& it : drawablesSet) {
		it.getSprite().lock()->draw(*boardTexture, pixelsPerMeter, it.getPos() - leftUpperCameraPosition);
	}
	Texture generalTexture(NULL);
	boardTexture->draw(generalTexture, {0,0,boardTexture->getSize().first,boardTexture->getSize().second},
		{ 0,0,800, 640 });
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

void BoardRenderer::addToCameraPos(Position pos)
{
	leftUpperCameraPosition = leftUpperCameraPosition + pos;
}
void BoardRenderer::setCameraPos(Position pos)
{
	leftUpperCameraPosition = pos;
}
