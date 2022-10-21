#include "BaseFood.h"
#include "Logger.h"
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
    //TODO set effects values for DamageEffects
    for (ValuesRange& a : values) {
        //effects.;
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
    for (int i = 0;i < 10;i++) {
        is >> filler;
    }
    for (int i = 0; i < eff_nr;i++)
    {
        bool isDmgEffect;
        is >> isDmgEffect;
        ValuesRange valR;
        shared_ptr<AbstractEffect> eff;
        if (isDmgEffect)
        {
            auto eff2 = make_shared<DamageEffect>();
            is >> *eff2;
            is >> valR;
            eff = eff2;
            f.values.push_back(valR);
        }
        else {
            auto eff2 = make_shared<StatChangingEffect>();
            is >> *eff2;
            eff = eff2;
        }
        f.effects.push_back(eff);
    }
    return is;
}
