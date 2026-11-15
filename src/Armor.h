#pragma once
#include "AbstractGearItem.h"
#include "DamageTypes.h"
#include <array>
class Armor :
    public AbstractGearItem
{
protected:
    double armor;
    ArmorTypes armorType;
    std::array<double, 3> effectiveness; ///< share of the armor that counts vs slashing, piercing, bludgeoning
public:
    Armor(item_size_t width, item_size_t height, int value, string& name,
        string& description, vector<item_mod_t> modifiers, double armor, ArmorTypes armorType,
        std::array<double, 3> effectiveness = { 1., 1., 1. });
    //TODO override input stream operator
    ItemTypes getItemType() override;
    double getArmor() const;
    void setArmor(double armor);
    /// 0..1 share of the armor that counts against @p type (0 for non-physical types).
    double getEffectiveness(DamageTypes type) const;
    virtual std::string getFilePath() override;

};

