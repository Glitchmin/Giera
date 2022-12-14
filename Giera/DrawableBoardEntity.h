#pragma once
#include "Sprite.h"
using std::vector;
class DrawableBoardEntity
{
protected:
	vector <Drawable> drawables;

public:
    const vector<Drawable>& getDrawables() const;
    void addDrawable(Drawable& drawable);
    void addDrawable(Position pos, shared_ptr<Sprite> sprite);

};

