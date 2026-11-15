// Characters, melee, shield and enemy behaviour. Bow / arrows: TestBow.cpp.
// Damage formulas: TestDamage.cpp. Stagger / parry / dash / eating: TestReactions.cpp.
// Numbers come from assets/rules/Combat.txt (CombatRules) and the item CSVs.
#include "TestFramework.h"
#include "TestSupport.h"
#include "AiCharacter.h"
#include "Board.h"
#include "CombatMath.h"
#include "CombatRules.h"
#include "Inventory.h"
#include "AbstractEqSlot.h"
#include "NormalItems.h"
#include "Player.h"
#include <cmath>
#include <sstream>

using giera_test::makeTestWorld;

namespace {
std::shared_ptr<AiCharacter> addEnemy(giera_test::TestWorld& world, Position pos)
{
    auto ai = std::make_shared<AiCharacter>(CharacterTypes::BANDIT_THUG, pos, 1);
    world.board->addAiCharacter(ai);
    return ai;
}

void raiseShieldFully(Player& player)
{
    player.parry(Time(5000));
}

/// Damage a melee hit of @p attacker would deal to @p target (no crit), before rounding.
double expectedMelee(AbstractCharacter& attacker, AbstractCharacter& target)
{
    auto weapon = attacker.getSelectedWeapon();
    double damage = weapon->getDamage()->getValue() *
        CombatMath::percentMultiplier(attacker.getModifierSum(ModifiersTypes::DAMAGE, weapon.get()));
    double ap = weapon->getDamage()->getAp() + attacker.getModifierSum(ModifiersTypes::ARMOR_PIERCING, weapon.get());
    return damage * (1. - target.getDamageReduction(weapon->getDamage()->getDamageType(), ap));
}
}

TEST_CASE("Combat", "takeDamage subtracts and clamps at zero (regression)")
{
    auto world = makeTestWorld();
    auto& player = *world.player;
    REQUIRE(player.getHp() == 100u);
    CHECK(!player.takeDamage(30));
    CHECK_EQ(player.getHp(), 70u);
    // hp is unsigned: the old "hp -= dmg; if (hp <= 0)" wrapped to ~4e9 and never died.
    CHECK(player.takeDamage(1000));
    CHECK_EQ(player.getHp(), 0u);
    CHECK(player.isDead());
    CHECK(!player.takeDamage(5)); // already dead: no second death
}

TEST_CASE("Combat", "dead characters cannot move or attack")
{
    auto world = makeTestWorld();
    auto player = world.player;
    player->takeDamage(player->getMaxHp());
    REQUIRE(player->isDead());
    CHECK(!player->canMove());
    CHECK(!player->canAttack());
    Position before = player->getPosition();
    player->move(Position(1, 0, 0));
    CHECK(player->getPosition() == before);
    player->setMeleeMode(false);
    player->startAttack(Position(5, 5, 0));
    CHECK(!player->isDrawing());
    CHECK(world.board->getProjectiles().empty());
}

TEST_CASE("Combat", "revive restores hp and position")
{
    auto world = makeTestWorld();
    auto player = world.player;
    player->takeDamage(500);
    player->revive(Position(3.5, 4.5, 0));
    CHECK(!player->isDead());
    CHECK_EQ(player->getHp(), player->getMaxHp());
    CHECK(player->getPosition() == Position(3.5, 4.5, 0));
    CHECK(player->canMove());
    player->move(Position(0.5, 0, 0));
    CHECK_APPROX(player->getPosition().getX(), 3.5 + 0.5 * player->getMoveSpeedMultiplier());
}

TEST_CASE("Combat", "move with NaN is ignored")
{
    auto world = makeTestWorld();
    Position before = world.player->getPosition();
    world.player->move(Position(std::nan(""), 0, 0));
    CHECK(world.player->getPosition() == before);
}

