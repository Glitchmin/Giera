// Drives the real BoardLoop (same map, spawns and rules as the game) with a fixed
// time step and no input, to check the core gameplay loop:
// enemy spawns -> fights -> dies -> respawns; player dies -> respawns.
#include "TestFramework.h"
#include "TestSupport.h"
#include "BoardLoop.h"
#include "Player.h"

namespace {
std::unique_ptr<BoardLoop> makeLoop()
{
    return std::make_unique<BoardLoop>(giera_test::testWindow(), std::make_shared<InputConfig>());
}

void simulate(BoardLoop& loop, int ms, int stepMs = 16)
{
    for (int t = 0; t < ms; t += stepMs) {
        loop.update(Time(stepMs));
    }
}
}

TEST_CASE("GameLoop", "an enemy is spawned at start")
{
    auto loop = makeLoop();
    CHECK_EQ(loop->getBoard()->getAiCharacters().size(), (size_t)1);
    CHECK_EQ(loop->getStats().enemiesSpawned, 1);
    CHECK(!loop->getPlayer()->isDead());
}

TEST_CASE("GameLoop", "enemy approaches the idle player")
{
    auto loop = makeLoop();
    auto ai = loop->getBoard()->getAiCharacters().at(0);
    double before = (ai->getPosition() - loop->getPlayer()->getPosition()).getNorm();
    simulate(*loop, 2000);
    double after = (ai->getPosition() - loop->getPlayer()->getPosition()).getNorm();
    CHECK(after < before - 1.0);
}

TEST_CASE("GameLoop", "fighting player kills enemies and they respawn")
{
    auto loop = makeLoop();
    loop->setAutoplay(true);
    simulate(*loop, 15000);
    auto& stats = loop->getStats();
    CHECK(stats.enemiesKilled >= 2);
    CHECK(stats.enemiesSpawned >= stats.enemiesKilled);
    CHECK(stats.enemiesSpawned <= stats.enemiesKilled + 1);
    CHECK_EQ(stats.playerDeaths, 0);
}

TEST_CASE("GameLoop", "enemy respawn waits for the delay")
{
    auto loop = makeLoop();
    loop->getBoard()->getAiCharacters().at(0)->takeDamage(1000);
    loop->update(Time(16));
    CHECK_EQ(loop->getStats().enemiesKilled, 1);
    CHECK(loop->getBoard()->getAiCharacters().empty());
    simulate(*loop, BoardLoop::enemyRespawnDelay.getTimeMs() - 100);
    CHECK(loop->getBoard()->getAiCharacters().empty());
    simulate(*loop, 200);
    CHECK_EQ(loop->getBoard()->getAiCharacters().size(), (size_t)1);
    CHECK_EQ(loop->getStats().enemiesSpawned, 2);
}

TEST_CASE("GameLoop", "idle player gets killed and respawns")
{
    auto loop = makeLoop();
    simulate(*loop, 30000);
    auto& stats = loop->getStats();
    CHECK(stats.playerDeaths >= 1);
    CHECK(stats.playerRespawns >= 1);
    CHECK(stats.playerRespawns <= stats.playerDeaths);
}

TEST_CASE("GameLoop", "player respawn waits for the delay and restores hp")
{
    auto loop = makeLoop();
    auto player = loop->getPlayer();
    player->move(Position(1, 1, 0));
    player->takeDamage(1000);
    loop->update(Time(16));
    CHECK_EQ(loop->getStats().playerDeaths, 1);
    simulate(*loop, BoardLoop::playerRespawnDelay.getTimeMs() - 100);
    CHECK(player->isDead());
    simulate(*loop, 200);
    CHECK(!player->isDead());
    CHECK_EQ(player->getHp(), player->getMaxHp());
    CHECK(player->getPosition() == BoardLoop::playerSpawnPosition);
    CHECK_EQ(loop->getStats().playerRespawns, 1);
}

TEST_CASE("GameLoop", "rendering frames works headless")
{
    auto loop = makeLoop();
    for (int i = 0; i < 5; i++) {
        loop->update(Time(16));
        loop->render(Time(16));
    }
    CHECK_EQ(loop->getStats().framesRendered, 5);
    CHECK_EQ(loop->getStats().updates, 5);
}
