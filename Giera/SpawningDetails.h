#pragma once
#include "BaseItems.h"
class SpawningDetails
{
protected:
	shared_ptr<AbstractBaseItem> item;
	double chance;
	unsigned int level;
public:
	SpawningDetails(shared_ptr<AbstractBaseItem> item, double chance, unsigned int level);

    shared_ptr<AbstractBaseItem> getItem() const;
    double getChance() const;
    unsigned int getLevel() const;

};

