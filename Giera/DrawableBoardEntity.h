#pragma once
#include "Drawable.h"
#include "DrawableEntityObserver.h"
using std::vector;

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