TEST_CASE("Combat", "walls and map edges block movement")
{
    auto world = makeTestWorld(8, 8);
    auto player = world.player;
    player->move(Position(-5, 0, 0)); // off the map
    CHECK_APPROX(player->getPosition().getX(), 1.5);
    Position p(3, 1, 0);
    world.board->getMap()->setMapTile(Coordinates(3, 1), std::make_shared<MapTile>(p, TerrainTypes::GRASS,
        Rotations::UP, ForegroundTypes::NONE, BackgroundTypes::NONE, WallTypes::ROCK));
    player->move(Position(2, 0, 0)); // right edge of the player would enter the rock tile
    CHECK_APPROX(player->getPosition().getX(), 1.5);
}

TEST_CASE("Combat", "player starts fully equipped")
{
    auto world = makeTestWorld();
    auto player = world.player;
    CHECK(player->getSelectedWeapon() != nullptr);
    CHECK(player->getSelectedShield() != nullptr);
    CHECK(player->getSelectedArmor() != nullptr);
    CHECK(player->getSelectedBow() != nullptr);
    CHECK_EQ(player->getArrowCount(), (int)CombatRules::get().startingArrows);
    CHECK_EQ(player->getFoodCount(), (int)CombatRules::get().startingFood);
    CHECK(player->meleeMode);
    CHECK_EQ(player->getHp(), CombatRules::get().playerMaxHp);
}

TEST_CASE("Combat", "swing time comes from the weapon AT_SPD and attack speed modifiers")
{
    auto world = makeTestWorld();
    auto player = world.player;
    auto sword = std::dynamic_pointer_cast<MeleeWeapon>(player->getSelectedWeapon());
    REQUIRE(sword != nullptr);
    double total = sword->getAttackSpeed().getTimeMs() *
        CombatMath::durationScale(player->getModifierSum(ModifiersTypes::ATTACK_SPEED, sword.get()));
    auto timings = player->getMeleeTimings();
    CHECK_BETWEEN((double)(timings.first.getTimeMs() + timings.second.getTimeMs()), total - 1, total + 1);
    CHECK_BETWEEN((double)timings.first.getTimeMs(), total * CombatRules::get().meleeWindupShare - 1,
        total * CombatRules::get().meleeWindupShare + 1);

    // the shield's -15% attack speed makes swings slower; without it they are faster
    player->getInventory()->getEqSlot(EqSlotTypes::SHIELD)->removeItem(0, 0);
    auto noShield = player->getMeleeTimings();
    CHECK(noShield.first.getTimeMs() + noShield.second.getTimeMs() < timings.first.getTimeMs() + timings.second.getTimeMs());
}

TEST_CASE("Combat", "melee attack hits an adjacent enemy through its armor")
{
    auto world = makeTestWorld();
    auto player = world.player;
    auto ai = addEnemy(world, Position(2.5, 1.5, 0));
    double expected = expectedMelee(*player, *ai);
    auto timings = player->getMeleeTimings();
    player->startAttack(ai->getPosition());
    CHECK(!player->canMove()); // swinging
    player->updateAttack(timings.first); // wind-up done -> hit resolved
    CHECK_BETWEEN((double)ai->getHp(), 100. - expected - 1., 100. - expected + 1.);
    CHECK(ai->getHp() < 100u);
    player->updateAttack(timings.second);
    player->updateAttack(Time(1));
    CHECK(player->canMove());
}

TEST_CASE("Combat", "melee without a weapon uses bare hands")
{
    auto world = makeTestWorld();
    auto player = world.player;
    player->getInventory()->getEqSlot(EqSlotTypes::MELEE)->removeItem(0, 0);
    REQUIRE(player->getSelectedWeapon() == nullptr);
    auto ai = addEnemy(world, Position(2.5, 1.5, 0));
    auto timings = player->getMeleeTimings();
    // bare hands: CombatRules swing time (scaled by the shield's attack speed penalty)
    double total = CombatRules::get().bareHandsSwingMs *
        CombatMath::durationScale(player->getModifierSum(ModifiersTypes::ATTACK_SPEED, nullptr));
    CHECK_BETWEEN((double)(timings.first.getTimeMs() + timings.second.getTimeMs()), total - 1, total + 1);
    player->startAttack(ai->getPosition());
    player->updateAttack(timings.first);
    double expected = CombatRules::get().bareHandsDamage * (1. - ai->getDamageReduction(DamageTypes::BLUDGEONING, 0));
    CHECK_BETWEEN((double)ai->getHp(), 100. - expected - 1., 100. - expected + 1.);
}

