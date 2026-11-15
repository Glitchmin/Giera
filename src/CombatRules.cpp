#include "CombatRules.h"
#include "Calculator.h"
#include "FileHandler.h"
#include "Logger.h"
#include <functional>
#include <map>
#include <sstream>

namespace {
CombatRules currentRules;
CombatRules loadedRules;

template <class T>
std::function<bool(std::istream&)> field(T& target)
{
	return [&target](std::istream& is) { T value{}; if (is >> value) { target = value; return true; } return false; };
}
}

CombatRules& CombatRules::get()
{
	return currentRules;
}

void CombatRules::parse(std::istream& is, CombatRules& r)
{
	const std::map<std::string, std::function<bool(std::istream&)>> fields{
		{ "playerMaxHp", field(r.playerMaxHp) },
		{ "enemyMaxHp", field(r.enemyMaxHp) },
		{ "playerWalkSpeed", field(r.playerWalkSpeed) },
		{ "enemyWalkSpeed", field(r.enemyWalkSpeed) },
		{ "enemyAttackRange", field(r.enemyAttackRange) },
		{ "enemyStopDistance", field(r.enemyStopDistance) },
		{ "enemyMeleeWeaponId", field(r.enemyMeleeWeaponId) },
		{ "enemyArmor", field(r.enemyArmor) },
		{ "armorK", field(r.armorK) },
		{ "baseCritChancePct", field(r.baseCritChancePct) },
		{ "defaultCritDamagePct", field(r.defaultCritDamagePct) },
		{ "bareHandsDamage", field(r.bareHandsDamage) },
		{ "bareHandsSwingMs", field(r.bareHandsSwingMs) },
		{ "meleeReach", field(r.meleeReach) },
		{ "meleeWindupShare", field(r.meleeWindupShare) },
		{ "minDrawToShoot", field(r.minDrawToShoot) },
		{ "arrowSpawnHeight", field(r.arrowSpawnHeight) },
		{ "shieldBlockStaggerMs", field(r.shieldBlockStaggerMs) },
		{ "parryWindowMs", field(r.parryWindowMs) },
		{ "parryMissCooldownMs", field(r.parryMissCooldownMs) },
		{ "riposteStaggerMs", field(r.riposteStaggerMs) },
		{ "moveSpeedWhileBusy", field(r.moveSpeedWhileBusy) },
		{ "actionSpeedWhileMoving", field(r.actionSpeedWhileMoving) },
		{ "dashDistance", field(r.dashDistance) },
		{ "dashDurationMs", field(r.dashDurationMs) },
		{ "dashCooldownMs", field(r.dashCooldownMs) },
		{ "arrowPickupRadius", field(r.arrowPickupRadius) },
		{ "quiverCapacity", field(r.quiverCapacity) },
		{ "startingArrows", field(r.startingArrows) },
		{ "eatTimeMs", field(r.eatTimeMs) },
		{ "startingFood", field(r.startingFood) },
	};
	std::string line;
	while (std::getline(is, line)) {
		auto comment = line.find('#');
		if (comment != std::string::npos) {
			line.erase(comment);
		}
		std::istringstream ls(line);
		std::string key;
		if (!(ls >> key)) {
			continue; // empty / comment line
		}
		auto it = fields.find(key);
		if (it == fields.end()) {
			Logger::logWarning("Combat rules: unknown key", key);
			continue;
		}
		if (!it->second(ls)) {
			Logger::logWarning("Combat rules: bad value for", key);
		}
	}
}

namespace {
struct RulesReader {
	CombatRules* rules;
	friend std::istream& operator>>(std::istream& is, RulesReader& reader)
	{
		CombatRules::parse(is, *reader.rules);
		return is;
	}
};
}

bool CombatRules::load()
{
	FileHandler file("rules/Combat", FileModeTypes::READ, ".txt");
	if (!file.isOpen()) {
		Logger::logWarning("assets/rules/Combat.txt not found, using default combat rules");
		loadedRules = currentRules;
		return false;
	}
	CombatRules rules;
	RulesReader reader{ &rules };
	file.readFromFile(reader);
	file.closeFile();
	currentRules = rules;
	loadedRules = rules;
	Logger::logInfo("combat rules loaded");
	return true;
}

void CombatRules::resetToLoaded()
{
	currentRules = loadedRules;
}

bool CombatRules::roll(double chance)
{
	if (currentRules.forceRolls.has_value()) {
		return currentRules.forceRolls.value();
	}
	return Calculator::calculateChance(chance);
}
