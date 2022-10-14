#pragma once
#include <memory>
#include <vector>
#include "AbstractItem.h"
#include "AbstractEffect.h"
#include "Damage.h"

using std::unique_ptr;
using std::shared_ptr;
using std::vector;

class Arrow :
    public AbstractItem
{
protected:
    unique_ptr<Damage> damage;
    vector<shared_ptr<AbstractEffect>> effects;
public:
    Arrow(item_size_t width, item_size_t height, int Value, string& name,
        string& description, unique_ptr<Damage>& damage, vector<shared_ptr<AbstractEffect>> effects);
    ItemTypes getItemType() override;
    string getPath();
    //TODO override input stream operator
};

