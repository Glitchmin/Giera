#pragma once
#include "UIElement.h"

class RowLayout : public UIElement {
protected:
    void alignChildren();
public:
    RowLayout(Rect<fr_pos_t> frRelPosRect, UIElement* parent, 
        SDL_Color bgColor = { 0, 0, 0, 0 });
    void render(std::shared_ptr<Texture>& textureToDrawOn) override;
};


