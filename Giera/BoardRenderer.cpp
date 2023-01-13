#include "BoardRenderer.h"
#include <algorithm>

BoardRenderer::BoardRenderer() {
	
}
BoardRenderer::BoardRenderer(unsigned int sizeX, unsigned int sizeY, 
	shared_ptr<Window> window, double horizontalViewRangeM)
{
	this->window = window;
	pixelsPerMeter = window->getSize().first / horizontalViewRangeM;
	double verticalViewRangeM = horizontalViewRangeM / window->getXToYRatio();
	boardTexture = TextureLoader::makeUniColorTexture(sizeX * pixelsPerMeter, sizeY * pixelsPerMeter, {0,0,0,255});
	leftUpperCameraPosition = Position(0, 0, 0);
	rightLowerCameraPosition = Position(horizontalViewRangeM, verticalViewRangeM, 0);
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
	boardTexture->draw(generalTexture, {(int)(leftUpperCameraPosition.getX()*pixelsPerMeter)
		,(int)(leftUpperCameraPosition.getY() * pixelsPerMeter),window->getSize().first, window->getSize().second},
		{ 0,0,window->getSize().first, window->getSize().second });
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
