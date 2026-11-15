// The damage pipeline and everything around it: gear modifiers, crits, armor, lifesteal,
// shield / parry reactions, stagger, dash and effects. Rules: docs/FightSystem.md.
#include "AbstractCharacter.h"
#include "Calculator.h"
#include "CombatMath.h"
#include "CombatRules.h"
#include "DamageEffect.h"
#include "Lifesteal.h"
#include "Logger.h"
#include "NormalItems.h"
#include "StatChangingEffect.h"
#include <algorithm>
#include <cmath>
#include <typeinfo>

using std::dynamic_pointer_cast;

// ------------------------------------------------------------------ gear and modifiers

shared_ptr<Armor> AbstractCharacter::getSelectedArmor() const
{
	return nullptr;
}

shared_ptr<AbstractWeapon> AbstractCharacter::getWeaponInHand() const
{
	if (!meleeMode) {
		if (auto bow = getSelectedBow()) {
			return bow;
		}
	}
	return getSelectedWeapon();
}

vector<shared_ptr<AbstractGearItem>> AbstractCharacter::getWornGear() const
{
	vector<shared_ptr<AbstractGearItem>> gear;
	if (auto shield = getSelectedShield()) {
		gear.push_back(shield);
	}
	if (auto armor = getSelectedArmor()) {
		gear.push_back(armor);
	}
	return gear;
}

double AbstractCharacter::getModifierSum(ModifiersTypes type, const AbstractGearItem* weapon) const
{
	double sum = 0.;
	for (auto& item : getWornGear()) {
		sum += item->getModifier(type);
	}
	if (weapon) {
		sum += weapon->getModifier(type);
	}
	return sum;
}

std::pair<Time, Time> AbstractCharacter::getMeleeTimings() const
{
	const auto& rules = CombatRules::get();
	auto weapon = getSelectedWeapon();
	auto melee = dynamic_pointer_cast<MeleeWeapon>(weapon);
	double total = melee ? (double)melee->getAttackSpeed().getTimeMs() : (double)rules.bareHandsSwingMs;
	total *= CombatMath::durationScale(getModifierSum(ModifiersTypes::ATTACK_SPEED, weapon.get()));
	double share = std::clamp(rules.meleeWindupShare, 0., 1.);
	unsigned int windup = (unsigned int)std::lround(total * share);
	unsigned int recovery = (unsigned int)std::lround(total) - windup;
	return { Time(windup), Time(recovery) };
}

Time AbstractCharacter::getDrawTime() const
{
	auto bow = getSelectedBow();
	if (!bow) {
		return Time(0);
	}
	double ms = bow->getDrawTime().getTimeMs() *
		CombatMath::durationScale(getModifierSum(ModifiersTypes::ATTACK_SPEED, bow.get()));
	return Time((unsigned int)std::lround(ms));
}

// ------------------------------------------------------------------ making hits

bool AbstractCharacter::rollCrit(HitInfo& hit, const AbstractGearItem* weapon)
{
	const auto& rules = CombatRules::get();
	double chancePct = rules.baseCritChancePct + getModifierSum(ModifiersTypes::CRIT_CHANCE, weapon);
	if (!CombatRules::roll(chancePct / 100.)) {
		return false;
	}
	double weaponCrit = weapon ? weapon->getModifier(ModifiersTypes::CRIT_DAMAGE) : 0.;
	double multiplierPct = (weaponCrit > 0 ? weaponCrit : rules.defaultCritDamagePct)
		+ getModifierSum(ModifiersTypes::CRIT_DAMAGE, nullptr); // worn gear adds to it
	hit.damage *= std::max(0., multiplierPct / 100.);
	hit.critical = true;
	return true;
}

namespace {
void copyLifesteal(HitInfo& hit, const unique_ptr<Damage>& damage)
{
	if (damage && damage->isLifesteal()) {
		auto& lifesteal = dynamic_cast<Lifesteal&>(*damage);
		hit.lifestealPercent = lifesteal.getPercentStolen();
		hit.lifestealChancePct = lifesteal.getProbability();
	}
}
}

HitInfo AbstractCharacter::makeMeleeHit()
{
	const auto& rules = CombatRules::get();
	HitInfo hit;
	hit.melee = true;
	hit.attacker = weak_from_this();
	auto weapon = getSelectedWeapon();
	if (weapon) {
		hit.damage = weapon->getDamage()->getValue();
		hit.type = weapon->getDamage()->getDamageType();
		hit.armorPiercing = weapon->getDamage()->getAp();
		copyLifesteal(hit, weapon->getDamage());
	}
	else {
		hit.damage = rules.bareHandsDamage;
		hit.type = DamageTypes::BLUDGEONING;
	}
	hit.damage *= CombatMath::percentMultiplier(getModifierSum(ModifiersTypes::DAMAGE, weapon.get()));
	hit.damage *= getStatEffectPercent(CharacterAttributeTypes::DMG) / 100.;
	hit.armorPiercing += getModifierSum(ModifiersTypes::ARMOR_PIERCING, weapon.get());
	rollCrit(hit, weapon.get());
	return hit;
}

