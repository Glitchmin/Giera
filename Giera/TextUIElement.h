#pragma once
#include "UIElement.h"
class TextUIElement :
	public UIElement
{
public:
	TextUIElement(Rect<fr_pos_t> frRelPosRect, shared_ptr<Texture> textTexture,
		VerticalAlignmentTypes vAlignment, HorizontalAlignmentTypes hAlignment, UIElement* parent);
	virtual void render(shared_ptr <Texture>& textureToDrawOn) override;
protected:
	VerticalAlignmentTypes vAlignment;
	HorizontalAlignmentTypes hAlignment;
	void align(SDL_Rect& sdlRect);
};

