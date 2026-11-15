#include "CombatMath.h"
#include <algorithm>
#include <cmath>

namespace CombatMath
{
	double armorReduction(double armor, double k)
	{
		if (!(armor > 0.) || !(k > 0.)) {
			return 0.;
		}
		return armor / (armor + k);
	}

	double armorAfterPiercing(double armor, double armorPiercing)
	{
		return std::max(0., armor - std::max(0., armorPiercing));
	}

	bool isPhysical(DamageTypes type)
	{
		return type == DamageTypes::SLASHING || type == DamageTypes::PIERCING || type == DamageTypes::BLUDGEONING;
	}

	double percentMultiplier(double percent)
	{
		return std::max(0., 1. + percent / 100.);
	}

	double durationScale(double speedPercent)
	{
		return 100. / (100. + std::max(-90., speedPercent));
	}
}
