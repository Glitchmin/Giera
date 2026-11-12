#include "ColumnLayout.h"

ColumnLayout::ColumnLayout(Rect<fr_pos_t> frRelPosRect, UIElement* parent, SDL_Color bgColor)
    : UIElement(frRelPosRect, nullptr, parent, bgColor) {}

void ColumnLayout::alignChildren() {
    if (children.empty()) return;

    px_pos_t childrenHSum = 0;
    for (auto& child : children) {
        childrenHSum += child->getPixelRealPosRect().h;
    }
    int gap = (pxRealPosRect.h - childrenHSum) / (children.size() + 1);
    int startY = pxRealPosRect.y + gap;
    for (auto& child : children) {
        Rect<px_pos_t> childPos = child->getPixelRealPosRect();
        childPos.y = startY;
        startY += childPos.h + gap;
        child->setPixelRealPosRect(childPos);
    }
}

void ColumnLayout::render(std::shared_ptr<Texture>& textureToDrawOn) {
    SDL_SetRenderDrawBlendMode(Texture::getRenderer(), SDL_BLENDMODE_NONE);
    if (updateNeeded) {
        alignChildren();
    }
    UIElement::render(textureToDrawOn);
    SDL_SetRenderDrawBlendMode(Texture::getRenderer(), SDL_BLENDMODE_BLEND);
}
