#include "BaseArrow.h"

BaseArrow::BaseArrow()
{
}

ItemTypes BaseArrow::getItemType()
{
    return ItemTypes::ARROW;
}

shared_ptr<AbstractItem> BaseArrow::generate()
{
    auto dmg_ptr = make_unique<Damage>(damage.getRandom(), damageType, std::shared_ptr<AbstractNPC>(nullptr));
    return make_shared<Arrow>(width,height,value.getRandom(),name,description,dmg_ptr,effects);
}
