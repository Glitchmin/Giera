#pragma once
#include "Texture.h"
#include "TextureLoader.h"
#include "Rect.h"
#include <array>
#include <vector>
using std::array;
using std::vector;
using std::unique_ptr;

typedef float rel_pos_t;
typedef int real_pos_t;
class UIElement
{
protected:
	Rect <rel_pos_t> relativePosRect; //relative to the parent
	Rect <real_pos_t> realPosRect;
	UIElement* parent;
	shared_ptr <Texture> texture;
	vector<unique_ptr<UIElement>> children;

public:
	UIElement(Rect <rel_pos_t> relativePosRect, shared_ptr<Texture> texture, UIElement* parent);
	UIElement(Rect <real_pos_t> realPosRect, shared_ptr<Texture> texture);
	virtual void render(shared_ptr <Texture>& textureToDrawOn);
	virtual void addChild(unique_ptr<UIElement> child);

    UIElement* getParent() const;

    shared_ptr<Texture> getTexture() const;

	Rect<rel_pos_t> getRelativePosRect() const;

	Rect<real_pos_t> getRealPosRect() const;

};

