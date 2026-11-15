// The damage pipeline: armor formula, armor piercing, damage types, gear modifiers,
// crits, lifesteal and effects (bleeding, healing, stat changes).
#include "TestFramework.h"
#include "TestSupport.h"
#include "AiCharacter.h"
#include "Board.h"
#include "CombatMath.h"
#include "CombatRules.h"
#include "DamageEffect.h"
#include "EffectsHandler.h"
#include "Inventory.h"
#include "AbstractEqSlot.h"
#include "Lifesteal.h"
#include "NormalItems.h"
#include "Player.h"
#include "StatChangingEffect.h"
#include "BaseItemHandler.h"
#include "MeleeWeaponTypes.h"

using giera_test::makeTestWorld;

namespace {
std::shared_ptr<AiCharacter> addEnemy(giera_test::TestWorld& world, Position pos)
{
    auto ai = std::make_shared<AiCharacter>(CharacterTypes::BANDIT_THUG, pos, 1);
    world.board->addAiCharacter(ai);
    return ai;
}

HitInfo plainHit(double damage, DamageTypes type = DamageTypes::SLASHING, double ap = 0.)
{
    HitInfo hit;
    hit.damage = damage;
    hit.type = type;
    hit.armorPiercing = ap;
    return hit;
}

void equipMelee(Player& player, MeleeWeaponTypes type)
{
    auto slot = player.getInventory()->getEqSlot(EqSlotTypes::MELEE);
    slot->removeItem(0, 0);
    slot->insertAcceptedItem(0, 0, BaseItemHandler::generate<MeleeWeapon>(ItemTypes::MELEE_WEAPON, (int)type));
}
}

// ------------------------------------------------------------------ formulas

TEST_CASE("Damage", "armor reduction has diminishing returns")
{
    const double k = 50.;
    CHECK_APPROX(CombatMath::armorReduction(0, k), 0.0);
    CHECK_APPROX(CombatMath::armorReduction(50, k), 0.5);
    CHECK_APPROX(CombatMath::armorReduction(100, k), 2. / 3.);
    CHECK_APPROX(CombatMath::armorReduction(150, k), 0.75);
    CHECK(CombatMath::armorReduction(100000, k) < 1.0); // never complete immunity
    CHECK_APPROX(CombatMath::armorReduction(-10, k), 0.0);
    // every armor point adds the same "effective hp": hp / (1 - reduction) = hp * (1 + armor / k)
    double ehp50 = 1. / (1. - CombatMath::armorReduction(50, k));
    double ehp100 = 1. / (1. - CombatMath::armorReduction(100, k));
    CHECK_APPROX(ehp100 - ehp50, 1.0);
}

TEST_CASE("Damage", "armor piercing removes flat armor, never below zero")
{
    CHECK_APPROX(CombatMath::armorAfterPiercing(40, 15), 25.0);
    CHECK_APPROX(CombatMath::armorAfterPiercing(10, 15), 0.0);
    CHECK_APPROX(CombatMath::armorAfterPiercing(10, -5), 10.0);
}

TEST_CASE("Damage", "modifier percentages")
{
    CHECK_APPROX(CombatMath::percentMultiplier(20), 1.2);
    CHECK_APPROX(CombatMath::percentMultiplier(-15), 0.85);
    CHECK_APPROX(CombatMath::percentMultiplier(-150), 0.0);
    CHECK_APPROX(CombatMath::durationScale(25), 0.8);
    CHECK_APPROX(CombatMath::durationScale(0), 1.0);
    CHECK(CombatMath::durationScale(-15) > 1.15);
    CHECK_APPROX(CombatMath::durationScale(-1000), 10.0); // clamped at -90 %
}

TEST_CASE("Damage", "only physical damage types meet armor")
{
    CHECK(CombatMath::isPhysical(DamageTypes::SLASHING));
    CHECK(CombatMath::isPhysical(DamageTypes::PIERCING));
    CHECK(CombatMath::isPhysical(DamageTypes::BLUDGEONING));
    CHECK(!CombatMath::isPhysical(DamageTypes::FIRE));
    CHECK(!CombatMath::isPhysical(DamageTypes::POISON));
}

// ------------------------------------------------------------------ armor on characters

