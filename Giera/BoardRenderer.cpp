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
	layers.resize((int)Drawable::DrawableLayer::COUNT);
	for (auto& layer : layers) {
		layer.requiresUpdate = true;
		layer.layerTexture = TextureLoader::makeUniColorTexture
		(sizeX * pixelsPerMeter, sizeY * pixelsPerMeter, { 0,0,0,0 });
	}
	leftUpperCameraPosition = Position(0, 0, 0);
	rightLowerCameraPosition = Position(horizontalViewRangeM, verticalViewRangeM, 0);
}

void BoardRenderer::drawBoard(Time timeDiff)
{
	SDL_SetRenderTarget(Texture::getRenderer(), NULL);
	SDL_SetRenderDrawColor(Texture::getRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(Texture::getRenderer());
	for (auto& layer : layers) {
		auto& drawablesSet = layer.drawablesSet;
		auto& layerTexture = layer.layerTexture;
		if (layer.requiresUpdate) {
			layer.requiresUpdate = 0;
			layer.layerTexture->fillWithColor({ 0,0,0,0 });
			for (auto& it : drawablesSet) {
				it.getSprite().lock()->draw(*layerTexture, pixelsPerMeter, it.getPos(), timeDiff);
			}
		}
		Texture generalTexture(NULL);
		layerTexture->draw(generalTexture, { (int)(leftUpperCameraPosition.getX() * pixelsPerMeter)
			,(int)(leftUpperCameraPosition.getY() * pixelsPerMeter),window->getSize().first, window->getSize().second },
			{ 0,0,window->getSize().first, window->getSize().second });
	}
}



void BoardRenderer::addDrawableBoardEntity(shared_ptr<DrawableBoardEntity> entity)
{
	addDrawableBoardEntity(entity.get());
}

void BoardRenderer::removeDrawableBoardEntity(shared_ptr<DrawableBoardEntity> entity)
{
	removeDrawableBoardEntity(entity.get());
}

void BoardRenderer::addDrawableBoardEntity(DrawableBoardEntity* entity)
{
	for (auto& drawable : entity->getDrawables()) {
		layers[(int)drawable.getDrawableLayer()].drawablesSet.insert(drawable);
	}
}
void BoardRenderer::removeDrawableBoardEntity(DrawableBoardEntity* entity)
{
	for (auto& drawable : entity->getDrawables()) {
		layers[(int)drawable.getDrawableLayer()].drawablesSet.erase(drawable);
	}
}

void BoardRenderer::addToCameraPos(Position pos)
{
	leftUpperCameraPosition = leftUpperCameraPosition + pos;
}
void BoardRenderer::notify(DrawableBoardEntity* entity, Change change)
{
	if (change == Change::ADDED) {
		addDrawableBoardEntity(entity);
	}
	if (change == Change::REMOVED) {
		removeDrawableBoardEntity(entity);
	}
	for (auto& drawable : entity->getDrawables()) {
		layers[(int)drawable.getDrawableLayer()].requiresUpdate = 1;
	}
}

void BoardRenderer::setCameraPos(Position pos)
{
	leftUpperCameraPosition = pos;
}
