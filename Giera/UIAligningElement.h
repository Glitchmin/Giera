#pragma once
#include "UIElement.h"
#include "HorizontalAlignmentTypes.h"
#include "VerticalAlignmentTypes.h"
class UIAligningElement :
	public UIElement {

protected:
	HorizontalAlignmentTypes hAlignment;
	VerticalAlignmentTypes vAlignment;
	void alignChild();

public:

	UIAligningElement(Rect<fr_pos_t> frRelPosRect, UIElement* parent,
		HorizontalAlignmentTypes hAlignment, VerticalAlignmentTypes vAlignment);

	virtual void render(shared_ptr<Texture>& textureToDrawOn) override;
};


