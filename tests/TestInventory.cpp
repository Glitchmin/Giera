// Equipment slots. Based on the old (commented-out) EqTests in UnitTests/BasicTests.cpp,
// rewritten for the current slot constructors.
#include "TestFramework.h"
#include "BaseItemHandler.h"
#include "NormalItems.h"
#include "SingleEqSlot.h"
#include "MultipleEqSlot.h"
#include "QuiverEqSlot.h"
#include "ArrowTypes.h"
#include "MeleeWeaponTypes.h"
#include "ShieldTypes.h"

namespace {
shared_ptr<MeleeWeapon> sword()
{
    return BaseItemHandler::generate<MeleeWeapon>(ItemTypes::MELEE_WEAPON, (int)MeleeWeaponTypes::SWORD_2);
}
shared_ptr<Shield> shield()
{
    return BaseItemHandler::generate<Shield>(ItemTypes::SHIELD, (int)ShieldTypes::SHIELD_1);
}
shared_ptr<Arrow> arrow(ArrowTypes type)
{
    return BaseItemHandler::generate<Arrow>(ItemTypes::ARROW, (int)type);
}
}

TEST_CASE("Inventory", "single slot holds one accepted item")
{
    auto slot = std::make_shared<SingleEqSlot>(vector{ ItemTypes::MELEE_WEAPON });
    auto mel = sword();
    auto sh = shield();
    REQUIRE(mel != nullptr);
    REQUIRE(sh != nullptr);

    CHECK(slot->getItem(0, 0) == nullopt);
    CHECK(slot->isAccepted(0, 0, mel));
    CHECK(!slot->isAccepted(0, 0, sh)); // wrong type

    slot->insertAcceptedItem(0, 0, mel);
    REQUIRE(slot->getItem(0, 0).has_value());
    CHECK(slot->getItem(0, 0).value() == mel);
    CHECK(!slot->isAccepted(0, 0, sword())); // occupied

    auto removed = slot->removeItem(0, 0);
    REQUIRE(removed.has_value());
    CHECK(removed.value() == mel);
    CHECK(slot->getItem(0, 0) == nullopt);
    CHECK(slot->removeItem(0, 0) == nullopt);
}

TEST_CASE("Inventory", "empty type list accepts everything")
{
    auto slot = std::make_shared<SingleEqSlot>(vector<ItemTypes>{});
    CHECK(slot->isAccepted(0, 0, sword()));
    CHECK(slot->isAccepted(0, 0, shield()));
    CHECK(slot->isAccepted(0, 0, arrow(ArrowTypes::ARROW_1)));
}

TEST_CASE("Inventory", "backpack places items by their size")
{
    auto mel = sword();
    REQUIRE(mel != nullptr);
    const int w = mel->getWidth(), h = mel->getHeight();
    REQUIRE(w >= 1);
    REQUIRE(h >= 1);

    auto slot = std::make_shared<MultipleEqSlot>(vector<ItemTypes>{}, 6, 6, 36, true);
    CHECK(slot->isAccepted(0, 0, mel));
    slot->insertAcceptedItem(0, 0, mel);

    // every cell the item covers points at it, the rest stays empty
    for (int x = 0; x < 6; x++) {
        for (int y = 0; y < 6; y++) {
            bool covered = x < w && y < h;
            CHECK_EQ(slot->getItem(x, y).has_value(), covered);
        }
    }
    // overlapping placement is refused, but moving onto its own cells is fine
    CHECK(!slot->isAccepted(0, 0, sword()));
    CHECK(slot->isAccepted(0, 0, mel));
    // does not fit past the edge
    CHECK(!slot->isAccepted(6 - w + 1, 0, sword()));
    CHECK(!slot->isAccepted(-1, 0, sword()));

    auto removed = slot->removeItem(0, 0);
    REQUIRE(removed.has_value());
    CHECK(removed.value() == mel);
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            CHECK(slot->getItem(x, y) == nullopt);
        }
    }
}

TEST_CASE("Inventory", "drop slots ignore item size")
{
    auto slot = std::make_shared<MultipleEqSlot>(vector<ItemTypes>{}, 3, 2, 6, false);
    auto mel = sword();
    slot->insertAcceptedItem(2, 1, mel);
    CHECK(slot->getItem(2, 1).has_value());
    CHECK(slot->getItem(1, 1) == nullopt);
    CHECK(!slot->isAccepted(2, 1, sword()));
    CHECK(slot->isAccepted(0, 0, sword()));
}

TEST_CASE("Inventory", "backpack out of range access is safe (regression)")
{
    auto slot = std::make_shared<MultipleEqSlot>(vector<ItemTypes>{}, 2, 2, 4, false);
    CHECK(slot->getItem(5, 0) == nullopt);
    CHECK(slot->getItem(0, -1) == nullopt);
    CHECK(slot->removeItem(2, 2) == nullopt);
    slot->insertAcceptedItem(7, 7, sword()); // ignored, used to write past the vector
    for (int x = 0; x < 2; x++) {
        for (int y = 0; y < 2; y++) {
            CHECK(slot->getItem(x, y) == nullopt);
        }
    }
}

TEST_CASE("Inventory", "quiver stacks only one arrow kind per slot")
{
    auto slot = std::make_shared<QuiverEqSlot>(3);
    auto arr = arrow(ArrowTypes::ARROW_1);
    auto pen = arrow(ArrowTypes::THE_PENETRATOR);
    REQUIRE(arr != nullptr);
    REQUIRE(pen != nullptr);
    REQUIRE(arr->getName() != pen->getName());

    CHECK(slot->getItem(0, 0) == nullopt);
    CHECK(!slot->isAccepted(0, 0, sword())); // not an arrow
    CHECK(slot->isAccepted(0, 0, arr));
    slot->insertAcceptedItem(0, 0, arr);
    CHECK(slot->getItem(0, 0).has_value());

    CHECK(slot->isAccepted(0, 0, arrow(ArrowTypes::ARROW_1)));
    CHECK(!slot->isAccepted(0, 0, pen)); // different kind in the primary stack
    CHECK(slot->isAccepted(1, 0, pen));  // secondary stack is still empty

    slot->insertAcceptedItem(0, 0, arrow(ArrowTypes::ARROW_1));
    slot->insertAcceptedItem(0, 0, arrow(ArrowTypes::ARROW_1));
    CHECK_EQ(slot->getFillLevels().first, 3);
    CHECK(!slot->isAccepted(0, 0, arrow(ArrowTypes::ARROW_1))); // full

    slot->removeItem(0, 0);
    CHECK_EQ(slot->getFillLevels().first, 2);
    CHECK_EQ(slot->getFillLevels().second, 0);
}

TEST_CASE("Inventory", "quiver rejects invalid stack index (regression)")
{
    auto slot = std::make_shared<QuiverEqSlot>(3);
    auto arr = arrow(ArrowTypes::ARROW_1);
    CHECK(!slot->isAccepted(2, 0, arr));
    CHECK(!slot->isAccepted(-1, 0, arr));
    slot->insertAcceptedItem(2, 0, arr); // ignored
    CHECK(slot->getItem(2, 0) == nullopt);
    CHECK(slot->removeItem(5, 0) == nullopt);
    CHECK_EQ(slot->getFillLevels().first + slot->getFillLevels().second, 0);
}
