#include "TestFramework.h"
#include "FileHandler.h"
#include "GrasslandsGenerator.h"
#include "Map.h"
#include "MapFileHandler.h"
#include "MapTile.h"
#include <filesystem>

namespace {
/// Redirects FileHandler's save folder to a scratch folder for the duration of a test.
struct ScratchFolder {
    std::filesystem::path dir;
    ScratchFolder() {
        dir = std::filesystem::temp_directory_path() / "giera_tests";
        std::filesystem::remove_all(dir);
        std::filesystem::create_directories(dir);
        FileHandler::setSaveFolderPath(dir.generic_string());
    }
    ~ScratchFolder() {
        FileHandler::setSaveFolderPath("");
    }
};

std::string tileText(const Map& map, unsigned x, unsigned y) {
    std::stringstream ss;
    ss << *map.getMapTile(Coordinates(x, y));
    return ss.str();
}
}

TEST_CASE("Persistence", "file handler write append read")
{
    ScratchFolder scratch;
    FileHandler fh("test1", FileModeTypes::WRITE, ".txt", FileLocation::SAVES);
    REQUIRE(fh.isOpen());
    char c = 'h';
    fh.saveToFile(c);
    fh.closeFile();
    fh.openFile("test1", FileModeTypes::APPEND);
    c = 'a';
    fh.saveToFile(c);
    fh.saveToFile(c);
    fh.closeFile();

    fh.openFile("test1", FileModeTypes::READ);
    std::string all;
    for (int i = 0; i < 3; i++) {
        std::string token;
        fh.readFromFile(token);
        all += token;
    }
    fh.closeFile();
    // Before the fix nothing was ever written to disk, so this read back nothing.
    CHECK_EQ(all, std::string("haa"));
}

TEST_CASE("Persistence", "file handler ints")
{
    ScratchFolder scratch;
    {
        FileHandler fh("ints", FileModeTypes::WRITE, ".txt", FileLocation::SAVES);
        int v = 15;
        fh.saveToFile(v);
    } // destructor flushes
    {
        FileHandler fh("ints", FileModeTypes::APPEND, ".txt", FileLocation::SAVES);
        int v = 10;
        fh.saveToFile(v);
        fh.saveToFile(v);
    }
    FileHandler fh("ints", FileModeTypes::READ, ".txt", FileLocation::SAVES);
    int sum = 0;
    for (int i = 0; i < 3; i++) {
        int v = 0;
        fh.readFromFile(v);
        sum += v;
    }
    CHECK_EQ(sum, 35);
}

TEST_CASE("Persistence", "file handler missing file")
{
    ScratchFolder scratch;
    FileHandler fh("does_not_exist", FileModeTypes::READ, ".txt", FileLocation::SAVES);
    CHECK(!fh.isOpen());
    int v = 42;
    fh.readFromFile(v); // must not crash; nothing to read
    fh.closeFile();
}

TEST_CASE("Persistence", "reads shipped asset csv")
{
    // The default folder is assets/ next to the executable.
    FileHandler fh("baseItems/Shield", FileModeTypes::READ, ".csv");
    CHECK(fh.isOpen());
}

TEST_CASE("Persistence", "missing data file does not hang the loader (regression)")
{
    // A missing CSV used to make AbstractEntityFileLoader loop over an uninitialized
    // item count. FileHandler must report the failure so the loader can skip it.
    FileHandler fh("baseItems/NoSuchFile", FileModeTypes::READ, ".csv");
    CHECK(!fh.isOpen());
}

TEST_CASE("Persistence", "saves create their sub folder")
{
    ScratchFolder scratch;
    {
        FileHandler fh("nested/dir/file", FileModeTypes::WRITE, ".txt", FileLocation::SAVES);
        int v = 7;
        fh.saveToFile(v);
    }
    CHECK(std::filesystem::exists(scratch.dir / "nested" / "dir" / "file.txt"));
}

TEST_CASE("Persistence", "map constructor")
{
    Map map(LandscapeTypes::GRASSLAND, MapTypes::QUEST_MAP, Directions::NORTH, 10, 15, 10);
    CHECK_EQ(map.getSeed(), 10);
    CHECK_EQ(map.getSizeX(), 10u);
    CHECK_EQ(map.getSizeY(), 15u);
    CHECK(map.getMapChanges().empty());
    CHECK_EQ((int)map.getStartDirection(), (int)Directions::NORTH);
    CHECK_EQ((int)map.getMapType(), (int)MapTypes::QUEST_MAP);
    CHECK_EQ((int)map.getLandscapeType(), (int)LandscapeTypes::GRASSLAND);
}

TEST_CASE("Persistence", "same seed generates the same map")
{
    Map a(LandscapeTypes::GRASSLAND, MapTypes::QUEST_MAP, Directions::NORTH, 12, 12, 1234);
    Map b(LandscapeTypes::GRASSLAND, MapTypes::QUEST_MAP, Directions::NORTH, 12, 12, 1234);
    for (unsigned x = 0; x < 12; x++) {
        for (unsigned y = 0; y < 12; y++) {
            CHECK_EQ(tileText(a, x, y), tileText(b, x, y));
        }
    }
}

TEST_CASE("Persistence", "grasslands wall ratios")
{
    for (int seed = 1; seed <= 50; seed++) {
        Map map(LandscapeTypes::GRASSLAND, MapTypes::QUEST_MAP, Directions::NORTH, 15, 10, seed * 7919);
        int rocks = 0, bushes = 0;
        for (unsigned x = 0; x < map.getSizeX(); x++) {
            for (unsigned y = 0; y < map.getSizeY(); y++) {
                rocks += map.getMapTile(Coordinates(x, y))->getWallType() == WallTypes::ROCK;
                bushes += map.getMapTile(Coordinates(x, y))->getWallType() == WallTypes::BUSH;
            }
        }
        int tiles = map.getSizeX() * map.getSizeY();
        CHECK_BETWEEN(rocks, (int)(GrasslandsGenerator::getRockRatio().getMin() * tiles), (int)(GrasslandsGenerator::getRockRatio().getMax() * tiles));
        CHECK_BETWEEN(bushes, (int)(GrasslandsGenerator::getBushRatio().getMin() * tiles), (int)(GrasslandsGenerator::getBushRatio().getMax() * tiles));
    }
}

TEST_CASE("Persistence", "map save and load round trip")
{
    ScratchFolder scratch;
    Map map(LandscapeTypes::GRASSLAND, MapTypes::GIERA, Directions::NORTH, 10, 15, 4242);
    Position p(3, 3, 0);
    map.setMapTile(Coordinates(3, 3), std::make_shared<MapTile>(p, TerrainTypes::SAND, Rotations::RIGHT,
        ForegroundTypes::TALL_GRASS, BackgroundTypes::GRASS, WallTypes::BUSH));
    MapFileHandler handler;
    handler.saveMap(map);
    CHECK(std::filesystem::exists(scratch.dir / "maps" / ("map" + std::to_string((int)MapTypes::GIERA) + ".txt")));

    Map loaded = handler.readMap(MapTypes::GIERA);
    REQUIRE(loaded.getSizeX() == map.getSizeX());
    REQUIRE(loaded.getSizeY() == map.getSizeY());
    for (unsigned x = 0; x < map.getSizeX(); x++) {
        for (unsigned y = 0; y < map.getSizeY(); y++) {
            CHECK_EQ(tileText(loaded, x, y), tileText(map, x, y));
        }
    }
}
