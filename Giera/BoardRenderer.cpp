#include "BoardRenderer.h"
#include "AbstractMapElement.h"
#include <algorithm>
using std::make_pair;
BoardRenderer::BoardRenderer() {

}
BoardRenderer::BoardRenderer(unsigned int sizeX, unsigned int sizeY,
	shared_ptr<Window> window, double horizontalViewRangeM)
{
	this->window = window;
	pixelsPerMeter = window->getSize().first / horizontalViewRangeM;
	double verticalViewRangeM = horizontalViewRangeM / window->getXToYRatio();
	boardSizeM = make_pair(sizeX / AbstractMapElement::getTilesPerMeter(),
		sizeY / AbstractMapElement::getTilesPerMeter());
	boardTexture = TextureLoader::makeUniColorTexture(boardSizeM.first * pixelsPerMeter,
		boardSizeM.second * pixelsPerMeter, { 0,0,0,255 });
	leftUpperCameraPosition = Position(0, 0, 0);
	rightLowerCameraPosition = Position(horizontalViewRangeM, verticalViewRangeM, 0);
	viewRangeM = make_pair(horizontalViewRangeM, verticalViewRangeM);
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
	boardTexture->draw(generalTexture, { (int)(leftUpperCameraPosition.getX() * pixelsPerMeter)
		,(int)(leftUpperCameraPosition.getY() * pixelsPerMeter),window->getSize().first, window->getSize().second },
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
	rightLowerCameraPosition = rightLowerCameraPosition + pos;
	if (leftUpperCameraPosition.getX() < 0) {
		leftUpperCameraPosition.setX(0);
		rightLowerCameraPosition.setX(boardSizeM.first);
	}
	if (rightLowerCameraPosition.getX() > boardSizeM.first) {
		rightLowerCameraPosition.setX(boardSizeM.first);
		leftUpperCameraPosition.setX(boardSizeM.first-viewRangeM.first);
	}
}
void BoardRenderer::setCameraPos(Position pos)
{
	leftUpperCameraPosition = pos;
}
