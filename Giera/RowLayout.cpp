#include "RowLayout.h"


RowLayout::RowLayout(Rect<fr_pos_t> frRelPosRect, UIElement* parent, SDL_Color bgColor)
	:UIElement(frRelPosRect,nullptr,parent,bgColor)
{}

void RowLayout::alignChildren() {
    if (children.empty()) return;

    px_pos_t childrenWSum = 0;
    for (auto& child : children) {
        childrenWSum += child->getPixelRealPosRect().w;
    }
    int gap = (pxRealPosRect.w - childrenWSum) / (children.size() + 1);
    int startX = pxRealPosRect.x + gap;
    for (auto& child : children) {
        Rect<px_pos_t> childRect = child->getPixelRealPosRect();
        auto childPos = child->getPixelRealPosRect();
        childPos.x = startX;
        startX += childPos.w + gap;
        child->setPixelRealPosRect(childPos);
    }
}

void RowLayout::render(std::shared_ptr<Texture>& textureToDrawOn) {
    if (updateNeeded){
        alignChildren();
    }
    UIElement::render(textureToDrawOn);
}