TEST_CASE("Damage", "body armor effectiveness depends on the damage type")
{
    auto world = makeTestWorld();
    auto player = world.player;
    auto armor = player->getSelectedArmor();
    REQUIRE(armor != nullptr);
    // Armor.csv body_block: 100% vs slashing, 60% vs piercing, 50% vs bludgeoning
    CHECK_APPROX(player->getArmorAgainst(DamageTypes::SLASHING), armor->getArmor());
    CHECK_APPROX(player->getArmorAgainst(DamageTypes::PIERCING), armor->getArmor() * 0.6);
    CHECK_APPROX(player->getArmorAgainst(DamageTypes::BLUDGEONING), armor->getArmor() * 0.5);
    CHECK_APPROX(player->getArmorAgainst(DamageTypes::FIRE), 0.0);
}

TEST_CASE("Damage", "a raised shield counts fully against every physical type")
{
    auto world = makeTestWorld();
    auto player = world.player;
    double before = player->getArmorAgainst(DamageTypes::BLUDGEONING);
    player->parry(Time(5000));
    CHECK_APPROX(player->getArmorAgainst(DamageTypes::BLUDGEONING), before + player->getSelectedShield()->getArmor());
    CHECK_APPROX(player->getArmorAgainst(DamageTypes::FROST), 0.0);
}

TEST_CASE("Damage", "hit damage goes through the armor formula")
{
    auto world = makeTestWorld();
    auto player = world.player;
    double reduction = player->getDamageReduction(DamageTypes::SLASHING, 0);
    REQUIRE(reduction > 0.1);
    auto outcome = player->receiveHit(plainHit(60));
    CHECK(outcome.landed);
    CHECK_APPROX(outcome.reduction, reduction);
    CHECK_BETWEEN((double)outcome.dealt, 60 * (1 - reduction) - 1, 60 * (1 - reduction) + 1);
    CHECK_EQ(player->getHp(), 100u - outcome.dealt);
}

TEST_CASE("Damage", "armor piercing lowers the reduction")
{
    auto world = makeTestWorld();
    auto player = world.player;
    double armor = player->getArmorAgainst(DamageTypes::SLASHING);
    CHECK(player->getDamageReduction(DamageTypes::SLASHING, 10) < player->getDamageReduction(DamageTypes::SLASHING, 0));
    CHECK_APPROX(player->getDamageReduction(DamageTypes::SLASHING, armor + 5), 0.0);
}

TEST_CASE("Damage", "elemental damage ignores armor")
{
    auto world = makeTestWorld();
    auto player = world.player;
    player->parry(Time(5000));
    auto outcome = player->receiveHit(plainHit(30, DamageTypes::FIRE));
    CHECK_APPROX(outcome.reduction, 0.0);
    CHECK_EQ(outcome.dealt, 30u);
}

TEST_CASE("Damage", "enemies have base armor from the rules")
{
    auto world = makeTestWorld();
    auto ai = addEnemy(world, Position(6, 6, 0));
    CHECK_APPROX(ai->getArmorAgainst(DamageTypes::PIERCING), CombatRules::get().enemyArmor);
    CHECK_APPROX(ai->getArmorAgainst(DamageTypes::FIRE), 0.0);
}

// ------------------------------------------------------------------ crits

TEST_CASE("Damage", "no crit when the roll fails")
{
    auto world = makeTestWorld(); // forceRolls = false
    auto hit = world.player->makeMeleeHit();
    CHECK(!hit.critical);
    CHECK_APPROX(hit.damage, world.player->getSelectedWeapon()->getDamage()->getValue());
}

TEST_CASE("Damage", "a crit multiplies by the weapon's CRIT_DAMAGE")
{
    auto world = makeTestWorld();
    CombatRules::get().forceRolls = true;
    auto player = world.player;
    auto weapon = player->getSelectedWeapon();
    double critPct = weapon->getModifier(ModifiersTypes::CRIT_DAMAGE); // world_destroyer: 150
    REQUIRE(critPct > 0);
    auto hit = player->makeMeleeHit();
    CHECK(hit.critical);
    CHECK_APPROX(hit.damage, weapon->getDamage()->getValue() * critPct / 100.);
}

