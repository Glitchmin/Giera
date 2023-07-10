#include "UIElement.h"
#include "TextureLoader.h"

using std::make_shared;

UIElement::UIElement(Rect <rel_pos_t> relativePosRect,
	shared_ptr<Texture> texture, UIElement* parent) :
	texture(texture), parent(parent), relativePosRect(relativePosRect),
	realPosRect({ (int)(relativePosRect.getPos()[0] * parent->realPosRect.getSize()[0] + realPosRect.getPos()[0]),
		(int)(relativePosRect.getPos()[1] * parent->realPosRect.getSize()[1] + realPosRect.getPos()[1]),
	(int)(relativePosRect.getSize()[0] * parent->realPosRect.getSize()[0]),
		(int)(relativePosRect.getSize()[1] * parent->realPosRect.getSize()[1]) })
{
}

UIElement::UIElement(Rect <real_pos_t> realPosRect, shared_ptr<Texture> texture)
	:parent(nullptr), texture(texture), realPosRect(realPosRect), relativePosRect(0, 0, 0, 0)
{
}

void UIElement::render(shared_ptr <Texture>& textureToDrawOn)
{
	for (auto& child : children) {
		child->render(texture);
	}
	SDL_Rect dstRect = realPosRect.turnToSDL_Rect();
	SDL_Rect srcRect{ 0,0, texture->getSize().first, texture->getSize().second };
	texture->draw(*textureToDrawOn, srcRect, dstRect);
}

void UIElement::addChild(unique_ptr<UIElement> child)
{
	children.push_back(std::move(child));
}


UIElement* UIElement::getParent() const
{
	return parent;
}

shared_ptr<Texture> UIElement::getTexture() const
{
	return texture;
}


Rect<rel_pos_t> UIElement::getRelativePosRect() const
{
	return relativePosRect;
}

Rect<real_pos_t> UIElement::getRealPosRect() const
{
	return realPosRect;
}

