#include "ButtonUI.h"
using std::min;
using std::max;

ButtonUI::ButtonUI(Rect<fr_pos_t> relativePosRect, shared_ptr<Texture> texture, UIElement* parent, double relativeEdgeThickness)
	:UIElement(relativePosRect, texture, parent), relativeEdgeThickness(relativeEdgeThickness), edgeTransparency(0)
{

}

void ButtonUI::drawEdges(shared_ptr<Texture>& texture)
{
	int edgeSize = (relativeEdgeThickness * pxRealPosRect.h +
		relativeEdgeThickness * pxRealPosRect.w + 1) / 2;
	SDL_SetRenderTarget(Texture::getRenderer(), texture->getTexture());
	SDL_SetRenderDrawColor(Texture::getRenderer(), 255, 255, 255, edgeTransparency);
	int x = pxRealPosRect.x - parent->getPixelRealPosRect().x;
	int y = pxRealPosRect.y - parent->getPixelRealPosRect().y;
	int Sx = pxRealPosRect.w;
	int Sy = pxRealPosRect.h;
	for (int i = 0; i < edgeSize;i++) {
		SDL_RenderDrawLine(Texture::getRenderer(), x + i, y, x + i, y + Sy);
		SDL_RenderDrawLine(Texture::getRenderer(), x + Sx - i - 1, y, x + Sx - i - 1, y + Sy);
		SDL_RenderDrawLine(Texture::getRenderer(), x, y + i, x + Sx, y + i);
		SDL_RenderDrawLine(Texture::getRenderer(), x, y + Sy - i - 1, x + Sx, y + Sy - i - 1);
	}
}

void ButtonUI::changeEdgeTransparency(Time timeDiff, bool positive)
{
	if (positive) {
		if (edgeTransparency != maxEdgeTransparency) {
			needsUpdate();
		}
		edgeTransparency += timeDiff.getTimeMs();
		edgeTransparency = min(edgeTransparency, maxEdgeTransparency);
		return;
	}
	if (edgeTransparency != 0) {
		needsUpdate();
	}
	edgeTransparency -= timeDiff.getTimeMs();
	edgeTransparency = max(edgeTransparency, 0);
}

void ButtonUI::handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff)
{
	if (pxRealPosRect.isPointInside(pos.first, pos.second)) {
		edgeTransparency += timeDiff.getTimeMs();
		edgeTransparency = min(edgeTransparency, maxEdgeTransparency);
		if (mouseEventType == MouseEventTypes::PRESS_LEFT) {
			Logger::logInfo("button pressed");
		}
	}
	else {
		edgeTransparency -= timeDiff.getTimeMs();
		edgeTransparency = max(edgeTransparency, 0);
	}
}

void ButtonUI::render(shared_ptr<Texture>& textureToDrawOn)
{
	UIElement::render(textureToDrawOn);
	drawEdges(textureToDrawOn);
}


