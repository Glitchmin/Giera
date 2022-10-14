#include "BaseArmor.h"

BaseArmor::BaseArmor()
{
}

shared_ptr<AbstractItem> BaseArmor::generate()
{
    return make_shared<Armor>(width, height, value.getRandom(), name, description,modifiers,armor.getRandom());
}
