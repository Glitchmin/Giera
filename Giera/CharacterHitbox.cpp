#include "pch.h"

#include "CharacterHitbox.h"

CharacterHitbox::CharacterHitbox(unique_ptr<AbstractGeometryFigure>figure, double damageMultiplier):
    Hitbox(std::move(figure),HittableEntityTypes::Character)
{
    this->damageMultiplier = damageMultiplier;
}

double CharacterHitbox::getDamageMultiplier() const
{
    return damageMultiplier;
}

void CharacterHitbox::setDamageMultiplier(double damageMultiplier)
{
    this->damageMultiplier = damageMultiplier;
}

