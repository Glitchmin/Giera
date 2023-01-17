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
    void addDrawable(Drawable& drawable);
    void addDrawable(Position pos, shared_ptr<Sprite> sprite);
    void addObserver(weak_ptr<DrawableEntityObserver> observer);
    void notifyObservers(DrawableEntityObserver::Change change);
};

