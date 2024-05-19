#include "pch.h"

#include "HittableBoardEntity.h"

vector<shared_ptr<Hitbox>>& HittableBoardEntity::getHitboxes()
{
    return hitboxes;
}


void HittableBoardEntity::addHittableObserver(weak_ptr<HittableEntityObserver> observer)
{
    this->observers.push_back(observer);
}

void HittableBoardEntity::notifyHittableObservers(HittableEntityObserver::Change change)
{
    for (auto& obs : observers) {
        auto obs_sp = obs.lock();
        if (obs_sp != nullptr) {
            obs_sp -> notify(this, change);
        }
    }
}

