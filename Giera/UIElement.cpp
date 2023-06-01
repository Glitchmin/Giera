#include "UIElement.h"

UIElement::UIElement(rel_pos_t relX, rel_pos_t relY, rel_pos_t relSizeX, rel_pos_t relSizeY,
	shared_ptr<Texture> texture, shared_ptr<UIElement> parent) :
	texture(texture), parent(parent)
{
	relativePos = { relX, relY };
	relativeSize = { relSizeX, relSizeY };
	realPos = { (int)(relSizeX * parent->realPos[0]), (int)(relSizeY * parent->realPos[0]), };
	realSize = { (int)(relSizeX * parent->realSize[0]), (int)(relSizeY * parent->realSize[0]), };
}

void UIElement::render(shared_ptr <Texture>& textureToDrawOn)
{
	SDL_Rect srcRect{ realPos[0], realPos[1], realSize[0], realSize[1] };
	SDL_Rect dstRect{ 0,0, texture->getSize().first, texture->getSize().second };
	texture->draw(*textureToDrawOn,srcRect, dstRect);
}
