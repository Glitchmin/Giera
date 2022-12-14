#include "DrawableBoardEntity.h"

const vector<Drawable>& DrawableBoardEntity::getDrawables() const
{
    return drawables;
}

void DrawableBoardEntity::addDrawable(Drawable& drawable)
{
    this->drawables.push_back(drawable);
}

void DrawableBoardEntity::addDrawable(Position pos, shared_ptr<Sprite> sprite)
{
    this->drawables.push_back(make_pair(pos,sprite));
}



