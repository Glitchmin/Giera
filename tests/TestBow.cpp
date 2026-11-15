// Bow, arrows, breaking and picking arrows up again.
#include "TestFramework.h"
#include "TestSupport.h"
#include "AiCharacter.h"
#include "ArrowProjectile.h"
#include "Board.h"
#include "CombatRules.h"
#include "Inventory.h"
#include "AbstractEqSlot.h"
#include "NormalItems.h"
#include "Player.h"

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

/// An arrow with fixed numbers (the CSV ones are random).
shared_ptr<Arrow> makeArrow(double breakChance, double damage = 10.)
{
    string name = "test_arrow", desc = "test";
    auto dmg = std::make_unique<Damage>(damage, 0., DamageTypes::PIERCING);
    return std::make_shared<Arrow>(1, 2, 1, name, desc, dmg, vector<shared_ptr<AbstractEffect>>{},
        ArrowTypes::ARROW_1, breakChance);
}

HitInfo arrowHit(double damage)
{
    HitInfo hit;
    hit.damage = damage;
    hit.type = DamageTypes::PIERCING;
    return hit;
}

/// Empties the quiver and puts @p count test arrows in.
void fillQuiver(Player& player, int count, double breakChance)
{
    while (player.takeArrow()) {
    }
    auto quiver = player.getInventory()->getEqSlot(EqSlotTypes::QUIVER);
    for (int i = 0; i < count; i++) {
        quiver->insertAcceptedItem(0, 0, makeArrow(breakChance));
    }
}

/// Draws the bow fully and shoots at @p target.
void shoot(Player& player, Position target, double draw = 1.0)
{
    player.setMeleeMode(false);
    player.startAttack(target);
    unsigned drawMs = player.getDrawTime().getTimeMs();
    player.updateAttack(Time((unsigned)(drawMs * draw)));
    player.releaseAttack(target);
}

int arrowsOnGround(giera_test::TestWorld& world, Position around, double radius)
{
    int n = 0;
    for (auto& item : world.board->getItemsNear(around, radius)) {
        n += item->getItemType() == ItemTypes::ARROW;
    }
    return n;
}
}

TEST_CASE("Bow", "bow mode needs an equipped bow")
{
    auto world = makeTestWorld();
    auto player = world.player;
    CHECK(!player->setMeleeMode(false)); // bow mode
    player->getInventory()->getEqSlot(EqSlotTypes::BOW)->removeItem(0, 0);
    player->setMeleeMode(true);
    CHECK(player->setMeleeMode(false)); // no bow: stays melee
    CHECK(player->meleeMode);
}

TEST_CASE("Bow", "pressing starts drawing, nothing flies yet")
{
    auto world = makeTestWorld();
    auto player = world.player;
    player->setMeleeMode(false);
    player->startAttack(Position(8, 1.5, 0));
    CHECK(player->isDrawing());
    CHECK(world.board->getProjectiles().empty());
    CHECK(player->canMove()); // you can walk while drawing
    CHECK(!player->canAttack());
}

TEST_CASE("Bow", "an aim line shows where the arrow will land while drawing")
{
    auto world = makeTestWorld();
    auto player = world.player;
    CHECK(!player->hasAimLine());
    player->setMeleeMode(false);
    Position target(14.5, 1.5, 0); // 13 m away: a weak draw can't reach it
    player->startAttack(target);
    unsigned drawMs = player->getDrawTime().getTimeMs();
    size_t drawablesBefore = player->getDrawables().size();
    player->updateAttack(Time(drawMs)); // fully drawn
    CHECK(player->hasAimLine());
    CHECK(player->getAimDotCount() > 5); // the dotted arc
    CHECK_EQ(player->getDrawables().size(), drawablesBefore + 1 + player->getAimDotCount()); // + ground line
    auto landing = player->getArrowLandingPoint();
    REQUIRE(landing.has_value());
    CHECK_BETWEEN(landing->getX(), target.getX() - 0.3, target.getX() + 0.3); // lands on the target
    CHECK_BETWEEN(landing->getY(), 1.2, 1.8);

    // a weak draw falls short of the same target
    player->cancelDrawing();
    CHECK(!player->hasAimLine());
    player->startAttack(target);
    player->updateAttack(Time(drawMs * 3 / 10));
    auto weak = player->getArrowLandingPoint();
    REQUIRE(weak.has_value());
    CHECK(weak->getX() < landing->getX() - 1.0);

    // aiming elsewhere moves the line; releasing removes it
    player->updateAttack(Time(drawMs));
    player->aimAt(Position(1.5, 6.5, 0));
    player->updateAttack(Time(16));
    CHECK(player->getArrowLandingPoint()->getY() > 4.0);
    player->releaseAttack(Position(1.5, 6.5, 0));
    CHECK(!player->hasAimLine());
    CHECK_EQ(player->getDrawables().size(), drawablesBefore);
}

