#pragma once
#include "Hitbox.h"
#include "HittableEntityObserver.h"
#include <vector>
using std::vector;
using std::shared_ptr;
using std::weak_ptr;


class HittableBoardEntity
{
protected:
	vector <shared_ptr <Hitbox> > hitboxes;
	vector<weak_ptr<HittableEntityObserver>> observers;

public:
	HittableBoardEntity() {};

    vector<shared_ptr<Hitbox>>& getHitboxes();
	void addObserver(weak_ptr<HittableEntityObserver> observer);

	virtual void updateHittables()=0;

	void notifyObservers(HittableEntityObserver::Change change);
};

