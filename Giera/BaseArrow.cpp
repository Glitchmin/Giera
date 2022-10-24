#include "BaseArrow.h"
#include "DamageEffect.h"
#include "StatChangingEffect.h"

BaseArrow::BaseArrow()
{
}

ItemTypes BaseArrow::getItemType()
{
    return ItemTypes::ARROW;
}

shared_ptr<AbstractItem> BaseArrow::generate()
{
    auto dmg_ptr = make_unique<Damage>(damage.getRandom(),armorPiercing.getRandom(), damageType);
    return make_shared<Arrow>(width,height,value.getRandom(),name,description,dmg_ptr,effects);
}

istream& operator>>(istream& is, BaseArrow& it)
{

    int tmp, tmp2, eff_nr;
    is >> (AbstractBaseItem&)it >> it.damage >> tmp2 >> it.armorPiercing >> eff_nr>>tmp;
    it.damageType = (DamageTypes)tmp2;
    it.arrowType = (ArrowTypes)tmp;
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
            it.values.push_back(valR);
        }
        else {
            auto eff2 = make_shared<StatChangingEffect>();
            is >> *eff2;
            eff = eff2;
        }
        it.effects.push_back(eff);
    }
    return is;
}
