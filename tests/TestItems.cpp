// Ported from the old MS CppUnitTest project (UnitTests/BasicTests.cpp).
// These read the real CSV files in assets/baseItems; expectations were updated where
// the CSV data was intentionally changed after the old tests were written
// (armor 3x4, sword 2x5, shield 3x3, first arrow row is "arrow").
#include "TestFramework.h"
#include "BaseItemHandler.h"
#include "NormalItems.h"
#include "DamageEffect.h"
#include "Lifesteal.h"
#include "ArmorTypes.h"
#include "ArrowTypes.h"
#include "FoodTypes.h"
#include "MeleeWeaponTypes.h"
#include "MiscTypes.h"
#include "RangedWeaponTypes.h"
#include "ReadableTypes.h"
#include "ShieldTypes.h"
#include "ModifiersTypes.h"

TEST_CASE("Items", "armor from csv")
{
    auto arm = BaseItemHandler::generate<Armor>(ItemTypes::ARMOR, (int)ArmorTypes::ARMOR_1);
    REQUIRE(arm != nullptr);
    CHECK_EQ(arm->getName(), std::string("body_block"));
    CHECK_EQ(arm->getDescription(), std::string("blocks_your_body"));
    CHECK_BETWEEN(arm->getValue(), 1.0, 2.0);
    CHECK_EQ(arm->getWidth(), (item_size_t)3);
    CHECK_EQ(arm->getHeight(), (item_size_t)4);
    for (int i = 0; i < (int)ModifiersTypes::COUNT_MODIFIERS; i++) {
        CHECK_EQ(arm->getModifier((ModifiersTypes)i), (item_mod_t)0);
    }
    CHECK_BETWEEN(arm->getArmor(), 1.5, 2.0);
}

TEST_CASE("Items", "melee weapon from csv")
{
    auto mel = BaseItemHandler::generate<MeleeWeapon>(ItemTypes::MELEE_WEAPON, (int)MeleeWeaponTypes::LUFREWOP_SWORD);
    REQUIRE(mel != nullptr);
    CHECK_EQ(mel->getName(), std::string("world_destroyer"));
    CHECK_EQ(mel->getDescription(), std::string("destroys_your_world"));
    CHECK_BETWEEN(mel->getValue(), 10.0, 30.0);
    CHECK_EQ(mel->getWidth(), (item_size_t)2);
    CHECK_EQ(mel->getHeight(), (item_size_t)5);
    for (int i = 0; i < (int)ModifiersTypes::COUNT_MODIFIERS; i++) {
        CHECK_EQ(mel->getModifier((ModifiersTypes)i), (item_mod_t)i);
    }
    CHECK_BETWEEN(mel->getDamage()->getValue(), 1.0, 2.0);
    CHECK_EQ((int)mel->getDamage()->getDamageType(), (int)DamageTypes::SLASHING);
    CHECK_BETWEEN(mel->getDamage()->getAp(), 1.0, 1.5);
    CHECK(!mel->getDamage()->isLifesteal());
    CHECK_BETWEEN(mel->getAttackSpeed().getTimeMs(), 69u, 420u);
}

TEST_CASE("Items", "melee lifesteal weapon")
{
    auto mel = BaseItemHandler::generate<MeleeWeapon>(ItemTypes::MELEE_WEAPON, (int)MeleeWeaponTypes::SWORD_2);
    REQUIRE(mel != nullptr);
    CHECK_EQ(mel->getName(), std::string("lifestealer"));
    REQUIRE(mel->getDamage()->isLifesteal());
    auto& lfst = dynamic_cast<Lifesteal&>(*mel->getDamage());
    CHECK_BETWEEN(lfst.getProbability(), 70.0, 80.0);
    CHECK_BETWEEN(lfst.getPercentStolen(), 50.0, 75.0);
}

