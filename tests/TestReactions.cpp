// Hit reactions and active defence: interrupting the bow, shield-block stagger, the timed
// parry + riposte without a shield, dashing, and eating food during a fight.
#include "TestFramework.h"
#include "TestSupport.h"
#include "AiCharacter.h"
#include "Board.h"
#include "CombatRules.h"
#include "EffectsHandler.h"
#include "DamageEffect.h"
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

void removeShield(Player& player)
{
    player.getInventory()->getEqSlot(EqSlotTypes::SHIELD)->removeItem(0, 0);
}

/// The enemy starts a swing at the player and finishes its wind-up.
void enemySwingLands(AiCharacter& ai, Player& player)
{
    ai.startAttack(player.getPosition());
    ai.updateAttack(ai.getMeleeTimings().first);
}
}

// ------------------------------------------------------------------ interrupting the bow

TEST_CASE("Reactions", "taking damage lowers a drawn bow")
{
    auto world = makeTestWorld();
    auto player = world.player;
    int arrows = player->getArrowCount();
    player->setMeleeMode(false);
    player->startAttack(Position(8, 1.5, 0));
    REQUIRE(player->isDrawing());
    player->receiveHit(30);
    CHECK(!player->isDrawing());
    CHECK_EQ(player->getArrowCount(), arrows); // the arrow stays in the quiver
}

TEST_CASE("Reactions", "bleeding does not lower the bow")
{
    auto world = makeTestWorld();
    auto player = world.player;
    player->addEffect(EffectsHandler::getEffect<DamageEffect>(EffectTypes::DAMAGE_EFFECT, 0), {});
    player->setMeleeMode(false);
    player->startAttack(Position(8, 1.5, 0));
    player->updateAttack(Time(500)); // two bleed ticks
    CHECK(player->getHp() < 100u);
    CHECK(player->isDrawing());
}

// ------------------------------------------------------------------ shield block

TEST_CASE("Reactions", "a melee hit on a fully raised shield staggers the attacker")
{
    auto world = makeTestWorld();
    auto player = world.player;
    auto ai = addEnemy(world, Position(2.5, 1.5, 0));
    player->parry(Time(5000));
    auto timings = ai->getMeleeTimings();
    enemySwingLands(*ai, *player);
    CHECK(player->getHp() < 100u); // the shield is armor, not immunity
    CHECK(ai->isStaggered());
    CHECK(!ai->canAttack());
    CHECK(!ai->canMove());
    // stagger = the rest of its swing (recovery) + shieldBlockStaggerMs
    Time total = timings.second + Time(CombatRules::get().shieldBlockStaggerMs);
    ai->updateAttack(total - Time(10));
    CHECK(ai->isStaggered());
    ai->updateAttack(Time(20));
    CHECK(!ai->isStaggered());
    CHECK(ai->canAttack());
}

TEST_CASE("Reactions", "a half raised shield does not stagger")
{
    auto world = makeTestWorld();
    auto player = world.player;
    auto ai = addEnemy(world, Position(2.5, 1.5, 0));
    player->parry(Time(player->getSelectedShield()->getTimeToRaise().getTimeMs() / 2));
    enemySwingLands(*ai, *player);
    CHECK(!ai->isStaggered());
}

// ------------------------------------------------------------------ timed parry (no shield)

TEST_CASE("Reactions", "without a shield a well timed block parries and ripostes")
{
    auto world = makeTestWorld();
    auto player = world.player;
    removeShield(*player);
    auto ai = addEnemy(world, Position(2.5, 1.5, 0));
    ai->startAttack(player->getPosition());
    auto windup = ai->getMeleeTimings().first;
    REQUIRE(windup.getTimeMs() > 50);
    ai->updateAttack(windup - Time(50));
    player->parry(Time(16)); // pressed 50 ms before the hit lands
    CHECK(player->isParryWindowOpen());
    CHECK_APPROX(player->getParryCompleteness(), 0.0); // nothing to raise
    ai->updateAttack(Time(50)); // the hit lands
    CHECK_EQ(player->getHp(), 100u);   // deflected
    CHECK(ai->getHp() < 100u);         // instant counter-hit
    CHECK(ai->isStaggered());
    ai->updateAttack(Time(CombatRules::get().riposteStaggerMs - 10));
    CHECK(ai->isStaggered()); // riposte stagger is the longer one
    CHECK(!player->isParryWindowOpen());
}

TEST_CASE("Reactions", "a parry pressed too early misses and has a cooldown")
{
    auto world = makeTestWorld();
    auto player = world.player;
    removeShield(*player);
    const auto& rules = CombatRules::get();
    player->parry(Time(16));
    REQUIRE(player->isParryWindowOpen());
    player->updateAttack(Time(rules.parryWindowMs)); // nothing came
    CHECK(!player->isParryWindowOpen());
    CHECK_EQ(player->getParryCooldownLeft().getTimeMs(), rules.parryMissCooldownMs);
    player->cancelParry();
    player->parry(Time(16)); // pressed again during the cooldown
    CHECK(!player->isParryWindowOpen());
    player->cancelParry();
    player->updateAttack(Time(rules.parryMissCooldownMs));
    player->parry(Time(16));
    CHECK(player->isParryWindowOpen());
}

TEST_CASE("Reactions", "a late parry does not help")
{
    auto world = makeTestWorld();
    auto player = world.player;
    removeShield(*player);
    auto ai = addEnemy(world, Position(2.5, 1.5, 0));
    enemySwingLands(*ai, *player);
    unsigned hp = player->getHp();
    REQUIRE(hp < 100u);
    player->parry(Time(16));
    CHECK_EQ(player->getHp(), hp);
    CHECK(!ai->isStaggered());
}

