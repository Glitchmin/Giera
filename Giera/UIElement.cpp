#include "UIElement.h"
#include "TextureLoader.h"

using std::make_shared;

UIElement::UIElement(Rect <fr_pos_t> frRelPosRect,
	shared_ptr<Texture> image, UIElement* parent) :
	image(image), parent(parent), frRelPosRect(frRelPosRect),
	pxRealPosRect({ (int)(frRelPosRect.getPos()[0] * parent->pxRealPosRect.getSize()[0] + parent->pxRealPosRect.getPos()[0]),
		(int)(frRelPosRect.getPos()[1] * parent->pxRealPosRect.getSize()[1] + parent->pxRealPosRect.getPos()[1]),
	(int)(frRelPosRect.getSize()[0] * parent->pxRealPosRect.getSize()[0]),
		(int)(frRelPosRect.getSize()[1] * parent->pxRealPosRect.getSize()[1]) })
{
	Logger::logInfo(frRelPosRect.getSize()[0] , parent->pxRealPosRect.getSize()[0],frRelPosRect.getSize()[0] * parent->pxRealPosRect.getSize()[0]);
	texture = TextureLoader::makeUniColorTexture(pxRealPosRect.getSize()[0], pxRealPosRect.getSize()[1], { 0,0,0,0 });
}

UIElement::UIElement(Rect <px_pos_t> pxRealPosRect, shared_ptr<Texture> image)
	:parent(nullptr), image(image), pxRealPosRect(pxRealPosRect), frRelPosRect(0, 0, 0, 0)
{
}

void UIElement::render(shared_ptr <Texture>& textureToDrawOn)
{
	if (updateNeeded) {
		updateNeeded = 0;
		texture->fillWithColor({ 0,0,0,0 });
		if (image != nullptr) {
			image->draw(*texture, nullopt, nullopt);
		}
		for (auto& child : children) {
			child->render(texture);
		}
	}
	SDL_Rect dstRect = getPixelRelativePosRect().turnToSDL_Rect();
	texture->draw(*textureToDrawOn, nullopt, dstRect);
}

void UIElement::handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff)
{
	for (auto& child : children) {
		child->handleMouseInput(mouseEventType, pos, timeDiff);
	}
}

void UIElement::addChild(unique_ptr<UIElement> child)
{
	children.push_back(std::move(child));
}

const vector<unique_ptr<UIElement>>& UIElement::getChildren()
{
	return children;
}

void UIElement::clearChildren()
{
	children.clear();
}


UIElement* UIElement::getParent() const
{
	return parent;
}

void UIElement::needsUpdate()
{
	if (parent == nullptr) {
		return;
	}
	updateNeeded = 1;
	parent->needsUpdate();
}

shared_ptr<Texture> UIElement::getTexture() const
{
	return texture;
}

Rect<fr_pos_t> UIElement::getFractionalRelativePosRect() const
{
	return frRelPosRect;
}

Rect<px_pos_t> UIElement::getPixelRelativePosRect() const
{
	return {pxRealPosRect.getPos()[0]-parent->pxRealPosRect.getPos()[0],
	pxRealPosRect.getPos()[1] - parent->pxRealPosRect.getPos()[1],
	pxRealPosRect.getSize()[0],pxRealPosRect.getSize()[1]};
}

Rect<px_pos_t> UIElement::getPixelRealPosRect() const
{
	return pxRealPosRect;
}



