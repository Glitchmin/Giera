#pragma once
#include "UIElement.h"

//Image holder that doesnt resize the image

class ImageUIElement :
    public UIElement
{
public:
    ImageUIElement(fr_pos_t xRel, fr_pos_t yRel, shared_ptr<Texture> image, UIElement* parent);
    virtual void render(shared_ptr <Texture>& textureToDrawOn) override;
};

