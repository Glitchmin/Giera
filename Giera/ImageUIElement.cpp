#include "ImageUIElement.h"

ImageUIElement::ImageUIElement(fr_pos_t xRel, fr_pos_t yRel, shared_ptr<Texture> image, UIElement* parent)
	:UIElement(Rect<fr_pos_t>{xRel, yRel, 0,0},image,parent) {
	pxRealPosRect.w = image->getSize().first;
	pxRealPosRect.h = image->getSize().second;
	texture = TextureLoader::makeUniColorTexture(pxRealPosRect.w, pxRealPosRect.h, { 0,0,0,0 });
}

void ImageUIElement::render(shared_ptr<Texture>& textureToDrawOn)
{
	UIElement::render(textureToDrawOn);
}