TEST_CASE("Damage", "weapons without CRIT_DAMAGE use the default crit multiplier")
{
    auto world = makeTestWorld();
    CombatRules::get().forceRolls = true;
    auto player = world.player;
    equipMelee(*player, MeleeWeaponTypes::SWORD_2); // lifestealer: CRIT_DAMAGE 0
    auto weapon = player->getSelectedWeapon();
    REQUIRE(weapon->getModifier(ModifiersTypes::CRIT_DAMAGE) == 0);
    auto hit = player->makeMeleeHit();
    CHECK_APPROX(hit.damage, weapon->getDamage()->getValue() * CombatRules::get().defaultCritDamagePct / 100.);
}

TEST_CASE("Damage", "crit chance is the base chance plus gear modifiers")
{
    auto world = makeTestWorld();
    auto player = world.player;
    auto weapon = player->getSelectedWeapon();
    // world_destroyer CRIT_CHANCE 5 + base 5 = 10 %; checked statistically
    CombatRules::get().forceRolls.reset();
    int crits = 0;
    const int n = 4000;
    for (int i = 0; i < n; i++) {
        crits += player->makeMeleeHit().critical;
    }
    double expected = (CombatRules::get().baseCritChancePct + player->getModifierSum(ModifiersTypes::CRIT_CHANCE, weapon.get())) / 100.;
    CHECK_BETWEEN((double)crits / n, expected - 0.025, expected + 0.025);
}

TEST_CASE("Damage", "DAMAGE modifier of worn gear scales every attack")
{
    auto world = makeTestWorld();
    auto player = world.player;
    double base = player->makeMeleeHit().damage;
    // a worn item's DAMAGE modifier affects every attack: give the armor +20 %
    auto armor = player->getSelectedArmor();
    string name = "test_armor", desc = "test";
    vector<item_mod_t> mods((int)ModifiersTypes::COUNT_MODIFIERS, 0);
    mods[(int)ModifiersTypes::DAMAGE] = 20;
    auto boosted = std::make_shared<Armor>(3, 4, 1, name, desc, mods, armor->getArmor(), ArmorTypes::ARMOR_1);
    auto slot = player->getInventory()->getEqSlot(EqSlotTypes::ARMOR);
    slot->removeItem(0, 0);
    slot->insertAcceptedItem(0, 0, boosted);
    CHECK_APPROX(player->makeMeleeHit().damage, base * 1.2);
}

// ------------------------------------------------------------------ lifesteal

TEST_CASE("Damage", "lifesteal heals the attacker by a share of the damage dealt")
{
    auto world = makeTestWorld();
    CombatRules::get().forceRolls = true; // the lifesteal chance always succeeds (crits too)
    auto player = world.player;
    equipMelee(*player, MeleeWeaponTypes::SWORD_2);
    auto lifesteal = dynamic_cast<Lifesteal*>(player->getSelectedWeapon()->getDamage().get());
    REQUIRE(lifesteal != nullptr);
    player->takeDamage(60);
    auto ai = addEnemy(world, Position(6, 6, 0));
    auto outcome = ai->receiveHit(player->makeMeleeHit());
    REQUIRE(outcome.dealt > 0);
    double healed = outcome.dealt * lifesteal->getPercentStolen() / 100.;
    CHECK_BETWEEN((double)player->getHp(), 40 + healed - 1, 40 + healed + 1);
}

TEST_CASE("Damage", "lifesteal never heals above max hp, and needs the roll")
{
    auto world = makeTestWorld();
    auto player = world.player;
    equipMelee(*player, MeleeWeaponTypes::SWORD_2);
    auto ai = addEnemy(world, Position(6, 6, 0));
    CombatRules::get().forceRolls = true;
    ai->receiveHit(player->makeMeleeHit());
    CHECK_EQ(player->getHp(), player->getMaxHp());
    CombatRules::get().forceRolls = false; // the roll fails: no healing
    player->takeDamage(50);
    ai->receiveHit(player->makeMeleeHit());
    CHECK_EQ(player->getHp(), 50u);
}

// ------------------------------------------------------------------ effects

