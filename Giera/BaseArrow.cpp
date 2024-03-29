#include "BaseArrow.h"
#include "DamageEffect.h"
#include "StatChangingEffect.h"
#include "EffectsHandler.h"
#include "Arrow.h"

BaseArrow::BaseArrow()
{
}

ItemTypes BaseArrow::getItemType()
{
    return ItemTypes::ARROW;
}

shared_ptr<AbstractItem> BaseArrow::generate()
{
    vector <shared_ptr<AbstractEffect>> effects;
    for (auto& ef : this->effects)
    {
        effects.push_back(ef->generate());
    }
    auto dmg_ptr = make_unique<Damage>(damage.getRandom(),armorPiercing.getRandom(), damageType);
    return make_shared<Arrow>(width,height,value.getRandom(),name,description,dmg_ptr,effects, arrowType);
}

istream& operator>>(istream& is, BaseArrow& it)
{

    int tmp, tmp2, effs_nr;
    is >> (AbstractBaseItem&)it >> it.damage >> tmp2 >> it.armorPiercing >> effs_nr>>tmp;
    it.damageType = (DamageTypes)tmp2;
    it.arrowType = (ArrowTypes)tmp;
    string filler;
    for (int i = 0;i < 2* effs_nr;i++) {
        is >> filler;
    }
    for (int i = 0; i < effs_nr;i++)
    {
        int effectType, effectID;
        is >> effectType >> effectID;

        it.effects.push_back(EffectsHandler::getEffect<AbstractEffect>((EffectTypes)effectType,effectID));
    }
    return is;
}
