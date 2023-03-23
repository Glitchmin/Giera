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
	vector <Drawable> drawables;
    vector <weak_ptr<DrawableEntityObserver> > observers;

public:
    const vector<Drawable>& getDrawables() const;
    void addObserver(weak_ptr<DrawableEntityObserver> observer);
    void notifyObservers(DrawableEntityObserver::Change change);
    virtual void updateDrawables() = 0;
};