TEST_CASE("Damage", "bleeding from an arrow ticks over time and ignores armor")
{
    auto world = makeTestWorld();
    auto player = world.player;
    auto bleed = EffectsHandler::getEffect<DamageEffect>(EffectTypes::DAMAGE_EFFECT, 0); // 1 dmg / 250 ms for 2 s
    REQUIRE(bleed != nullptr);
    HitInfo hit = plainHit(0);
    hit.effects.push_back(bleed);
    player->parry(Time(5000)); // lots of armor - doesn't matter for bleeding
    player->receiveHit(hit);
    REQUIRE(player->getActiveEffects().size() == 1);
    unsigned before = player->getHp();
    for (int t = 0; t < 2000; t += 50) {
        player->updateAttack(Time(50));
    }
    CHECK_EQ(before - player->getHp(), 8u); // 8 ticks x 1
    CHECK(player->getActiveEffects().empty());
}

TEST_CASE("Damage", "the same effect refreshes instead of stacking")
{
    auto world = makeTestWorld();
    auto player = world.player;
    auto bleed = EffectsHandler::getEffect<DamageEffect>(EffectTypes::DAMAGE_EFFECT, 0);
    player->addEffect(bleed, {});
    player->updateAttack(Time(1500));
    player->addEffect(bleed, {});
    CHECK_EQ(player->getActiveEffects().size(), (size_t)1);
    CHECK_EQ(player->getActiveEffects()[0]->getTimeLeft().getTimeMs(), bleed->getDuration().getTimeMs());
}

TEST_CASE("Damage", "long frames don't lose effect ticks")
{
    auto world = makeTestWorld();
    auto player = world.player;
    player->addEffect(EffectsHandler::getEffect<DamageEffect>(EffectTypes::DAMAGE_EFFECT, 0), {});
    unsigned before = player->getHp();
    player->updateAttack(Time(1000)); // one 1 s frame = 4 ticks
    CHECK_EQ(before - player->getHp(), 4u);
}

TEST_CASE("Damage", "healing effect restores hp up to the maximum")
{
    auto world = makeTestWorld();
    auto player = world.player;
    auto heal = EffectsHandler::getEffect<DamageEffect>(EffectTypes::DAMAGE_EFFECT, 1); // berries: 2 hp / 300 ms for 3 s
    REQUIRE(heal != nullptr);
    player->takeDamage(50);
    player->addEffect(heal, {});
    for (int t = 0; t < 3000; t += 100) {
        player->updateAttack(Time(100));
    }
    CHECK_EQ(player->getHp(), 70u); // 10 ticks x 2
    for (int meal = 0; meal < 2; meal++) { // 70 -> 90 -> 100 (capped, not 110)
        player->addEffect(heal, {});
        for (int t = 0; t < 3000; t += 100) {
            player->updateAttack(Time(100));
        }
    }
    CHECK_EQ(player->getHp(), player->getMaxHp());
}

TEST_CASE("Damage", "stat effects change movement speed and damage")
{
    auto world = makeTestWorld();
    auto player = world.player;
    double normalSpeed = player->getMoveSpeedMultiplier();
    double normalDamage = player->makeMeleeHit().damage;
    auto slow = std::make_shared<StatChangingEffect>(Time(1000), false, 1, std::weak_ptr<AbstractCharacter>{},
        std::weak_ptr<AbstractCharacter>{}, 50., 50., Time(0), CharacterAttributeTypes::MOVEMENT_SPEED);
    auto strong = std::make_shared<StatChangingEffect>(Time(1000), true, 1, std::weak_ptr<AbstractCharacter>{},
        std::weak_ptr<AbstractCharacter>{}, 150., 150., Time(0), CharacterAttributeTypes::DMG);
    player->addEffect(slow, {});
    player->addEffect(strong, {});
    CHECK_APPROX(player->getMoveSpeedMultiplier(), normalSpeed * 0.5);
    CHECK_APPROX(player->makeMeleeHit().damage, normalDamage * 1.5);
    player->updateAttack(Time(1000));
    CHECK(player->getActiveEffects().empty());
    CHECK_APPROX(player->getMoveSpeedMultiplier(), normalSpeed);
}

TEST_CASE("Damage", "death clears effects")
{
    auto world = makeTestWorld();
    auto player = world.player;
    player->addEffect(EffectsHandler::getEffect<DamageEffect>(EffectTypes::DAMAGE_EFFECT, 0), {});
    player->takeDamage(1000);
    CHECK(player->getActiveEffects().empty());
}
