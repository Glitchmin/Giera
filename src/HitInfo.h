#pragma once
#include "DamageTypes.h"
#include <memory>
#include <vector>

class AbstractCharacter;
class AbstractEffect;

/**
 * @brief Everything an attack carries to its target. Built by the attacker
 * (AbstractCharacter::makeMeleeHit / makeArrowHit: damage modifiers and the crit roll
 * are already applied) and resolved by the target (AbstractCharacter::receiveHit:
 * parry, armor, lifesteal, effects, reactions).
 */
struct HitInfo
{
	double damage = 0.;                  ///< after the attacker's modifiers and crit
	DamageTypes type = DamageTypes::SLASHING;
	double armorPiercing = 0.;           ///< flat amount removed from the target's armor
	bool critical = false;
	bool melee = false;                  ///< melee hits can be parried and stagger on a shield
	bool canBeParried = true;            ///< false for the riposte counter-hit itself
	double lifestealPercent = 0.;        ///< % of the damage dealt that heals the attacker
	double lifestealChancePct = 0.;      ///< chance (%) that lifesteal happens on this hit
	std::vector<std::shared_ptr<AbstractEffect>> effects; ///< applied to the target on hit
	std::weak_ptr<AbstractCharacter> attacker;
};

/// What happened to a hit (returned by AbstractCharacter::receiveHit).
struct HitOutcome
{
	bool landed = false;        ///< false: dodged (dash) or parried
	bool parried = false;       ///< deflected by a timed parry (riposte followed)
	bool shieldBlocked = false; ///< melee hit on a fully raised shield (attacker staggered)
	bool killed = false;
	unsigned int dealt = 0;     ///< hp actually lost
	double reduction = 0.;      ///< 0..1 share removed by armor
};
