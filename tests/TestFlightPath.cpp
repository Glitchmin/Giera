// Projectile trajectories. Ported from FlightPathTest in the old UnitTests/BasicTests.cpp;
// the old versions slept on a real timer, these step a fixed 2 ms so they are deterministic.
#include "TestFramework.h"
#include "FlightPath.h"
#include "GameTime.h"
#include <cmath>

namespace {
// Flies until the projectile drops below the ground (or a step limit), returns the last position.
Position fly(FlightPath& path, bool* stayedFinite = nullptr)
{
    Position pos = path.getPosition();
    bool finite = true;
    for (int step = 0; step < 200000 && pos.getZ() >= 0; step++) {
        pos = path.updatePosition(Time(2));
        finite = finite && std::isfinite(pos.getX()) && std::isfinite(pos.getY()) && std::isfinite(pos.getZ());
        if (!finite) {
            break;
        }
    }
    if (stayedFinite) {
        *stayedFinite = finite;
    }
    return pos;
}
}

TEST_CASE("FlightPath", "straight throw along x lands on target")
{
    FlightPath path(Position(0, 0, 0.5), Position(120, 0, 0), 1, 40);
    CHECK(path.willReachTarget());
    Position end = fly(path);
    CHECK_BETWEEN(end.getX(), 119.0, 121.0);
    CHECK_BETWEEN(end.getY(), -0.1, 0.1);
}

TEST_CASE("FlightPath", "throw keeps its y offset")
{
    FlightPath path(Position(0, 5, 0.5), Position(120, 5, 0), 1, 40);
    Position end = fly(path);
    CHECK_BETWEEN(end.getX(), 119.0, 121.0);
    CHECK_BETWEEN(end.getY(), 4.9, 5.1);
}

TEST_CASE("FlightPath", "throw towards negative x")
{
    FlightPath path(Position(120, 5, 0.5), Position(0, 5, 0), 1, 40);
    Position end = fly(path);
    CHECK_BETWEEN(end.getX(), -1.0, 1.0);
    CHECK_BETWEEN(end.getY(), 4.9, 5.1);
}

TEST_CASE("FlightPath", "diagonal throw lands on target")
{
    FlightPath path(Position(0, 0, 0.5), Position(120, 120, 0), 1, 50);
    CHECK(path.willReachTarget());
    Position end = fly(path);
    CHECK_BETWEEN(end.getX(), 119.0, 121.1);
    CHECK_BETWEEN(end.getY(), 119.0, 121.1);
}

TEST_CASE("FlightPath", "short game-sized throw")
{
    // a game-sized throw: mass 1, force ~30, a few meters
    FlightPath path(Position(2, 2, 0), Position(6, 3, 0), 1, 32);
    CHECK(path.willReachTarget());
    Position end = fly(path);
    CHECK_BETWEEN(end.getX(), 5.8, 6.2);
    CHECK_BETWEEN(end.getY(), 2.8, 3.2);
}

TEST_CASE("FlightPath", "close target below the start is hit, not overshot (regression)")
{
    // An arrow leaves the bow at 1 m; a ground target 5 m away needs a downward shot.
    // Angles used to be limited to 0..45 degrees: the arrow flew flat and landed ~12 m away.
    FlightPath path(Position(1.5, 1.5, 1.0), Position(6.5, 1.5, 0), 1, 27);
    CHECK(path.willReachTarget());
    Position end = fly(path);
    CHECK_BETWEEN(end.getX(), 6.3, 6.7);
}

TEST_CASE("FlightPath", "too weak throw does not reach")
{
    FlightPath path(Position(0, 0, 0.5), Position(120, 120, 0), 1, 10);
    CHECK(!path.willReachTarget());
    Position end = fly(path);
    CHECK(end.getZ() < 0); // it still comes down
    CHECK(end.getX() < 120);
}

TEST_CASE("FlightPath", "target at the start position stays finite (regression)")
{
    // Used to divide 0/0: every position was NaN and the projectile was never removed.
    FlightPath path(Position(5, 5, 0.5), Position(5, 5, 0), 1, 30);
    bool finite = false;
    Position end = fly(path, &finite);
    CHECK(finite);
    CHECK(end.getZ() < 0);
}
