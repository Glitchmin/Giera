#include "ButtonUI.h"
using std::min;
using std::max;

ButtonUI::ButtonUI(Rect<fr_pos_t> relativePosRect, shared_ptr<Texture> image, UIElement* parent, double relativeEdgeThickness,
	ImageResizeTypes imageResizePolicy, VerticalAlignmentTypes vImageAlign,	HorizontalAlignmentTypes hImageAlign)
	:UIElement(relativePosRect, image, parent,SDL_Color{0,0,0,0},imageResizePolicy,vImageAlign,hImageAlign), relativeEdgeThickness(relativeEdgeThickness), edgeTransparency(0)
{

}

void ButtonUI::drawEdges(shared_ptr<Texture>& texture)
{
	int edgeSize = (relativeEdgeThickness * pxRealPosRect.h +
		relativeEdgeThickness * pxRealPosRect.w + 1) / 2;
	SDL_SetRenderTarget(Texture::getRenderer(), texture->getSDLTexture());
	SDL_SetRenderDrawColor(Texture::getRenderer(), 255, 255, 255, edgeTransparency);
	int x = pxRealPosRect.x - parent->getPixelRealPosRect().x;
	int y = pxRealPosRect.y - parent->getPixelRealPosRect().y;
	int Sx = pxRealPosRect.w;
	int Sy = pxRealPosRect.h;

	auto edgePixel =
		TextureLoader::makeUniColorTexture(1, 1, { 255,255,255,(Uint8)edgeTransparency });
	SDL_SetTextureBlendMode(edgePixel->getSDLTexture(), SDL_BLENDMODE_NONE);
	edgePixel->draw(*texture, nullopt, SDL_Rect{ x,y + edgeSize,edgeSize,Sy - 2 * edgeSize });
	edgePixel->draw(*texture, nullopt, SDL_Rect{ x+Sx-edgeSize,y+edgeSize,edgeSize,Sy - 2 * edgeSize });
	edgePixel->draw(*texture, nullopt, SDL_Rect{ x,y,Sx,edgeSize });
	edgePixel->draw(*texture, nullopt, SDL_Rect{ x,y+Sy-edgeSize,Sx,edgeSize });
	
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

bool ButtonUI::handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff)
{
	if (pxRealPosRect.isPointInside(pos.first, pos.second)) {
		edgeTransparency += timeDiff.getTimeMs();
		edgeTransparency = min(edgeTransparency, maxEdgeTransparency);
		return true;
	}
	else {
		edgeTransparency -= timeDiff.getTimeMs();
		edgeTransparency = max(edgeTransparency, 0);
		return false;
	}
}

void ButtonUI::render(shared_ptr<Texture>& textureToDrawOn)
{
	drawEdges(textureToDrawOn);
	UIElement::render(textureToDrawOn);
}


