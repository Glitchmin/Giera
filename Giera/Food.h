#pragma once
#include <memory>
#include <vector>
#include "AbstractItem.h"
#include "AbstractEffect.h"
using std::shared_ptr;
using std::vector;
class Food :
    public AbstractItem
{
protected:
    vector<shared_ptr<AbstractEffect>> effects;
    vector <double> values; //for dmg effects only
public:
    Food(item_size_t width, item_size_t height, int Value, string& name,
        string& description, vector<shared_ptr<AbstractEffect>> effects,
        vector <double> values);

    //TODO override input stream operator
};