TEST_CASE("Combat", "melee attack misses a distant enemy")
{
    auto world = makeTestWorld();
    auto ai = addEnemy(world, Position(8, 8, 0));
    world.player->startAttack(ai->getPosition());
    world.player->updateAttack(world.player->getMeleeTimings().first);
    CHECK_EQ(ai->getHp(), 100u);
}

TEST_CASE("Combat", "switching weapon mid-swing does not freeze the player (regression)")
{
    auto world = makeTestWorld();
    auto player = world.player;
    player->startAttack(Position(5, 1.5, 0));
    player->toggleWeaponMode();
    for (int i = 0; i < 30; i++) {
        player->updateAttack(Time(100));
    }
    CHECK(player->canMove());
}

TEST_CASE("Combat", "melee on own position does not produce NaN")
{
    auto world = makeTestWorld();
    world.player->startAttack(world.player->getPosition());
    for (int i = 0; i < 20; i++) {
        world.player->updateAttack(Time(100));
    }
    CHECK(std::isfinite(world.player->getPosition().getX()));
    CHECK(world.player->canMove());
}

TEST_CASE("Combat", "shield rises over its raise time and adds its armor")
{
    auto world = makeTestWorld();
    auto player = world.player;
    auto shield = player->getSelectedShield();
    REQUIRE(shield != nullptr);
    size_t drawablesBefore = player->getDrawables().size();
    double bodyOnly = player->getArmorAgainst(DamageTypes::SLASHING);

    unsigned raise = shield->getTimeToRaise().getTimeMs();
    player->parry(Time(raise / 2));
    CHECK_BETWEEN(player->getParryCompleteness(), 0.45, 0.55);
    CHECK_BETWEEN(player->getArmorAgainst(DamageTypes::SLASHING) - bodyOnly,
        shield->getArmor() * 0.45, shield->getArmor() * 0.55);
    player->parry(Time(raise));
    CHECK_APPROX(player->getParryCompleteness(), 1.0);
    CHECK_APPROX(player->getArmorAgainst(DamageTypes::SLASHING), bodyOnly + shield->getArmor());
    CHECK_EQ(player->getDrawables().size(), drawablesBefore + 1); // shield shown

    player->cancelParry();
    CHECK_APPROX(player->getParryCompleteness(), 0.0);
    CHECK_APPROX(player->getArmorAgainst(DamageTypes::SLASHING), bodyOnly);
    CHECK_EQ(player->getDrawables().size(), drawablesBefore);
}

TEST_CASE("Combat", "raising the shield while walking is slower")
{
    auto world = makeTestWorld();
    auto player = world.player;
    unsigned raise = player->getSelectedShield()->getTimeToRaise().getTimeMs();
    player->move(Position(0.1, 0, 0));
    player->parry(Time(raise / 2));
    CHECK_BETWEEN(player->getParryCompleteness(), 0.2, 0.3); // half speed
}

TEST_CASE("Combat", "walking with the shield up is slower")
{
    auto world = makeTestWorld();
    auto player = world.player;
    double normal = player->getMoveSpeedMultiplier();
    raiseShieldFully(*player);
    CHECK_APPROX(player->getMoveSpeedMultiplier(), normal * CombatRules::get().moveSpeedWhileBusy);
    player->move(Position(1, 0, 0));
    CHECK_APPROX(player->getPosition().getX(), 1.5 + player->getMoveSpeedMultiplier());
}

TEST_CASE("Combat", "shield movement penalty applies while it is equipped")
{
    auto world = makeTestWorld();
    auto player = world.player;
    double withShield = player->getMoveSpeedMultiplier();
    double penalty = player->getSelectedShield()->getModifier(ModifiersTypes::MOVEMENT_SPEED); // -10 %
    REQUIRE(penalty < 0);
    player->getInventory()->getEqSlot(EqSlotTypes::SHIELD)->removeItem(0, 0);
    CHECK_APPROX(withShield, player->getMoveSpeedMultiplier() * CombatMath::percentMultiplier(penalty));
}

