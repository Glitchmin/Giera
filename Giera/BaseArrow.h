#pragma once
#include "AbstractEffect.h"
#include "AbstractBaseItem.h"
#include "DamageTypes.h"
using std::shared_ptr;
using std::make_unique;
using std::vector;

class BaseArrow :
    public AbstractBaseItem
{
protected:
    ValuesRange damage;
    DamageTypes damageType;
    ValuesRange armorPiercing;
    vector<shared_ptr<AbstractEffect>> effects;
    vector<ValuesRange> values;
    ArrowTypes arrowType;
public:
    BaseArrow();
    ItemTypes getItemType() override;
    shared_ptr <AbstractItem> generate();
    friend istream& operator>>(istream& is, BaseArrow& it);
};

