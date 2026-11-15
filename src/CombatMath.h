#pragma once
#include "DamageTypes.h"

/// The fight system's formulas, free of game objects so they are easy to test and tune.
namespace CombatMath
{
	/// Share of damage removed by armor: armor / (armor + k). 0 for armor <= 0.
	double armorReduction(double armor, double k);

	/// Armor left after flat armor piercing (never below 0).
	double armorAfterPiercing(double armor, double armorPiercing);

	/// Slashing, piercing and bludgeoning. Other types (fire, frost...) ignore armor.
	bool isPhysical(DamageTypes type);

	/// Gear modifier in % as a multiplier: +20 -> 1.2, -15 -> 0.85 (never below 0).
	double percentMultiplier(double percent);

	/// How a speed modifier in % scales a duration: +25 -> x0.8, -15 -> x1.18.
	/// (Clamped at -90 so a duration can't explode.)
	double durationScale(double speedPercent);
}
