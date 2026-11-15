#pragma once
#include <memory>

class Board;
class Player;
class BoardRenderer;
class Window;

namespace giera_test {

/// Starts SDL with the headless "dummy" video driver and a software renderer,
/// so game objects that create textures can be constructed in tests.
void initTestEnvironment();
void shutdownTestEnvironment();

/// The (hidden, dummy-driver) window shared by all tests.
std::shared_ptr<Window> testWindow();

/// A small, fully wired game world (map + renderer + board + player) for tests.
struct TestWorld {
    std::shared_ptr<Window> window;
    std::shared_ptr<BoardRenderer> renderer;
    std::shared_ptr<Board> board;
    std::shared_ptr<Player> player;
};

/// Builds a board of the given size with no walls, so movement tests are deterministic.
/// Also resets CombatRules to the shipped file with forceRolls = false (no crits / lifesteal rolls).
TestWorld makeTestWorld(int sizeX = 16, int sizeY = 16);

}
