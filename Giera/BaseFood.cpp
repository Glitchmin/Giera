#include "BaseFood.h"

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
    int ef_num;
    is >> ef_num;
    f.effects.resize(ef_num);
    for (int i = 0; i < ef_num; i++) 
    {
        f.effects[i] = make_shared<AbstractEffect>();
        is >> *f.effects[i];
        string tmp;
        if (tmp[0] != '-')
        {
            double tmp2;
            is >> tmp2;
            f.values.push_back(ValuesRange(stod(tmp), tmp2));
        }
    }
    return is;
}
