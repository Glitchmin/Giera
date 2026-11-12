#include "TestFramework.h"
#include "TestSupport.h"
#include "AiCharacter.h"
#include "Board.h"
#include "Player.h"
#include "Shield.h"
#include <cmath>

using giera_test::makeTestWorld;

namespace {
std::shared_ptr<AiCharacter> addEnemy(giera_test::TestWorld& world, Position pos)
{
    auto ai = std::make_shared<AiCharacter>(CharacterTypes::BANDIT_THUG, pos, 1);
    world.board->addAiCharacter(ai);
    return ai;
}

void runProjectiles(giera_test::TestWorld& world, int ms)
{
    for (int t = 0; t < ms; t += 16) {
        world.board->calculateProjectiles(Time(16));
    }
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
    player->startAttack(Position(5, 5, 0));
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
    CHECK_APPROX(player->getPosition().getX(), 4.0);
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
    player->move(Position(1.4, 0, 0)); // right edge of the player would enter the rock tile
    CHECK_APPROX(player->getPosition().getX(), 1.5);
}

TEST_CASE("Combat", "one shot creates exactly one projectile (regression)")
{
    auto world = makeTestWorld();
    world.player->meleeMode = false;
    world.player->startAttack(Position(8, 1.5, 0.5));
    // The old code filled the board up to 5 projectiles per click.
    CHECK_EQ(world.board->getProjectiles().size(), (size_t)1);
}

TEST_CASE("Combat", "ranged attack has a cooldown")
{
    auto world = makeTestWorld();
    auto player = world.player;
    player->startAttack(Position(8, 1.5, 0.5));
    player->startAttack(Position(8, 1.5, 0.5));
    CHECK_EQ(world.board->getProjectiles().size(), (size_t)1);
    player->updateAttack(Time(450));
    player->startAttack(Position(8, 1.5, 0.5));
    CHECK_EQ(world.board->getProjectiles().size(), (size_t)2);
}

TEST_CASE("Combat", "spell projectile damages the enemy")
{
    auto world = makeTestWorld();
    auto ai = addEnemy(world, Position(6, 1.5, 0));
    world.player->startAttack(ai->getPosition() + Position(0, 0, 0.5));
    runProjectiles(world, 1000);
    CHECK_EQ(ai->getHp(), 80u);
    CHECK(world.board->getProjectiles().empty());
}

TEST_CASE("Combat", "projectile does not hit its shooter")
{
    auto world = makeTestWorld();
    world.player->startAttack(Position(10, 10, 0.5));
    runProjectiles(world, 1000);
    CHECK_EQ(world.player->getHp(), 100u);
}

TEST_CASE("Combat", "five spell hits kill and dead enemies are removed")
{
    auto world = makeTestWorld();
    auto target = addEnemy(world, Position(6, 1.5, 0));
    auto bystander = addEnemy(world, Position(6, 10, 0));
    for (int shot = 0; shot < 5; shot++) {
        world.player->updateAttack(Time(500));
        world.player->startAttack(target->getPosition() + Position(0, 0, 0.5));
        runProjectiles(world, 800);
    }
    CHECK(target->isDead());
    CHECK(!bystander->isDead());
    // AiCharacter::die() used to clear *all* enemies; now only the dead one goes.
    CHECK_EQ(world.board->removeDeadCharacters(), 1);
    REQUIRE(world.board->getAiCharacters().size() == 1);
    CHECK(world.board->getAiCharacters()[0] == bystander);
}

TEST_CASE("Combat", "melee attack hits an adjacent enemy")
{
    auto world = makeTestWorld();
    auto player = world.player;
    auto ai = addEnemy(world, Position(2.5, 1.5, 0));
    player->meleeMode = true;
    player->startAttack(ai->getPosition());
    CHECK(!player->canMove()); // swinging
    player->updateAttack(Time(300)); // wind-up done -> hit resolved
    CHECK(ai->getHp() < 100u);
    CHECK(ai->getHp() >= 97u);       // weapon damage is 1-2
    player->updateAttack(Time(700)); // cooldown
    player->updateAttack(Time(1));
    CHECK(player->canMove());
}

TEST_CASE("Combat", "melee attack misses a distant enemy")
{
    auto world = makeTestWorld();
    auto ai = addEnemy(world, Position(8, 8, 0));
    world.player->meleeMode = true;
    world.player->startAttack(ai->getPosition());
    world.player->updateAttack(Time(300));
    CHECK_EQ(ai->getHp(), 100u);
}

TEST_CASE("Combat", "switching weapon mid-swing does not freeze the player (regression)")
{
    auto world = makeTestWorld();
    auto player = world.player;
    player->meleeMode = true;
    player->startAttack(Position(5, 1.5, 0));
    player->meleeMode = false;
    for (int i = 0; i < 20; i++) {
        player->updateAttack(Time(100));
    }
    CHECK(player->canMove());
}

TEST_CASE("Combat", "melee on own position does not produce NaN")
{
    auto world = makeTestWorld();
    world.player->meleeMode = true;
    world.player->startAttack(world.player->getPosition());
    world.player->updateAttack(Time(300));
    world.player->updateAttack(Time(800));
    CHECK(std::isfinite(world.player->getPosition().getX()));
    CHECK(world.player->canMove());
}

TEST_CASE("Combat", "parry raises the shield and its armor")
{
    auto world = makeTestWorld();
    auto player = world.player;
    REQUIRE(player->getSelectedShield() != nullptr);
    size_t drawablesBefore = player->getDrawables().size();
    CHECK_APPROX(player->getTotalArmor(), 0.0);

    player->parry(Time(250));
    CHECK_APPROX(player->getParryCompleteness(), 0.5);
    player->parry(Time(300));
    CHECK_APPROX(player->getParryCompleteness(), 1.0);
    CHECK_APPROX(player->getTotalArmor(), player->getSelectedShield()->getArmor());
    CHECK_EQ(player->getDrawables().size(), drawablesBefore + 1); // shield shown

    player->cancelParry();
    CHECK_APPROX(player->getParryCompleteness(), 0.0);
    CHECK_EQ(player->getDrawables().size(), drawablesBefore);
}

TEST_CASE("Combat", "parry reduces melee damage")
{
    auto world = makeTestWorld();
    auto player = world.player;
    auto ai = addEnemy(world, Position(2.5, 1.5, 0));
    player->parry(Time(250));
    player->parry(Time(300));
    REQUIRE(player->getTotalArmor() > 0);
    ai->meleeMode = true;
    ai->startAttack(player->getPosition());
    ai->updateAttack(Time(300));
    // weapon damage 1-2 minus shield armor 1-2 -> at most 1
    CHECK(player->getHp() >= 99u);
}

TEST_CASE("Combat", "enemy walks toward the player")
{
    auto world = makeTestWorld();
    auto ai = addEnemy(world, Position(6.5, 1.5, 0));
    ai->updateBehaviour(Time(1000));
    CHECK_APPROX(ai->getPosition().getX(), 5.5);
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

TEST_CASE("Combat", "enemy in range shoots the player")
{
    auto world = makeTestWorld();
    auto ai = addEnemy(world, Position(2.2, 1.5, 0));
    ai->updateBehaviour(Time(16));
    CHECK_EQ(world.board->getProjectiles().size(), (size_t)1);
    runProjectiles(world, 500);
    CHECK_EQ(world.player->getHp(), 80u);
}
