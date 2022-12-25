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
	SDL_SetRenderTarget(Texture::getRenderer(), NULL);
	SDL_SetRenderDrawColor(Texture::getRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(Texture::getRenderer());
	for (auto& it : drawablesSet) {
		it.getSprite().lock()->draw(*boardTexture, pixelsPerMeter, it.getPos());
	}
	Texture generalTexture(NULL);
	boardTexture->draw(generalTexture, {0,0,boardTexture->getSize().first,boardTexture->getSize().second},
		{ (int)(leftUpperCameraPosition.getX()*pixelsPerMeter),(int)(leftUpperCameraPosition.getY()*pixelsPerMeter),800, 640 });
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
