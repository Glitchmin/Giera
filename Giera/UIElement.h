#pragma once
#include "Texture.h"
#include "TextureLoader.h"
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
	array<rel_pos_t, 2> relativePos; //relative to the parent
	array<rel_pos_t, 2> relativeSize; //relative to the parent
	array<real_pos_t, 2> realPos;
	array<real_pos_t, 2> realSize;
	UIElement* parent;
	shared_ptr <Texture> texture;
	vector<unique_ptr<UIElement>> children;

public:
	UIElement(rel_pos_t relX, rel_pos_t relY, rel_pos_t relSizeX, rel_pos_t relSizeY,
		shared_ptr<Texture> texture, UIElement* parent);
	UIElement(real_pos_t realX, real_pos_t realY, real_pos_t realSizeX, real_pos_t realSizeY);
	void render(shared_ptr <Texture>& textureToDrawOn);
	void addChild(unique_ptr<UIElement> child);

};