HitInfo AbstractCharacter::makeArrowHit(const shared_ptr<RangedWeapon>& bow, const shared_ptr<Arrow>& arrow, double draw)
{
	HitInfo hit;
	hit.attacker = weak_from_this();
	if (!bow || !arrow) {
		return hit;
	}
	hit.damage = (bow->getDamage()->getValue() + arrow->getDamage()->getValue()) * draw;
	hit.type = arrow->getDamage()->getDamageType();
	hit.armorPiercing = bow->getDamage()->getAp() + arrow->getDamage()->getAp()
		+ getModifierSum(ModifiersTypes::ARMOR_PIERCING, bow.get());
	hit.damage *= CombatMath::percentMultiplier(getModifierSum(ModifiersTypes::DAMAGE, bow.get()));
	hit.damage *= getStatEffectPercent(CharacterAttributeTypes::DMG) / 100.;
	copyLifesteal(hit, bow->getDamage());
	hit.effects = arrow->getEffects();
	rollCrit(hit, bow.get());
	return hit;
}

// ------------------------------------------------------------------ receiving hits

double AbstractCharacter::getBaseArmor() const
{
	return 0.;
}

double AbstractCharacter::getArmorAgainst(DamageTypes type) const
{
	if (!CombatMath::isPhysical(type)) {
		return 0.; // fire, frost... ignore armor
	}
	double armor = getBaseArmor();
	if (auto body = getSelectedArmor()) {
		armor += body->getArmor() * body->getEffectiveness(type);
	}
	if (auto shield = getSelectedShield()) {
		armor += shield->getArmor() * parryCompleteness; // full vs every physical type
	}
	return armor;
}

double AbstractCharacter::getDamageReduction(DamageTypes type, double armorPiercing) const
{
	double armor = CombatMath::armorAfterPiercing(getArmorAgainst(type), armorPiercing);
	return CombatMath::armorReduction(armor, CombatRules::get().armorK);
}

HitOutcome AbstractCharacter::receiveHit(const HitInfo& hit)
{
	HitOutcome outcome;
	if (dead || !std::isfinite(hit.damage) || hit.damage < 0.) {
		return outcome;
	}
	const auto& rules = CombatRules::get();
	auto attacker = hit.attacker.lock();

	if (isDashing()) {
		Logger::logDebug("dodged (dash)");
		return outcome; // invulnerable while dashing
	}

	// Timed parry (no shield): deflect, stagger the attacker, strike back at once.
	if (hit.melee && hit.canBeParried && !getSelectedShield() && parryWindowLeft.getTimeMs() > 0) {
		parryWindowLeft = Time(0);
		outcome.parried = true;
		Logger::logInfo("parried - riposte!");
		if (attacker && !attacker->isDead()) {
			attacker->stagger(Time(rules.riposteStaggerMs));
			HitInfo counter = makeMeleeHit();
			counter.canBeParried = false;
			attacker->receiveHit(counter);
		}
		return outcome;
	}

	outcome.landed = true;
	outcome.reduction = getDamageReduction(hit.type, hit.armorPiercing);
	double damage = hit.damage * (1. - outcome.reduction);
	int typeIndex = (int)hit.type;
	if (!CombatMath::isPhysical(hit.type) && typeIndex >= 0 && typeIndex < (int)resitances.size()) {
		damage *= resitances[typeIndex];
	}
	outcome.dealt = Calculator::getIntFromDoubleWithProb(damage);

	// A melee hit on a fully raised shield staggers the attacker (counter-attack window).
	if (hit.melee && getSelectedShield() && parryCompleteness >= 1. && attacker && !attacker->isDead()) {
		outcome.shieldBlocked = true;
		attacker->stagger(Time(rules.shieldBlockStaggerMs));
	}

	outcome.killed = takeDamage(outcome.dealt);
	if (outcome.dealt > 0 && !outcome.killed) {
		cancelDrawing(); // getting hurt lowers the bow (arrow kept)
	}

	if (attacker && !attacker->isDead() && outcome.dealt > 0 && hit.lifestealPercent > 0. &&
		CombatRules::roll(hit.lifestealChancePct / 100.)) {
		attacker->heal(outcome.dealt * hit.lifestealPercent / 100.);
	}

	if (!dead) {
		for (auto& effect : hit.effects) {
			addEffect(effect, hit.attacker);
		}
	}
	return outcome;
}

bool AbstractCharacter::receiveHit(double rawDamage, DamageTypes type)
{
	HitInfo hit;
	hit.damage = rawDamage;
	hit.type = type;
	return receiveHit(hit).killed;
}

void AbstractCharacter::heal(double amount)
{
	if (dead || !(amount > 0.)) {
		return;
	}
	character_hp_t gained = Calculator::getIntFromDoubleWithProb(amount);
	hp = std::min(maxHp, hp + gained);
}

// ------------------------------------------------------------------ reactions

