#include "pch.h"

#include "SpawningDetails.h"
#include "BaseItems.h"

SpawningDetails::SpawningDetails(shared_ptr<AbstractBaseItem> item, double chance, unsigned int level)
{
	this->item = item;
	this->chance = chance;
	this->level = level;
}

shared_ptr<AbstractBaseItem> SpawningDetails::getItem() const
{
    return item;
}
double SpawningDetails::getChance() const
{
    return chance;
}
unsigned int SpawningDetails::getLevel() const
{
    return level;
}

