#include "UIElement.h"
#include "TextureLoader.h"

UIElement::UIElement(Rect <fr_pos_t> frRelPosRect,
	shared_ptr<Texture> image, UIElement* parent, SDL_Color bgColor) :
	image(image), parent(parent),
	pxRealPosRect({ (int)(frRelPosRect.x * parent->pxRealPosRect.w + parent->pxRealPosRect.x),
		(int)(frRelPosRect.y * parent->pxRealPosRect.h + parent->pxRealPosRect.y),
	(int)(frRelPosRect.w * parent->pxRealPosRect.w),
		(int)(frRelPosRect.h * parent->pxRealPosRect.h) }), bgColor(bgColor)
{
	texture = TextureLoader::makeUniColorTexture(pxRealPosRect.w, pxRealPosRect.h, { 0,0,0,0 });
}

UIElement::UIElement(Rect <px_pos_t> pxRealPosRect, shared_ptr<Texture> image, SDL_Color bgColor)
	:parent(nullptr), image(image), pxRealPosRect(pxRealPosRect), bgColor(bgColor)
{
}

void UIElement::render(shared_ptr <Texture>& textureToDrawOn)
{
	if (updateNeeded) {
		updateNeeded = false;
		insertBackground();
		if (image) {
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

void UIElement::insertBackground()
{
	texture->fillWithColor(bgColor);
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
	updateNeeded = true;
	parent->needsUpdate();
}

shared_ptr<Texture> UIElement::getTexture() const
{
	return texture;
}

Rect<px_pos_t> UIElement::getPixelRelativePosRect() const
{
	return { pxRealPosRect.x - parent->pxRealPosRect.x,
	pxRealPosRect.y - parent->pxRealPosRect.y,
	pxRealPosRect.w,pxRealPosRect.h };
}

Rect<px_pos_t> UIElement::getPixelRealPosRect() const
{
	return pxRealPosRect;
}

void UIElement::setPixelRealPosRect(Rect<px_pos_t> pixelRealPosRect)
{
	px_pos_t xShift = pixelRealPosRect.x - pxRealPosRect.x;
	px_pos_t yShift = pixelRealPosRect.y - pxRealPosRect.y;
	for (auto& child : children) {
		auto posRect = child->getPixelRealPosRect();
		posRect.x += xShift;
		posRect.y += yShift;
		child->setPixelRealPosRect(posRect);
	}
	pxRealPosRect = pixelRealPosRect;
}




void UIElement::setBgColor(SDL_Color bgColor)
{
	this->bgColor = bgColor;
}

