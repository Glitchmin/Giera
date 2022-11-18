#include "BaseFood.h"
#include "Logger.h"
#include "EffectsHandler.h"
using std::dynamic_pointer_cast;
BaseFood::BaseFood()
{
}

ItemTypes BaseFood::getItemType()
{
    return ItemTypes::FOOD;
}

shared_ptr<AbstractItem> BaseFood::generate()
{
    vector <shared_ptr<AbstractEffect>> effects;
    for (auto& ef : this->effects) 
    {
        effects.push_back(ef->generate());
    }
    return make_shared<Food>(width,height,value.getRandom(),name,description,effects);
}

istream& operator>>(istream& is, BaseFood& f)
{
    int eff_nr;
    int tmp;
    is >> (AbstractBaseItem&) f >>eff_nr >> tmp;
    f.foodType = (FoodTypes)tmp;
    string filler;
    for (int i = 0;i < 2;i++) {
        is >> filler;
    }
    for (int i = 0; i < eff_nr;i++)
    {
        int effectType, effectID;
        is >> effectType >> effectID;

        f.effects.push_back(EffectsHandler::getEffect<AbstractEffect>((EffectTypes)effectType, effectID));
    }
    return is;
}