TEST_CASE("Combat", "attacking lowers the shield")
{
    auto world = makeTestWorld();
    auto player = world.player;
    raiseShieldFully(*player);
    player->startAttack(Position(5, 1.5, 0));
    CHECK_APPROX(player->getParryCompleteness(), 0.0);
}

TEST_CASE("Combat", "raised shield reduces enemy sword damage")
{
    auto world = makeTestWorld();
    auto player = world.player;
    auto ai = addEnemy(world, Position(2.5, 1.5, 0));
    raiseShieldFully(*player);
    double expected = expectedMelee(*ai, *player);
    ai->startAttack(player->getPosition());
    ai->updateAttack(ai->getMeleeTimings().first);
    CHECK_BETWEEN((double)player->getHp(), 100. - expected - 1., 100. - expected + 1.);
    // shield 40-60 + body armor 15-25 (minus the enemy's piercing): about half of the hit is stopped
    CHECK(expected < ai->getSelectedWeapon()->getDamage()->getValue() * 0.55);
}

TEST_CASE("Combat", "enemy walks toward the player")
{
    auto world = makeTestWorld();
    auto ai = addEnemy(world, Position(6.5, 1.5, 0));
    ai->updateBehaviour(Time(1000));
    CHECK_APPROX(ai->getPosition().getX(), 6.5 - CombatRules::get().enemyWalkSpeed);
    CHECK_APPROX(ai->getPosition().getY(), 1.5);
}

TEST_CASE("Combat", "enemy on top of the player stays finite (regression)")
{
    auto world = makeTestWorld();
    auto ai = addEnemy(world, world.player->getPosition());
    ai->updateBehaviour(Time(100));
    CHECK(std::isfinite(ai->getPosition().getX()));
    CHECK(std::isfinite(ai->getPosition().getY()));
}

TEST_CASE("Combat", "enemy ignores a dead player")
{
    auto world = makeTestWorld();
    auto ai = addEnemy(world, Position(6.5, 1.5, 0));
    world.player->takeDamage(1000);
    ai->updateBehaviour(Time(1000));
    CHECK_APPROX(ai->getPosition().getX(), 6.5);
}

TEST_CASE("Combat", "enemy in range swings a sword and never shoots")
{
    auto world = makeTestWorld();
    auto ai = addEnemy(world, Position(2.2, 1.5, 0));
    REQUIRE(ai->getSelectedWeapon() != nullptr);
    double expected = expectedMelee(*ai, *world.player);
    ai->updateBehaviour(Time(16));
    CHECK(world.board->getProjectiles().empty());
    CHECK(ai->isAttacking());
    ai->updateBehaviour(ai->getMeleeTimings().first);
    CHECK(world.board->getProjectiles().empty());
    CHECK_BETWEEN((double)world.player->getHp(), 100. - expected - 1., 100. - expected + 1.);
    CHECK(world.player->getHp() < 100u);
}

TEST_CASE("Combat", "rules file format: comments, unknown keys, bad values")
{
    CombatRules rules;
    std::istringstream text(
        "# a comment\n"
        "playerMaxHp 250   # trailing comment\n"
        "\n"
        "moveSpeedWhileBusy 0.25\n"
        "noSuchRule 7\n"
        "bareHandsSwingMs abc\n");
    CombatRules::parse(text, rules);
    CHECK_EQ(rules.playerMaxHp, 250u);
    CHECK_APPROX(rules.moveSpeedWhileBusy, 0.25);
    CHECK_EQ(rules.bareHandsSwingMs, 700u); // bad value keeps the default
}

TEST_CASE("Combat", "shipped rules file is loaded")
{
    CHECK(CombatRules::load());
    CHECK_EQ(CombatRules::get().playerMaxHp, 100u);
    CHECK_APPROX(CombatRules::get().armorK, 50.0);
}
