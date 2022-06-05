#pragma once
#include <memory>
#include <vector>
#include "AbstractEffect.h"
#include "AbstractBaseItem.h"
#include "DamageTypes.h"
using std::shared_ptr;
using std::vector;

class BaseArrow :
    public AbstractBaseItem
{
protected:
    ValuesRange damage;
    DamageTypes damageType;
    ValuesRange armorPiercing;
    vector<shared_ptr<AbstractEffect>> effects;
public:
    BaseArrow();
    ItemTypes getItemType() override;

    //TODO generateArrow();
    //TODO override input stream operator
};

