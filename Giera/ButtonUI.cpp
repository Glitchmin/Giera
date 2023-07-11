#include "ButtonUI.h"
using std::min;

ButtonUI::ButtonUI(Rect<rel_pos_t> relativePosRect, shared_ptr<Texture> texture, UIElement* parent, double relativeEdgeThickness)
	:UIElement(relativePosRect, texture, parent), relativeEdgeThickness(relativeEdgeThickness), edgeTransparency(0)
{
	edgesTexture = 
		TextureLoader::makeUniColorTexture(realPosRect.getSize()[0], realPosRect.getSize()[1], { 0,0,0,0 });
	fillEdgesTexture();
	
}

void ButtonUI::fillEdgesTexture()
{
	int edgeSize = (relativeEdgeThickness * realPosRect.getSize()[1] +
		relativeEdgeThickness * realPosRect.getSize()[0] + 1) / 2;
	SDL_SetRenderTarget(Texture::getRenderer(), edgesTexture->getTexture());
	SDL_SetRenderDrawColor(Texture::getRenderer(), 255, 255, 255, 255);
	int texSizeX = edgesTexture->getSize().first;
	int texSizeY = edgesTexture->getSize().second;
	for (int i = 0; i < edgeSize;i++) {
		SDL_RenderDrawLine(Texture::getRenderer(), i, 0, i, texSizeY);
		SDL_RenderDrawLine(Texture::getRenderer(), texSizeX - i, 0, texSizeX - i, texSizeY);
		SDL_RenderDrawLine(Texture::getRenderer(), 0, i, texSizeX, i);
		SDL_RenderDrawLine(Texture::getRenderer(), 0, texSizeY - i, texSizeX, texSizeY - i);
	}
}

bool ButtonUI::handleMouseInput(MouseButtonTypes mouseButtonType, pair<int, int> pos)
{
	edgeTransparency+=5;
	Logger::logWarning("tmp", edgeTransparency);
	return true;
}

void ButtonUI::render(shared_ptr<Texture>& textureToDrawOn)
{
	UIElement::render(textureToDrawOn);
	SDL_SetTextureAlphaMod(edgesTexture->getTexture(),min(edgeTransparency, maxEdgeTransparency));
	edgesTexture->draw(*textureToDrawOn, { 0,0,edgesTexture->getSize().first, edgesTexture->getSize().second },
		{ 0,0,realPosRect.getSize()[0],realPosRect.getSize()[1]});

}


