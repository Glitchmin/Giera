#pragma once
#include "AbstractBaseItem.h"
#include "DamageEffect.h"
#include "StatChangingEffect.h"
using std::shared_ptr;
using std::vector;
using std::istream;
class BaseFood :
    public AbstractBaseItem
{
protected:
    vector<shared_ptr<AbstractEffect>> effects;
    FoodTypes foodType;
public:
    BaseFood();
    ItemTypes getItemType();
    shared_ptr<AbstractItem> generate();
    friend istream& operator >> (istream& is, BaseFood& f);
};

