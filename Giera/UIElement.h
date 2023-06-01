#pragma once
#include "Texture.h"
#include "TextureLoader.h"
#include <array>
using std::array;

typedef float rel_pos_t;
typedef int real_pos_t;
class UIElement
{
protected:
	array<rel_pos_t,2> relativePos; //relative to the parent
	array<rel_pos_t, 2> relativeSize; //relative to the parent
	array<real_pos_t, 2> realPos;
	array<real_pos_t, 2> realSize;
	shared_ptr <UIElement> parent;
	shared_ptr <Texture> texture;

public:
	UIElement(rel_pos_t relX, rel_pos_t relY, rel_pos_t relSizeX, rel_pos_t relSizeY,
		shared_ptr<Texture> texture, shared_ptr <UIElement> parent);
	void render(shared_ptr <Texture>& textureToDrawOn);

};

