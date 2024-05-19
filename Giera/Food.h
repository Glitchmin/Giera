#pragma once
#include "AbstractItem.h"
#include "AbstractEffect.h"
using std::shared_ptr;
using std::vector;
class Food :
    public AbstractItem
{
protected:
    vector<shared_ptr<AbstractEffect>> effects;
    FoodTypes foodType;
public:
    Food(item_size_t width, item_size_t height, int value, string& name,
        string& description, vector<shared_ptr<AbstractEffect>> effects, FoodTypes foodType);
    ItemTypes getItemType() override;
    const vector<shared_ptr<AbstractEffect>>& getEffects();
    virtual std::string getFilePath() override;
    //TODO override input stream operator
};

