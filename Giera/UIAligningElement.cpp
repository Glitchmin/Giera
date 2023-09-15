#include "UIAligningElement.h"

void UIAligningElement::alignChild()
{
	auto child = children[0].get();
	auto finalChildPos = child->getPixelRealPosRect();
	finalChildPos.x = pxRealPosRect.x;
	finalChildPos.y = pxRealPosRect.y;
	if (hAlignment == HorizontalAlignmentTypes::CENTER) {
		finalChildPos.x += pxRealPosRect.w / 2 - finalChildPos.w / 2;
	}
	if (hAlignment == HorizontalAlignmentTypes::RIGHT) {
		finalChildPos.x += pxRealPosRect.w - finalChildPos.w;
	}
	if (vAlignment == VerticalAlignmentTypes::CENTER) {
		finalChildPos.y += pxRealPosRect.h / 2 - finalChildPos.h / 2;
	}
	if (vAlignment == VerticalAlignmentTypes::BOTTOM) {
		finalChildPos.y += pxRealPosRect.h - finalChildPos.h;
	}
	child->setPixelRealPosRect(finalChildPos);
}

UIAligningElement::UIAligningElement(Rect<fr_pos_t> frRelPosRect, UIElement* parent, 
	HorizontalAlignmentTypes hAlignment, VerticalAlignmentTypes vAlignment)
	:UIElement(frRelPosRect,nullptr,parent), vAlignment(vAlignment), hAlignment(hAlignment)
{}

void UIAligningElement::render(shared_ptr<Texture>& textureToDrawOn)
{
	if (updateNeeded) {
		alignChild();
	}
	UIElement::render(textureToDrawOn);

}

