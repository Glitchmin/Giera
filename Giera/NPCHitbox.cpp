#include "NPCHitbox.h"

NPCHitbox::NPCHitbox(unique_ptr<AbstractGeometryFigure>figure, double damageMultiplier):
    Hitbox(std::move(figure),HittableEntityTypes::NPC)
{
    this->damageMultiplier = damageMultiplier;
}

double NPCHitbox::getDamageMultiplier() const
{
    return damageMultiplier;
}

void NPCHitbox::setDamageMultiplier(double damageMultiplier)
{
    this->damageMultiplier = damageMultiplier;
}

