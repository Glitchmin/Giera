#include "UIElement.h"
#include "TextureLoader.h"

UIElement::UIElement(rel_pos_t relX, rel_pos_t relY, rel_pos_t relSizeX, rel_pos_t relSizeY,
	shared_ptr<Texture> texture, UIElement* parent) :
	texture(texture), parent(parent)
{
	relativePos = { relX, relY };
	relativeSize = { relSizeX, relSizeY };
	realPos = { (int)(relX * parent->realSize[0]+parent->realPos[0]),
		(int)(relY * parent->realSize[1] + parent->realPos[1]), };
	realSize = { (int)(relSizeX * parent->realSize[0]), (int)(relSizeY * parent->realSize[1]) };
}

UIElement::UIElement(real_pos_t realX, real_pos_t realY, real_pos_t realSizeX, real_pos_t realSizeY)
	:parent(nullptr), texture(nullptr)
{
	realPos = { (int)(realX), (int)(realY) };
	realSize = { (int)realSizeX, (int)(realSizeY) };
}

void UIElement::render(shared_ptr <Texture>& textureToDrawOn)
{
	for (auto& child : children) {
		child->render(texture);
	}
	SDL_Rect dstRect{ realPos[0], realPos[1], realSize[0], realSize[1] };
	SDL_Rect srcRect{ 0,0, texture->getSize().first, texture->getSize().second };
	texture->draw(*textureToDrawOn, srcRect, dstRect);
}

void UIElement::addChild(unique_ptr<UIElement> child)
{
	children.push_back(std::move(child));
}

array<rel_pos_t,2> UIElement::getRelativePos() const
{
    return relativePos;
}

void UIElement::setRelativePos(array<rel_pos_t,2> relativePos)
{
    this->relativePos = relativePos;
}

array<rel_pos_t,2> UIElement::getRelativeSize() const
{
    return relativeSize;
}

void UIElement::setRelativeSize(array<rel_pos_t,2> relativeSize)
{
    this->relativeSize = relativeSize;
}

array<real_pos_t,2> UIElement::getRealPos() const
{
    return realPos;
}

void UIElement::setRealPos(array<real_pos_t,2> realPos)
{
    this->realPos = realPos;
}

array<real_pos_t,2> UIElement::getRealSize() const
{
    return realSize;
}

void UIElement::setRealSize(array<real_pos_t,2> realSize)
{
    this->realSize = realSize;
}

UIElement* UIElement::getParent() const
{
    return parent;
}

shared_ptr<Texture> UIElement::getTexture() const
{
    return texture;
}

