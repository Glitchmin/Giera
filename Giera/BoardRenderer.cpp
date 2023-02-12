#include "BoardRenderer.h"
#include <algorithm>
#include "Player.h"

using std::make_pair;

BoardRenderer::BoardRenderer() {

}
BoardRenderer::BoardRenderer(unsigned int sizeX, unsigned int sizeY,
	shared_ptr<Window> window, double horizontalViewRangeM, shared_ptr<Player> player)
{
	this->window = window;
	pixelsPerMeter = window->getSize().first / horizontalViewRangeM;
	double verticalViewRangeM = horizontalViewRangeM / window->getXToYRatio();
	layers.resize((int)Drawable::DrawableLayer::COUNT);
	for (auto& layer : layers) {
		layer.layerTexture = TextureLoader::makeUniColorTexture
		(sizeX * pixelsPerMeter, sizeY * pixelsPerMeter, { 0,0,0,0 });
	}
	camera = Camera(make_pair(horizontalViewRangeM, verticalViewRangeM),player);
}

void BoardRenderer::drawBoard(Time timeDiff)
{
	SDL_SetRenderTarget(Texture::getRenderer(), NULL);
	SDL_SetRenderDrawColor(Texture::getRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(Texture::getRenderer());
	camera.updatePosition();
	for (auto& layer : layers) {
		auto& drawablesSet = layer.drawablesSet;
		auto& layerTexture = layer.layerTexture;
		layer.layerTexture->fillWithColor({ 0,0,0,0 });
		for (auto& it : drawablesSet) {
			if ((it.getPos().getX()+1 >= camera.getLeftUpperPosition().getX() &&
				it.getPos().getX() <= camera.getLeftUpperPosition().getX() + camera.getViewRangeM().first)) {
				it.getSprite().lock()->draw(*layerTexture, pixelsPerMeter, it.getPos(), timeDiff);
			}
				
		}

		Texture generalTexture(NULL);
		layerTexture->draw(generalTexture, { (int)(camera.getLeftUpperPosition().getX() * pixelsPerMeter)
			,(int)(camera.getLeftUpperPosition().getY() * pixelsPerMeter),window->getSize().first, window->getSize().second },
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

Camera& BoardRenderer::getCamera()
{
	return camera;
}
void BoardRenderer::notify(DrawableBoardEntity* entity, Change change)
{
	if (change == Change::ADDED) {
		addDrawableBoardEntity(entity);
	}
	if (change == Change::REMOVED) {
		removeDrawableBoardEntity(entity);
	}
}

