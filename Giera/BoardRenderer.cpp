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
	camera = Camera(make_pair(horizontalViewRangeM, verticalViewRangeM),
		player, make_pair(sizeX, sizeY), window);
}

void BoardRenderer::drawBoard(Time timeDiff)
{
	SDL_SetRenderTarget(Texture::getRenderer(), NULL);
	SDL_SetRenderDrawColor(Texture::getRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(Texture::getRenderer());
	camera.updatePosition(timeDiff);
	for (auto& layer : layers) {
		auto& drawablesMap = layer.drawablesMap;
		auto& layerTexture = layer.layerTexture;
		layer.layerTexture->fillWithColor({ 0,0,0,0 });
		for (auto& [pos, it] : layer.drawablesMap) {
			if ((it->getPos().getX() + 1 >= camera.getLeftUpperPosition().getX() &&
				it->getPos().getX()-it->getSizeXY().first/2 <= camera.getLeftUpperPosition().getX() + camera.getViewRangeM().first)) {
				it->updateCurrentState(timeDiff);
				it->draw(*layerTexture, pixelsPerMeter);
			}

		}

		Texture generalTexture(NULL);
		layerTexture->draw(generalTexture, (SDL_Rect{ (int)(camera.getLeftUpperPosition().getX() * pixelsPerMeter)
			,(int)(camera.getLeftUpperPosition().getY() * pixelsPerMeter),window->getSize().first, window->getSize().second }),
			(SDL_Rect{ 0,0,window->getSize().first, window->getSize().second }));
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
	mapMutex.lock();
	for (auto& drawable : entity->getDrawables()) {
		layers[(int)drawable->getDrawableLayer()].drawablesMap.emplace(drawable->getPos(), drawable);
	}
	mapMutex.unlock();
}

void BoardRenderer::removeDrawableBoardEntity(DrawableBoardEntity* entity)
{
	mapMutex.lock();
	for (auto& drawable : entity->getDrawables()) {
		auto& map = layers[(int)drawable->getDrawableLayer()].drawablesMap;
		auto itLB = map.lower_bound(drawable->getPos());
		auto itLB2 = itLB;
		while ( !(itLB == map.end() || (itLB->second == drawable))) {
			itLB++;
		}
		map.erase(itLB);
	}
	mapMutex.unlock();
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

void BoardRenderer::lockMutex()
{
	mapMutex.lock();
}
void BoardRenderer::unlockMutex()
{
	mapMutex.unlock();
}

