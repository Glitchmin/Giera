#pragma once
#include "Drawable.h"
#include "DrawableEntityObserver.h"
#include <memory>
using std::vector;
using std::weak_ptr;

class Drawable;
class DrawableEntityObserver;

class DrawableBoardEntity
{
protected:
	vector <shared_ptr<Drawable> > drawables;
    vector <weak_ptr<DrawableEntityObserver> > observers;

public:
    void addObserver(weak_ptr<DrawableEntityObserver> observer);
    void notifyObservers(DrawableEntityObserver::Change change);
    vector<shared_ptr<Drawable>>& getDrawables();
    virtual void updateDrawables() = 0;

};

