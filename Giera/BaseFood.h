#pragma once
#include <memory>
#include <vector>
#include "AbstractBaseItem.h"
#include "AbstractEffect.h"
using std::shared_ptr;
using std::vector;
class BaseFood :
    public AbstractBaseItem
{
protected:
    vector<shared_ptr<AbstractEffect>> effects;
    vector <ValuesRange> values; //for dmg effects only
public:
    BaseFood();

    //TODO generateFood();
    //TODO override input stream operator
};

