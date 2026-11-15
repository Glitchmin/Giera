#pragma once
#include <istream>
#include <optional>
#include <string>

/**
 * @brief Global fight-system numbers, loaded from assets/rules/Combat.txt.
 *
 * Item-specific numbers (weapon damage, swing / draw time, armor, crit modifiers, arrow
 * break chance...) live in the item CSVs in assets/baseItems. Everything that is a rule of
 * the fight system itself lives here. See docs/FightSystem.md for what each value does.
 *
 * Missing keys / a missing file keep the defaults below.
 */
struct CombatRules
{
	// --- characters ---
	unsigned int playerMaxHp = 100;
	unsigned int enemyMaxHp = 100;
	double playerWalkSpeed = 4.0;          ///< m/s
	double enemyWalkSpeed = 1.0;           ///< m/s
	double enemyAttackRange = 1.0;         ///< m, enemy starts a swing when closer than this
	double enemyStopDistance = 0.8;        ///< m, enemy stops walking when closer than this
	int enemyMeleeWeaponId = 1;            ///< row in Melee.csv the enemies are armed with
	double enemyArmor = 15.0;              ///< enemies' armor (full effect vs physical damage)

	// --- damage ---
	double armorK = 50.0;                  ///< reduction = armor / (armor + armorK)
	double baseCritChancePct = 5.0;        ///< added to the gear's CRIT_CHANCE modifiers
	double defaultCritDamagePct = 150.0;   ///< crit multiplier when the weapon's CRIT_DAMAGE is 0

	// --- melee ---
	double bareHandsDamage = 5.0;          ///< damage without a melee weapon equipped
	unsigned int bareHandsSwingMs = 700;   ///< swing time without a melee weapon
	double meleeReach = 1.0;               ///< m
	double meleeWindupShare = 0.3;         ///< part of the swing time before the hit lands

	// --- bow ---
	double minDrawToShoot = 0.25;          ///< releasing earlier puts the arrow back
	double arrowSpawnHeight = 1.0;         ///< m above the ground

	// --- blocking ---
	unsigned int shieldBlockStaggerMs = 600; ///< melee attacker hitting a fully raised shield
	unsigned int parryWindowMs = 250;        ///< no shield: block pressed at most this long before the hit
	unsigned int parryMissCooldownMs = 600;  ///< no shield: after a missed parry
	unsigned int riposteStaggerMs = 1000;    ///< attacker whose hit was parried

	// --- movement ---
	double moveSpeedWhileBusy = 0.5;       ///< walking speed while drawing / shield up / eating
	double actionSpeedWhileMoving = 0.5;   ///< draw / raise speed while walking
	double dashDistance = 2.5;             ///< m
	unsigned int dashDurationMs = 200;     ///< invulnerable for this long
	unsigned int dashCooldownMs = 1000;

	// --- arrows ---
	double arrowPickupRadius = 0.6;        ///< m, walking this close picks an arrow up
	unsigned int quiverCapacity = 20;      ///< arrows per quiver stack
	unsigned int startingArrows = 20;

	// --- healing ---
	unsigned int eatTimeMs = 500;          ///< eating food from the satchel
	unsigned int startingFood = 2;         ///< berries in the satchel at start

	/// Not read from the file: tests set it to make every chance roll (crit, lifesteal)
	/// succeed (true) or fail (false).
	std::optional<bool> forceRolls;

	/// The rules in use (defaults until load() is called).
	static CombatRules& get();
	/// Reads assets/rules/Combat.txt (key value per line, # comments). @return false if not found.
	static bool load();
	/// Restores the rules as last loaded (tests change them).
	static void resetToLoaded();
	/// Parses "key value" lines; unknown keys are logged and ignored.
	static void parse(std::istream& is, CombatRules& rules);
	/// A chance roll (0..1) that honours forceRolls.
	static bool roll(double chance);
};