void AbstractCharacter::stagger(Time duration)
{
	if (dead) {
		return;
	}
	// The stagger comes on top of whatever is left of the current swing.
	Time remaining(0);
	if (meleeAttackInfo.has_value()) {
		remaining = meleeAttackInfo->hasStruct ? meleeAttackInfo->cooldownAfterAttack
			: meleeAttackInfo->timeToAttack + meleeAttackInfo->cooldownAfterAttack;
	}
	endMeleeSwing();
	cancelDrawing();
	cancelParry();
	parryWindowLeft = Time(0);
	onInterrupted();
	Time total = remaining + duration;
	if (staggerLeft < total) {
		staggerLeft = total;
	}
	Logger::logDebug("staggered for", total);
}

bool AbstractCharacter::isStaggered() const
{
	return staggerLeft.getTimeMs() > 0;
}

bool AbstractCharacter::isParryWindowOpen() const
{
	return parryWindowLeft.getTimeMs() > 0;
}

Time AbstractCharacter::getParryCooldownLeft() const
{
	return parryCooldownLeft;
}

// ------------------------------------------------------------------ movement and dash

double AbstractCharacter::getMoveSpeedMultiplier() const
{
	const auto& rules = CombatRules::get();
	bool busy = bowDraw.has_value() || parryCompleteness > 0. || isOccupied();
	double multiplier = busy ? rules.moveSpeedWhileBusy : 1.0;
	auto weapon = getWeaponInHand();
	multiplier *= CombatMath::percentMultiplier(getModifierSum(ModifiersTypes::MOVEMENT_SPEED, weapon.get()));
	multiplier *= getStatEffectPercent(CharacterAttributeTypes::MOVEMENT_SPEED) / 100.;
	return std::max(0.05, multiplier);
}

bool AbstractCharacter::startDash()
{
	if (dead || isDashing() || dashCooldownLeft.getTimeMs() > 0 || meleeAttackInfo.has_value() || isStaggered()) {
		return false;
	}
	const auto& rules = CombatRules::get();
	cancelDrawing();
	cancelParry();
	onInterrupted();
	dashDirection = lastMoveDirection;
	dashLeft = Time(rules.dashDurationMs);
	auto weapon = getWeaponInHand();
	double cooldown = rules.dashCooldownMs *
		CombatMath::durationScale(getModifierSum(ModifiersTypes::DODGE_SPEED, weapon.get()));
	dashCooldownLeft = Time(rules.dashDurationMs + (unsigned int)std::lround(cooldown));
	Logger::logDebug("dash");
	return true;
}

bool AbstractCharacter::isDashing() const
{
	return dashLeft.getTimeMs() > 0;
}

Time AbstractCharacter::getDashCooldownLeft() const
{
	return dashCooldownLeft;
}

Position AbstractCharacter::getLastMoveDirection() const
{
	return lastMoveDirection;
}

bool AbstractCharacter::isOccupied() const
{
	return false;
}

void AbstractCharacter::onInterrupted()
{
}

// ------------------------------------------------------------------ effects

namespace {
bool sameKind(const AbstractEffect& a, const AbstractEffect& b)
{
	return typeid(a) == typeid(b) && a.getLevel() == b.getLevel() && a.IsBuff() == b.IsBuff();
}
}

void AbstractCharacter::addEffect(const shared_ptr<AbstractEffect>& effect, weak_ptr<AbstractCharacter> origin)
{
	if (!effect || dead) {
		return;
	}
	auto copy = effect->generate();
	copy->setTargetCharacter(weak_from_this());
	for (auto& active : activeEffects) {
		if (sameKind(*active, *copy)) {
			active = copy; // the same effect again refreshes it
			return;
		}
	}
	activeEffects.push_back(copy);
}

const vector<shared_ptr<AbstractEffect>>& AbstractCharacter::getActiveEffects() const
{
	return activeEffects;
}

void AbstractCharacter::updateEffects(Time timeDiff)
{
	if (activeEffects.empty() || timeDiff.getTimeMs() == 0) {
		return;
	}
	auto effects = activeEffects; // ticks may kill (die() clears the list)
	for (auto& effect : effects) {
		if (dead) {
			return;
		}
		if (auto damageEffect = dynamic_pointer_cast<DamageEffect>(effect)) {
			double amount = damageEffect->advance(timeDiff);
			if (amount > 0.) {
				if (damageEffect->getDamage()->getDamageType() == DamageTypes::HEALING) {
					heal(amount);
				}
				else {
					// damage over time ignores armor and doesn't interrupt anything
					takeDamage(Calculator::getIntFromDoubleWithProb(amount));
				}
			}
		}
		else {
			effect->subtractFromTimeLeft(timeDiff);
		}
	}
	if (!dead) {
		activeEffects.erase(std::remove_if(activeEffects.begin(), activeEffects.end(),
			[](const shared_ptr<AbstractEffect>& e) { return e->getTimeLeft().getTimeMs() == 0; }),
			activeEffects.end());
	}
}

double AbstractCharacter::getStatEffectPercent(CharacterAttributeTypes attribute) const
{
	double percent = 100.;
	for (auto& effect : activeEffects) {
		auto stat = dynamic_pointer_cast<StatChangingEffect>(effect);
		if (stat && stat->getAttributeType() == attribute) {
			percent *= stat->getCurrentValue() / 100.;
		}
	}
	return percent;
}
