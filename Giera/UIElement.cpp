#include "UIElement.h"
#include "TextureLoader.h"
#include <SDL.h>

UIElement::UIElement(Rect <fr_pos_t> frRelPosRect,
	shared_ptr<Texture> image, UIElement* parent, SDL_Color bgColor, ImageResizeTypes imageResizePolicy,
	VerticalAlignmentTypes vImageAlign, HorizontalAlignmentTypes hImageAlign) :
	image(image), parent(parent),
	pxRealPosRect({ (int)(frRelPosRect.x * parent->pxRealPosRect.w + parent->pxRealPosRect.x),
		(int)(frRelPosRect.y * parent->pxRealPosRect.h + parent->pxRealPosRect.y),
	(int)(frRelPosRect.w * parent->pxRealPosRect.w),
		(int)(frRelPosRect.h * parent->pxRealPosRect.h) }), bgColor(bgColor),
	imageResizePolicy(imageResizePolicy), vImageAlign(vImageAlign), hImageAlign(hImageAlign)
{
	texture = TextureLoader::makeUniColorTexture(pxRealPosRect.w, pxRealPosRect.h, { 0,0,0,0 });
}

UIElement::UIElement(Rect <px_pos_t> pxRealPosRect, shared_ptr<Texture> image, SDL_Color bgColor)
	:parent(nullptr), image(image), pxRealPosRect(pxRealPosRect), bgColor(bgColor)
{
}

void UIElement::drawInside() {
	drawImage();
}

void UIElement::drawImage() {
	if (image == nullptr) {
		return;
	}
	if (imageResizePolicy == ImageResizeTypes::STRETCH) {
		image->draw(*texture, nullopt, nullopt);
		return;
	}
	int imSizeX = image->getSize().first;
	int imSizeY = image->getSize().second;
	if (imageResizePolicy == ImageResizeTypes::KEEP_ASPECT_RATIO) {
		double aspectRatio = ((double)imSizeX) / (double)imSizeY;
		double resizeRatio = std::min((double)pxRealPosRect.w / (double)imSizeX,
			(double)pxRealPosRect.h / (double)imSizeY);
		imSizeX *= resizeRatio;
		imSizeY *= resizeRatio;
	}
	int x = 0;
	int y = 0;

	if (hImageAlign == HorizontalAlignmentTypes::CENTER) {
		x += (pxRealPosRect.w - imSizeX) / 2;
	}
	if (hImageAlign == HorizontalAlignmentTypes::RIGHT) {
		x += pxRealPosRect.w - imSizeX;
	}

	if (vImageAlign == VerticalAlignmentTypes::CENTER) {
		y += (pxRealPosRect.h - imSizeY) / 2;
	}
	if (vImageAlign == VerticalAlignmentTypes::BOTTOM) {
		y += pxRealPosRect.h - imSizeY;
	}
	image->draw(*texture, nullopt, SDL_Rect{ x,y,imSizeX,imSizeY });
}



void UIElement::render(shared_ptr <Texture>& textureToDrawOn)
{
	if (updateNeeded) {
		updateNeeded = false;
		insertBackground();
		drawInside();
		for (auto& child : children) {
			child->render(texture);
		}
	}
	SDL_Rect dstRect = getPixelRelativePosRect().turnToSDL_Rect();
	texture->draw(*textureToDrawOn, nullopt, dstRect);
}

bool UIElement::handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff)
{
	bool ans = false;
	for (auto& child : children) {
		ans = ans || child->handleMouseInput(mouseEventType, pos, timeDiff);
	}
	return ans;
}
UIElement::EventHandleResult UIElement::handleEvent(const SDL_Event& e, Time timeDiff, int screenSizeX, int screenSizeY)
{
    // After handling the event, always refresh fingerPositions
    // with all active fingers across all touch devices:
    fingerPositions.clear();
    int numDevices = SDL_GetNumTouchDevices();
    for (int d = 0; d < numDevices; ++d)
    {
        SDL_TouchID touchId = SDL_GetTouchDevice(d);
        if (touchId == 0) continue;

        int nFingers = SDL_GetNumTouchFingers(touchId);
        for (int i = 0; i < nFingers; ++i)
        {
            SDL_Finger* finger = SDL_GetTouchFinger(touchId, i);
            if (finger)
            {
                float x = finger->x * screenSizeX;
                float y = finger->y * screenSizeY;
                fingerPositions.emplace_back(x, y);
            }
        }
    }

    // Pass the event down to children (if any)
    for (auto& child : children)
    {
        child->handleEvent(e, timeDiff, screenSizeX, screenSizeY);
    }

    EventHandleResult ret{ false, {} };
    return ret;
}



	void UIElement::addChild(unique_ptr<UIElement> child)
	{
		children.push_back(std::move(child));
		needsUpdate();
	}

	void UIElement::removeChild(UIElement * childToRemove)
	{
		for (int i = 0; i < children.size(); i++) {
			if (children[i].get() == childToRemove) {
				children.erase(children.begin() + i);
				break;
			}
		}
		needsUpdate();
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

std::ostream& operator<<(std::ostream& os, UIElement::MouseEventTypes eventType) {
	switch (eventType) {
	case UIElement::MouseEventTypes::HOVER:
		os << "HOVER";
		break;
	case UIElement::MouseEventTypes::PRESS_LEFT:
		os << "PRESS_LEFT";
		break;
	case UIElement::MouseEventTypes::RELEASE_LEFT:
		os << "RELEASE_LEFT";
		break;
	case UIElement::MouseEventTypes::PRESS_RIGHT:
		os << "PRESS_RIGHT";
		break;
	case UIElement::MouseEventTypes::RELEASE_RIGHT:
		os << "RELEASE_RIGHT";
		break;
	case UIElement::MouseEventTypes::MOUSE_SCROLL_UP:
		os << "MOUSE_SCROLL_UP";
		break;
	case UIElement::MouseEventTypes::MOUSE_SCROLL_DOWN:
		os << "MOUSE_SCROLL_DOWN";
		break;
	case UIElement::MouseEventTypes::COUNT:
		os << "COUNT";
		break;
	default:
		os << "UNKNOWN_EVENT";
	}
	return os;
}