TEST_CASE("Bow", "aim dots are red until releasing would shoot, then green")
{
    auto world = makeTestWorld();
    auto player = world.player;
    player->setMeleeMode(false);
    player->startAttack(Position(10.5, 1.5, 0));
    unsigned drawMs = player->getDrawTime().getTimeMs();
    double minDraw = CombatRules::get().minDrawToShoot;
    player->updateAttack(Time((unsigned)(drawMs * minDraw * 0.5)));
    CHECK(player->hasAimLine());
    CHECK(!player->canReleaseArrow()); // red: releasing now puts the arrow back
    player->updateAttack(Time((unsigned)(drawMs * minDraw)));
    CHECK(player->canReleaseArrow());  // green
    CHECK(player->hasAimLine());
}

TEST_CASE("Bow", "the aim arc follows the arrow's flight")
{
    auto world = makeTestWorld();
    auto player = world.player;
    player->setMeleeMode(false);
    Position target(14.5, 14.5, 0); // ~18 m: needs an upward shot
    player->startAttack(target);
    player->updateAttack(Time(player->getDrawTime().getTimeMs()));
    auto arc = player->getArrowTrajectory();
    REQUIRE(arc.size() > 10);
    CHECK_APPROX(arc.front().getZ(), CombatRules::get().arrowSpawnHeight); // leaves the bow at bow height
    CHECK_APPROX(arc.back().getZ(), 0.0);                                    // ends on the ground
    // a real arc: it rises above the bow before coming down, and the shot that follows
    // lands where the arc ends
    double highest = 0.;
    for (auto& p : arc) {
        highest = std::max(highest, p.getZ());
    }
    CHECK(highest > CombatRules::get().arrowSpawnHeight);
    Position end = arc.back();
    fillQuiver(*player, 1, 0.);
    player->releaseAttack(target);
    runProjectiles(world, 3000);
    auto items = world.board->getItemsNear(end, 1.0);
    CHECK_EQ(items.size(), (size_t)1);
}

TEST_CASE("Bow", "the aim arc stops at the first character in the way")
{
    auto world = makeTestWorld();
    auto player = world.player;
    auto ai = addEnemy(world, Position(6, 1.5, 0));
    player->setMeleeMode(false);
    player->startAttack(Position(12, 1.5, 0)); // aiming past the enemy
    player->updateAttack(Time(player->getDrawTime().getTimeMs()));
    auto end = player->getArrowLandingPoint();
    REQUIRE(end.has_value());
    CHECK_BETWEEN(end->getX(), 5.3, 6.5); // ends at the enemy, not at x = 12
}

TEST_CASE("Bow", "the aim line is removed when the bow is lowered or the player dies")
{
    auto world = makeTestWorld();
    auto player = world.player;
    player->setMeleeMode(false);
    player->startAttack(Position(7.5, 1.5, 0));
    player->updateAttack(Time(500));
    REQUIRE(player->hasAimLine());
    player->receiveHit(20); // getting hurt lowers the bow
    CHECK(!player->hasAimLine());
    player->startAttack(Position(7.5, 1.5, 0));
    player->updateAttack(Time(500));
    REQUIRE(player->hasAimLine());
    player->takeDamage(1000);
    CHECK(!player->hasAimLine());
}

TEST_CASE("Bow", "draw progress follows the bow draw time")
{
    auto world = makeTestWorld();
    auto player = world.player;
    unsigned drawMs = player->getDrawTime().getTimeMs();
    player->setMeleeMode(false);
    player->startAttack(Position(8, 1.5, 0));
    player->updateAttack(Time(drawMs / 2));
    CHECK_BETWEEN(player->getDrawProgress(), 0.45, 0.55);
    player->updateAttack(Time(drawMs));
    CHECK_APPROX(player->getDrawProgress(), 1.0);
}

TEST_CASE("Bow", "drawing while walking is slower and walking is slowed")
{
    auto world = makeTestWorld();
    auto player = world.player;
    unsigned drawMs = player->getDrawTime().getTimeMs();
    player->setMeleeMode(false);
    player->startAttack(Position(8, 1.5, 0));
    double speed = player->getMoveSpeedMultiplier();
    player->getInventory()->getEqSlot(EqSlotTypes::SHIELD)->removeItem(0, 0);
    double unencumbered = player->getMoveSpeedMultiplier(); // without the shield's -10 %
    CHECK_APPROX(unencumbered, CombatRules::get().moveSpeedWhileBusy);
    CHECK(speed < unencumbered);
    player->move(Position(1, 0, 0));
    CHECK_APPROX(player->getPosition().getX(), 1.5 + unencumbered);
    player->updateAttack(Time(drawMs / 2));
    CHECK_BETWEEN(player->getDrawProgress(), 0.2, 0.3);
}