TEST_CASE("Items", "ranged weapon from csv")
{
    auto rang = BaseItemHandler::generate<RangedWeapon>(ItemTypes::RANGED_WEAPON, (int)RangedWeaponTypes::BOW_1);
    REQUIRE(rang != nullptr);
    CHECK_EQ(rang->getName(), std::string("THE_ARROW_YEETER"));
    CHECK_BETWEEN(rang->getValue(), 4.0, 12.0);
    CHECK_EQ(rang->getWidth(), (item_size_t)2);
    CHECK_EQ(rang->getHeight(), (item_size_t)5);
    CHECK_BETWEEN(rang->getDamage()->getValue(), 0.5, 2.5);
    CHECK(!rang->getDamage()->isLifesteal());
    CHECK_BETWEEN(rang->getDrawTime().getTimeMs(), 1000u, 1500u);
    CHECK_BETWEEN(rang->getArrowVelocity(), 2.0, 4.0);
}

TEST_CASE("Items", "ranged lifesteal weapon")
{
    auto bow = BaseItemHandler::generate<RangedWeapon>(ItemTypes::RANGED_WEAPON, (int)RangedWeaponTypes::BOW_2);
    REQUIRE(bow != nullptr);
    CHECK_EQ(bow->getName(), std::string("bow_of_lifesteal"));
    REQUIRE(bow->getDamage()->isLifesteal());
    auto& lfst = dynamic_cast<Lifesteal&>(*bow->getDamage());
    CHECK_BETWEEN(lfst.getProbability(), 10.0, 20.0);
    CHECK_BETWEEN(lfst.getPercentStolen(), 80.0, 90.0);
}

TEST_CASE("Items", "arrow with damage effect")
{
    auto arr = BaseItemHandler::generate<Arrow>(ItemTypes::ARROW, (int)ArrowTypes::ARROW_1);
    REQUIRE(arr != nullptr);
    CHECK_EQ(arr->getName(), std::string("arrow"));
    CHECK_BETWEEN(arr->getValue(), 2.0, 5.0);
    CHECK_EQ(arr->getWidth(), (item_size_t)1);
    CHECK_EQ(arr->getHeight(), (item_size_t)2);
    CHECK_BETWEEN(arr->getDamage()->getValue(), 1.0, 2.0);
    CHECK_EQ((int)arr->getDamage()->getDamageType(), (int)DamageTypes::PIERCING);
    REQUIRE(arr->getEffects().size() == 1);
    auto eff = std::dynamic_pointer_cast<DamageEffect>(arr->getEffects()[0]);
    REQUIRE(eff != nullptr);
    CHECK(!eff->IsBuff());
    CHECK_EQ(eff->getDuration().getTimeMs(), 1000u);
    CHECK_EQ(eff->getLevel(), (short)3);
    CHECK_APPROX(eff->getDamage()->getValue(), 1.5);
    CHECK_EQ(eff->getTickrate().getTimeMs(), 100u);
    CHECK_APPROX(eff->getDamageIncrease(), 1.2);
}

TEST_CASE("Items", "food with healing effect")
{
    auto food = BaseItemHandler::generate<Food>(ItemTypes::FOOD, (int)FoodTypes::BERRIES);
    REQUIRE(food != nullptr);
    CHECK_EQ(food->getName(), std::string("borowkis"));
    CHECK_EQ(food->getWidth(), (item_size_t)1);
    CHECK_EQ(food->getHeight(), (item_size_t)1);
    REQUIRE(food->getEffects().size() == 1);
    auto eff = std::dynamic_pointer_cast<DamageEffect>(food->getEffects()[0]);
    REQUIRE(eff != nullptr);
    CHECK(eff->IsBuff());
    CHECK_EQ((int)eff->getDamage()->getDamageType(), (int)DamageTypes::HEALING);
    CHECK_EQ(eff->getTickrate().getTimeMs(), 200u);
}

