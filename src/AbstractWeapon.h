#pragma once
#include "AbstractGearItem.h"
#include "Damage.h"

using std::unique_ptr;

class AbstractWeapon :
    public AbstractGearItem
{
protected:
    unique_ptr<Damage> damage;

public:
    AbstractWeapon(item_size_t width, item_size_t height, int value,
        string& name, string& description, vector<item_mod_t> modifiers,
        unique_ptr<Damage>& damage);

    const unique_ptr<Damage>& getDamage() const;
};