TEST_CASE("Bow", "release shoots one arrow from the quiver")
{
    auto world = makeTestWorld();
    auto player = world.player;
    int before = player->getArrowCount();
    shoot(*player, Position(8, 1.5, 0));
    CHECK(!player->isDrawing());
    CHECK_EQ(world.board->getProjectiles().size(), (size_t)1);
    CHECK_EQ(player->getArrowCount(), before - 1);
}

TEST_CASE("Bow", "releasing too early keeps the arrow")
{
    auto world = makeTestWorld();
    auto player = world.player;
    int before = player->getArrowCount();
    shoot(*player, Position(8, 1.5, 0), CombatRules::get().minDrawToShoot / 2);
    CHECK(world.board->getProjectiles().empty());
    CHECK_EQ(player->getArrowCount(), before);
}

TEST_CASE("Bow", "damage scales with the draw")
{
    auto world = makeTestWorld();
    auto player = world.player;
    fillQuiver(*player, 2, 0.);
    double bowDamage = player->getSelectedBow()->getDamage()->getValue();
    shoot(*player, Position(8, 1.5, 0), 1.0);
    shoot(*player, Position(8, 1.5, 0), 0.5);
    auto& projectiles = world.board->getProjectiles();
    REQUIRE(projectiles.size() == 2);
    auto full = std::dynamic_pointer_cast<ArrowProjectile>(projectiles[0]);
    auto half = std::dynamic_pointer_cast<ArrowProjectile>(projectiles[1]);
    REQUIRE(full != nullptr);
    REQUIRE(half != nullptr);
    CHECK_APPROX(full->getDamage(), bowDamage + 10.);
    CHECK_BETWEEN(half->getDamage(), (bowDamage + 10.) * 0.45, (bowDamage + 10.) * 0.55);
}

TEST_CASE("Bow", "empty quiver: attacking is a melee swing")
{
    auto world = makeTestWorld();
    auto player = world.player;
    fillQuiver(*player, 0, 0.);
    player->setMeleeMode(false);
    player->startAttack(Position(8, 1.5, 0));
    CHECK(!player->isDrawing());
    CHECK(!player->canMove()); // swinging the sword
}

TEST_CASE("Bow", "arrow hits and damages an enemy")
{
    auto world = makeTestWorld();
    auto player = world.player;
    fillQuiver(*player, 1, 1.);
    auto ai = addEnemy(world, Position(6, 1.5, 0));
    auto bow = player->getSelectedBow();
    double reduction = ai->getDamageReduction(DamageTypes::PIERCING, bow->getDamage()->getAp()); // enemy armor - bow piercing
    double expected = (bow->getDamage()->getValue() + 10.) * (1. - reduction);
    shoot(*player, ai->getPosition() + Position(0, 0, 0.6));
    runProjectiles(world, 1500);
    CHECK(world.board->getProjectiles().empty());
    CHECK_BETWEEN((double)ai->getHp(), 100. - expected - 1., 100. - expected + 1.);
}

TEST_CASE("Bow", "arrow does not hit its shooter")
{
    auto world = makeTestWorld();
    shoot(*world.player, Position(10, 10, 0));
    runProjectiles(world, 1500);
    CHECK_EQ(world.player->getHp(), 100u);
}

TEST_CASE("Bow", "raised shield blocks arrows too")
{
    auto world = makeTestWorld();
    auto player = world.player;
    player->parry(Time(5000));
    auto ai = addEnemy(world, Position(6, 1.5, 0));
    double block = player->getDamageReduction(DamageTypes::PIERCING, 0); // body armor (60% vs piercing) + shield
    REQUIRE(block > 0.5);
    // the enemy has no bow - let an arrow projectile fly at the player directly
    world.board->addProjectile(std::make_shared<ArrowProjectile>(
        std::make_shared<FlightPath>(Position(6, 1.5, 1), player->getPosition() + Position(0, 0, 0.6), 1, 25),
        makeArrow(1.), arrowHit(40.), ai));
    runProjectiles(world, 1500);
    CHECK_BETWEEN((double)player->getHp(), 100. - 40. * (1 - block) - 1, 100. - 40. * (1 - block) + 1);
}