TEST_CASE("Items", "misc and readable")
{
    auto misc = BaseItemHandler::generate<Misc>(ItemTypes::MISC, (int)MiscTypes::MISC_1);
    REQUIRE(misc != nullptr);
    CHECK_EQ(misc->getName(), std::string("a_stone"));
    auto book = BaseItemHandler::generate<Readable>(ItemTypes::READABLE, (int)ReadableTypes::BUUK);
    REQUIRE(book != nullptr);
    CHECK_EQ(book->getName(), std::string("a_book"));
    CHECK_EQ(book->getText(), std::string("witaj_podrozny"));
}

TEST_CASE("Items", "shield from csv")
{
    auto shield = BaseItemHandler::generate<Shield>(ItemTypes::SHIELD, (int)ShieldTypes::SHIELD_1);
    REQUIRE(shield != nullptr);
    CHECK_EQ(shield->getName(), std::string("cock_block"));
    // Changed from 2x5 to 3x3 in Jan 2025 (the Android copy still had the old 2x5).
    CHECK_EQ(shield->getWidth(), (item_size_t)3);
    CHECK_EQ(shield->getHeight(), (item_size_t)3);
    CHECK_BETWEEN(shield->getArmor(), 1.0, 2.0);
    CHECK_BETWEEN(shield->getTimeToRaise().getTimeMs(), 1500u, 2000u);
}

TEST_CASE("Items", "penetrator arrow has no effects")
{
    auto arr = BaseItemHandler::generate<Arrow>(ItemTypes::ARROW, (int)ArrowTypes::THE_PENETRATOR);
    REQUIRE(arr != nullptr);
    CHECK_EQ(arr->getName(), std::string("penetrator"));
    CHECK_BETWEEN(arr->getValue(), 21.0, 37.0);
    CHECK_BETWEEN(arr->getDamage()->getValue(), 1.5, 2.5);
    CHECK(arr->getEffects().empty());
}

TEST_CASE("Items", "unknown subtype returns nullptr instead of UB (regression)")
{
    // ArmorTypes/ShieldTypes list more subtypes than the CSVs have rows. Reading past
    // the end of the vector used to be undefined behaviour (it hung this test run).
    CHECK(BaseItemHandler::generate<Armor>(ItemTypes::ARMOR, (int)ArmorTypes::GLITTER_THIGHS) == nullptr);
    CHECK(BaseItemHandler::generate<Shield>(ItemTypes::SHIELD, (int)ShieldTypes::COCC_BLOCC) == nullptr);
    CHECK(BaseItemHandler::generate<Armor>(ItemTypes::ARMOR, -1) == nullptr);
    CHECK(BaseItemHandler::generate<Armor>(ItemTypes::ARMOR, 1000) == nullptr);
}

TEST_CASE("Items", "csv row counts")
{
    auto countRows = [](ItemTypes type) {
        int n = 0;
        while (BaseItemHandler::getBaseItem(type, n) != nullptr) n++;
        return n;
    };
    CHECK_EQ(countRows(ItemTypes::ARMOR), 1);
    CHECK_EQ(countRows(ItemTypes::MELEE_WEAPON), 2);
    CHECK_EQ(countRows(ItemTypes::RANGED_WEAPON), 2);
    CHECK_EQ(countRows(ItemTypes::SHIELD), 1);
    CHECK_EQ(countRows(ItemTypes::ARROW), 2);
    CHECK_EQ(countRows(ItemTypes::FOOD), 1);
}

TEST_CASE("Items", "generated items vary within the csv range")
{
    double lo = 1e9, hi = -1e9;
    for (int i = 0; i < 200; i++) {
        auto mel = BaseItemHandler::generate<MeleeWeapon>(ItemTypes::MELEE_WEAPON, 0);
        lo = std::min<double>(lo, mel->getValue());
        hi = std::max<double>(hi, mel->getValue());
    }
    CHECK(lo >= 10.0);
    CHECK(hi <= 30.0);
    CHECK(hi - lo > 5.0);
}
