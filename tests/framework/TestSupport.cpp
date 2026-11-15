#include "TestSupport.h"
#include "Board.h"
#include "BoardRenderer.h"
#include "Map.h"
#include "MapTile.h"
#include "Player.h"
#include "Platform.h"
#include "Window.h"
#include "BaseItemHandler.h"
#include "TextureLoader.h"
#include "CombatRules.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <cstdlib>
#include <iostream>

namespace giera_test {

namespace {
std::shared_ptr<Window> sharedWindow;
}

void initTestEnvironment()
{
    Platform::initWorkingDirectory();
    Logger::setHandler(true, false);
    // GIERA_TEST_VERBOSE=1 shows the game log while tests run.
    Logger::setLevel(std::getenv("GIERA_TEST_VERBOSE") ? LoggingLevels::DEBUG : LoggingLevels::ERROR);

    SDL_SetHint(SDL_HINT_VIDEODRIVER, "dummy");
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
    }
    if (TTF_Init() < 0) {
        std::cerr << "TTF_Init failed: " << TTF_GetError() << "\n";
    }
    // Creates the renderer that TextureLoader / Texture use.
    sharedWindow = std::make_shared<Window>();
    BaseItemHandler::initialize();
    CombatRules::load();
}

void shutdownTestEnvironment()
{
    TextureLoader::clearCache();
    sharedWindow.reset();
    TTF_Quit();
    SDL_Quit();
}

std::shared_ptr<Window> testWindow()
{
    return sharedWindow;
}

TestWorld makeTestWorld(int sizeX, int sizeY)
{
    // Every test starts from the shipped rules, with chance rolls (crit, lifesteal) off
    // so damage numbers are predictable; tests that need a roll set forceRolls = true.
    CombatRules::resetToLoaded();
    CombatRules::get().forceRolls = false;
    TestWorld world;
    world.window = sharedWindow;
    auto map = std::make_unique<Map>(LandscapeTypes::GRASSLAND, MapTypes::QUEST_MAP, Directions::NORTH, sizeX, sizeY, 1);
    for (int x = 0; x < sizeX; x++) {
        for (int y = 0; y < sizeY; y++) {
            Position pos(x, y, 0);
            map->setMapTile(Coordinates(x, y), std::make_shared<MapTile>(pos, TerrainTypes::GRASS));
        }
    }
    world.player = std::make_shared<Player>();
    world.renderer = std::make_shared<BoardRenderer>(map->getSizeX(), map->getSizeY(), world.window, 20, world.player);
    world.board = std::make_shared<Board>(std::move(map), world.renderer);
    world.board->addPlayerCharacter(world.player);
    return world;
}

}
