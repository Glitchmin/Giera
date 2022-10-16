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
public:
    Food(item_size_t width, item_size_t height, int value, string& name,
        string& description, vector<shared_ptr<AbstractEffect>> effects);
    ItemTypes getItemType() override;
    //TODO override input stream operator
};

