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
    ArrowTypes arrowType;
    double breakChance; ///< 0..1, rolled on every impact (Arrow.csv BREAK_CHANCE)
public:
    Arrow(item_size_t width, item_size_t height, int Value, string& name, string& description,
        unique_ptr<Damage>& damage, vector<shared_ptr<AbstractEffect>> effects, ArrowTypes arrowType, double breakChance = 0.);
    ItemTypes getItemType() override;

    const unique_ptr<Damage>& getDamage() const;
    const vector<shared_ptr<AbstractEffect>>& getEffects() const;
    double getBreakChance() const;
    ArrowTypes getArrowType() const;
    virtual std::string getFilePath() override;
    //TODO override input stream operator

};

