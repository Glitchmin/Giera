#include "BaseRangedWeapon.h"

BaseRangedWeapon::BaseRangedWeapon()
{
}

ItemTypes BaseRangedWeapon::getItemType()
{
    return ItemTypes::RANGED_WEAPON;
}

shared_ptr<AbstractItem> BaseRangedWeapon::generate()
{
    auto tmp = make_unique<Damage>(damage.getRandom(), damageType, std::shared_ptr<AbstractNPC>(nullptr));
    return make_shared<RangedWeapon>(width,height,value.getRandom(),name,description,modifiers,tmp,
        Time(drawSpeed.getRandom()),arrowSpeed.getAverage());
}
