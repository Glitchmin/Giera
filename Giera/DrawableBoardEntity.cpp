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
    this->drawables.push_back(Drawable(pos,sprite));
}

void DrawableBoardEntity::addObserver(weak_ptr<DrawableEntityObserver> observer)
{
    observers.push_back(observer);
}

void DrawableBoardEntity::notifyObservers(DrawableEntityObserver::Change change)
{
    for (auto& obs_w : observers) {
        if (auto obs = obs_w.lock()) {
            obs->notify(this, change);
        }
    }
}



