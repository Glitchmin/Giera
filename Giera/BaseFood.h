#pragma once
#include <memory>
#include <vector>
#include <string>
#include "AbstractBaseItem.h"
#include "AbstractEffect.h"
using std::shared_ptr;
using std::vector;
using std::istream;
class BaseFood :
    public AbstractBaseItem
{
protected:
    vector<shared_ptr<AbstractEffect>> effects;
    vector <ValuesRange> values; //for dmg effects only
public:
    BaseFood();

    //TODO generateFood();
    friend istream& operator >> (istream& is, BaseFood& f);
};