TEST_CASE("Bow", "missed arrow lands and can be picked up")
{
    auto world = makeTestWorld();
    auto player = world.player;
    fillQuiver(*player, 3, 0.); // never breaks
    shoot(*player, Position(8, 1.5, 0));
    runProjectiles(world, 1500);
    REQUIRE(world.board->getProjectiles().empty());
    CHECK_EQ(arrowsOnGround(world, Position(8, 1.5, 0), 1.0), 1);
    CHECK_EQ(player->getArrowCount(), 2);

    CHECK_EQ(player->pickUpArrows(), 0); // too far away
    for (int i = 0; i < 100 && player->getPosition().getX() < 7.8; i++) {
        player->move(Position(0.1, 0, 0)); // walk over to it
    }
    CHECK_EQ(player->pickUpArrows(), 1);
    CHECK_EQ(player->getArrowCount(), 3);
    CHECK_EQ(arrowsOnGround(world, Position(8, 1.5, 0), 1.0), 0);
}

TEST_CASE("Bow", "arrow with break chance 1 is gone after the impact")
{
    auto world = makeTestWorld();
    auto player = world.player;
    fillQuiver(*player, 1, 1.);
    shoot(*player, Position(8, 1.5, 0));
    runProjectiles(world, 1500);
    CHECK_EQ(arrowsOnGround(world, Position(8, 8, 0), 12.), 0);
}

TEST_CASE("Bow", "arrow hitting a wall lands in front of it")
{
    auto world = makeTestWorld();
    auto player = world.player;
    Position wallPos(5.5, 1.5, 0); // map tiles are positioned by their centre
    world.board->getMap()->setMapTile(Coordinates(5, 1), std::make_shared<MapTile>(wallPos,
        TerrainTypes::GRASS, Rotations::UP, ForegroundTypes::NONE, BackgroundTypes::NONE, WallTypes::ROCK));
    fillQuiver(*player, 1, 0.);
    shoot(*player, Position(9, 1.5, 0.5));
    runProjectiles(world, 1500);
    auto items = world.board->getItemsNear(Position(4, 1.5, 0), 2.0);
    REQUIRE(items.size() == 1);
    Position landed = items[0]->getBoardRect()->first;
    CHECK(landed.getX() < 5.0);
    CHECK(world.board->isStepablePosition(landed));
}

TEST_CASE("Bow", "arrows stuck in an enemy drop when it dies")
{
    auto world = makeTestWorld();
    auto player = world.player;
    fillQuiver(*player, 2, 0.);
    auto ai = addEnemy(world, Position(6, 1.5, 0));
    shoot(*player, ai->getPosition() + Position(0, 0, 0.6));
    runProjectiles(world, 1500);
    REQUIRE(!ai->isDead());
    CHECK_EQ(ai->getStuckArrows().size(), (size_t)1);
    CHECK_EQ(arrowsOnGround(world, ai->getPosition(), 1.5), 0);

    ai->takeDamage(ai->getHp() - 1);
    shoot(*player, ai->getPosition() + Position(0, 0, 0.6)); // the killing arrow
    runProjectiles(world, 1500);
    CHECK(ai->isDead());
    CHECK_EQ(arrowsOnGround(world, ai->getPosition(), 1.5), 2);
}

TEST_CASE("Bow", "full first quiver stack: picked-up arrow goes to the second stack")
{
    auto world = makeTestWorld();
    auto player = world.player;
    fillQuiver(*player, (int)CombatRules::get().quiverCapacity, 0.);
    auto arrow = makeArrow(0.);
    world.board->dropItem(player->getPosition(), arrow);
    CHECK_EQ(player->pickUpArrows(), 1);
    CHECK_EQ(player->getArrowCount(), (int)CombatRules::get().quiverCapacity + 1); // second stack
}

TEST_CASE("Bow", "inventory full: arrow stays on the ground")
{
    auto world = makeTestWorld();
    auto player = world.player;
    // fill both quiver stacks and the backpack completely
    fillQuiver(*player, (int)CombatRules::get().quiverCapacity, 0.);
    auto quiver = player->getInventory()->getEqSlot(EqSlotTypes::QUIVER);
    for (unsigned i = 0; i < CombatRules::get().quiverCapacity; i++) {
        quiver->insertAcceptedItem(1, 0, makeArrow(0.));
    }
    while (player->storeItem(makeArrow(0.))) {
    }
    auto arrow = makeArrow(0.);
    world.board->dropItem(player->getPosition(), arrow);
    CHECK_EQ(player->pickUpArrows(), 0);
    CHECK_EQ(arrowsOnGround(world, player->getPosition(), 0.5), 1);
}
