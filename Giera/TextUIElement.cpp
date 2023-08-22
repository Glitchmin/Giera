#include "TextUIElement.h"

TextUIElement::TextUIElement(Rect<fr_pos_t> frRelPosRect, shared_ptr<Texture> textTexture,
	VerticalAlignmentTypes vAlignment, HorizontalAlignmentTypes hAlignment, UIElement* parent)
	:UIElement(frRelPosRect, textTexture, parent), vAlignment(vAlignment), hAlignment(hAlignment)
{

}

void TextUIElement::render(shared_ptr<Texture>& textureToDrawOn)
{
	if (updateNeeded) {
		updateNeeded = 0;
		texture->fillWithColor({ 0,0,0,0 });
		SDL_Rect dstRect{ 0,0,image->getSize().first, image->getSize().second };
		
		align(dstRect);

		Logger::logInfo(dstRect.x, dstRect.y);

		if (image != nullptr) {
			image->draw(*texture, nullopt, dstRect);
		}
		for (auto& child : children) {
			child->render(texture);
		}
	}
	SDL_Rect dstRect = getPixelRelativePosRect().turnToSDL_Rect();
	texture->draw(*textureToDrawOn, nullopt, dstRect);

}

void TextUIElement::align(SDL_Rect& sdlRect)
{
	int imSizeX = image->getSize().first;
	int imSizeY = image->getSize().second;
	if (hAlignment == HorizontalAlignmentTypes::CENTER) {
		sdlRect.x +=pxRealPosRect.getSize()[0] / 2 - imSizeX / 2;
	}
	if (hAlignment == HorizontalAlignmentTypes::RIGHT) {
		sdlRect.x += pxRealPosRect.getSize()[0] - imSizeX;
	}
	if (vAlignment == VerticalAlignmentTypes::CENTER) {
		sdlRect.y += pxRealPosRect.getSize()[1] / 2 - imSizeY / 2;
	}
	if (vAlignment == VerticalAlignmentTypes::BOTTOM) {
		sdlRect.y += pxRealPosRect.getSize()[1] - imSizeY;
	}
}
