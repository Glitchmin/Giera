#include "BaseFood.h"

BaseFood::BaseFood()
{
}

ItemTypes BaseFood::getItemType()
{
    return ItemTypes::FOOD;
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
