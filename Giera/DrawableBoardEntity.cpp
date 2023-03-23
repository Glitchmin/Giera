#include "DrawableBoardEntity.h"

const vector<Drawable>& DrawableBoardEntity::getDrawables() const
{
    return drawables;
}

void DrawableBoardEntity::addObserver(weak_ptr<DrawableEntityObserver> observer)
{
    observers.push_back(observer);
    observer.lock()->notify(this,DrawableEntityObserver::Change::ADDED);
}

void DrawableBoardEntity::notifyObservers(DrawableEntityObserver::Change change)
{
    for (auto& obs_w : observers) {
        if (auto obs = obs_w.lock()) {
            obs->notify(this, change);
        }
    }
}



