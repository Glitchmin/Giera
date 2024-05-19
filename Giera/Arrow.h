#pragma once
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
    ArrowTypes arrowType;
public:
    Arrow(item_size_t width, item_size_t height, int Value, string& name, string& description,
        unique_ptr<Damage>& damage, vector<shared_ptr<AbstractEffect>> effects, ArrowTypes arrowType);
    ItemTypes getItemType() override;

    const unique_ptr<Damage>& getDamage() const;
    const vector<shared_ptr<AbstractEffect>>& getEffects() const;
    virtual std::string getFilePath() override;
    //TODO override input stream operator

};