TEST_CASE("Reactions", "holding block does not reopen the parry window")
{
    auto world = makeTestWorld();
    auto player = world.player;
    removeShield(*player);
    const auto& rules = CombatRules::get();
    player->parry(Time(16));
    player->updateAttack(Time(rules.parryWindowMs + rules.parryMissCooldownMs));
    player->parry(Time(16)); // still held - not a new press
    CHECK(!player->isParryWindowOpen());
}

TEST_CASE("Reactions", "with a shield the block button raises the shield, no parry window")
{
    auto world = makeTestWorld();
    auto player = world.player;
    player->parry(Time(16));
    CHECK(!player->isParryWindowOpen());
    CHECK(player->getParryCompleteness() > 0.);
}

TEST_CASE("Reactions", "arrows can't be parried")
{
    auto world = makeTestWorld();
    auto player = world.player;
    removeShield(*player);
    player->parry(Time(16));
    HitInfo arrow;
    arrow.damage = 20;
    arrow.type = DamageTypes::PIERCING;
    arrow.melee = false;
    auto outcome = player->receiveHit(arrow);
    CHECK(outcome.landed);
    CHECK(!outcome.parried);
}

// ------------------------------------------------------------------ dash

TEST_CASE("Reactions", "dash moves in the walking direction")
{
    auto world = makeTestWorld();
    auto player = world.player;
    player->move(Position(0, 0.1, 0)); // walking down
    Position start = player->getPosition();
    REQUIRE(player->startDash());
    CHECK(player->isDashing());
    CHECK(!player->canMove());
    CHECK(!player->canAttack());
    player->updateAttack(Time(CombatRules::get().dashDurationMs));
    CHECK(!player->isDashing());
    CHECK_APPROX(player->getPosition().getY(), start.getY() + CombatRules::get().dashDistance);
    CHECK_APPROX(player->getPosition().getX(), start.getX());
}

TEST_CASE("Reactions", "dashing makes you invulnerable")
{
    auto world = makeTestWorld();
    auto player = world.player;
    player->startDash();
    auto outcome = player->receiveHit(HitInfo{ 50. });
    CHECK(!outcome.landed);
    CHECK_EQ(player->getHp(), 100u);
}

TEST_CASE("Reactions", "dash has a cooldown")
{
    auto world = makeTestWorld();
    auto player = world.player;
    REQUIRE(player->startDash());
    player->updateAttack(Time(CombatRules::get().dashDurationMs));
    CHECK(!player->startDash());
    player->updateAttack(Time(CombatRules::get().dashCooldownMs));
    CHECK(player->startDash());
}

TEST_CASE("Reactions", "dash lowers the bow and the shield")
{
    auto world = makeTestWorld();
    auto player = world.player;
    player->parry(Time(5000));
    player->startDash();
    CHECK_APPROX(player->getParryCompleteness(), 0.0);
}

TEST_CASE("Reactions", "a dash stops at walls")
{
    auto world = makeTestWorld(8, 8);
    auto player = world.player;
    Position wall(3.5, 1.5, 0);
    world.board->getMap()->setMapTile(Coordinates(3, 1), std::make_shared<MapTile>(wall, TerrainTypes::GRASS,
        Rotations::UP, ForegroundTypes::NONE, BackgroundTypes::NONE, WallTypes::ROCK));
    player->startDash(); // default direction: +x, towards the rock
    for (int i = 0; i < 20; i++) {
        player->updateAttack(Time(20));
    }
    CHECK(player->getPosition().getX() + 0.35 < 3.0);
}

// ------------------------------------------------------------------ eating

TEST_CASE("Reactions", "eating takes time, uses the food and heals over time")
{
    auto world = makeTestWorld();
    auto player = world.player;
    player->takeDamage(40);
    int food = player->getFoodCount();
    REQUIRE(food > 0);
    REQUIRE(player->eatFromSatchel(0));
    CHECK(player->isEating());
    CHECK(!player->canAttack());
    CHECK(player->canMove()); // walking is slowed, not stopped
    player->updateAttack(Time(CombatRules::get().eatTimeMs));
    CHECK(!player->isEating());
    CHECK_EQ(player->getFoodCount(), food - 1);
    REQUIRE(player->getActiveEffects().size() == 1);
    for (int t = 0; t < 3000; t += 100) {
        player->updateAttack(Time(100));
    }
    CHECK_EQ(player->getHp(), 80u); // berries: 10 x 2 hp
}

TEST_CASE("Reactions", "eating needs food and free hands")
{
    auto world = makeTestWorld();
    auto player = world.player;
    CHECK(!player->eatFromSatchel(3)); // empty cell
    player->startAttack(Position(5, 1.5, 0)); // swinging
    CHECK(!player->eatFromSatchel(0));
}

TEST_CASE("Reactions", "a dash interrupts eating and the food is kept")
{
    auto world = makeTestWorld();
    auto player = world.player;
    int food = player->getFoodCount();
    player->eatFromSatchel(0);
    player->updateAttack(Time(100));
    player->startDash();
    CHECK(!player->isEating());
    player->updateAttack(Time(CombatRules::get().eatTimeMs));
    CHECK_EQ(player->getFoodCount(), food);
    CHECK(player->getActiveEffects().empty());
}
